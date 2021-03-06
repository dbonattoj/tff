#include "sync_node.h"
#include "../node_output.h"
#include <functional>

#include <iostream>

namespace tff {

sync_node::sync_node(node_graph& gr, const std::string& name) :
	processing_node(gr, name) { }


void sync_node::verify() const {
	processing_node::verify();
	
	if(outputs().size() == 0) throw invalid_node_graph("sync_node must have at least one output");
	
	for(output_index_type out = 1; out < outputs().size(); ++out)
		if(outputs().at(out).reader_thread() != processing_thread())
			throw invalid_node_graph("sync_node outputs must all be on the processing thread");
}


void sync_node::setup() {
	processing_node::setup();
	
	const node_request_connection& req_sender = request_sender();
	if(req_sender.sender_thread() != processing_thread())
		throw invalid_node_graph("sync_node requests must come from the processing thread");

	std::size_t capacity = req_sender.window().duration();
	
	processing_node::setup_ring_(rqueue_variant::sync, capacity);
	
	rqueue_().set_sync_writer(std::bind(&sync_node::write_, this, std::placeholders::_1));
}


void sync_node::write_(rqueue_type::write_handle& handle) {
	if(handle.has_stopped()) return;

	node::forward_request_(handle.time());
	
	//std::cout << name() << ":" << &handle.frame().state().flag << std::endl;
	handle.frame().state().flag = frame_state_flag::failure;

	bool write_success = processing_node::write_next_(handle);
	if(write_success) handle.commit();
}


thread_index_type sync_node::processing_thread() const {
	Assert(outputs().size() >= 1);
	return outputs().front().reader_thread();
}

	
void sync_node::request(time_span span) {
	processing_node::queue_request_(span);
}


void sync_node::launch() {
	node::forward_launch_();
}


void sync_node::stop() {
	node::forward_stop_();
	processing_node::queue_stop_();
}


}