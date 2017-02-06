#include "processing_node.h"

namespace tff {


bool processing_node::call_handler_pre_process(processing_job& job) const {
	try {
		handler_->handler_pre_process_(job);
		return true;
	} catch(...) {
		return false;
	}
}


bool processing_node::call_handler_process(processing_job& job) const {
	try {
		handler_->handler_process_(job);
		return true;
	} catch(...) {
		return false;
	}
}


bool processing_node::write_next_(rqueue_write_handle& write_handle) {
	// set current time
	current_time_ = write_handle.time();
	
	// create job object, to be passed to processing handler
	processing_job job(*this, current_time_);
	
	// set writable frame view for each ndarray channel
	// handler will write into these
	for(channel_index_type chan = 0; chan < channels_count(); ++chan)
		job.set_channel_view(chan, write_handle.frame().ndarray(chan));
	
	// let handler pre-process frame
	bool pre_processed = call_handler_pre_process(job);
	if(! pre_processed) {
		// handler failure on preprocess:
		// write failure flag into metadata, and return
		write_handle.frame().metadata().state = ring::failure;
		return true;
	}
	
	// begin reading from activated inputs, and retain read handles
	// handles are released whenever node_read_handle are destroyed (RAII)
	std::vector<node_read_handle> open_read_handles;
	open_read_handles.reserve(inputs().size());

	for(node_input& input : inputs()) {
		if(! input.is_activated()) continue;
		
		// blocks until the span is available, or transitory failure occured
		// TODO first read sync nodes: async nodes will load in background, while sync node is processing upon read
		node_read_handle read_handle = input.read_frame(current_time_);
		
		// transitory failure: span to read is outside the current requested span
		if(! read_handle.valid()) return false;
		
		// propagate failure, or end of stream flag
		ring::frame_state input_frame_state = read_handle.view().metadata().at_time(current_time_).state;
		if(input_frame_state != ring::success) {
			write_handle.frame().metadata().state = input_frame_state;
			return true;
		}
		
		// retain read handle, and add view to job object
		job.set_input_view(input.index(), read_handle.view());
		open_read_handles.push_back(std::move(read_handle));
	}
	
	// now let handler process the frame
	bool processed = call_handler_process(job);
	if(! processed) {
		write_handle.frame().metadata().state = ring::failure;
		return true;
	}
	
	// close read handles now
	open_read_handles.clear();
	
	write_handle.frame().metadata().state = ring::success;
	
	return true;
}


void processing_node::setup() {
	
}


void processing_node::request(time_span span) {
	if(span.begin < 0) span.begin = 0;
	node::request(span);
	queue_->request(span);
}


void processing_node::launch() {
	node::launch();
}


void processing_node::stop() {
	node::stop();
	queue_->stop();
}


auto processing_node::read_output(time_span span, output_index_type idx) -> node_read_handle {
	rqueue_read_handle queue_handle = queue_->read(span);

	while(! queue_handle.valid()) {
		// transitory failure
		queue_handle = std::move(queue_->read(span));
	}
	
	channel_index_type channel_idx = output_channels_.at(idx);
	
	return node_read_handle(
		std::move(queue_handle),
		span.begin,
		channel_idx
	);
}


}