#include "async_node.h"
#include "../node_output.h"
#include "../node_graph.h"
#include "../../rqueue/rqueue.h"
#include <thread>

namespace tff {

async_node::async_node(node_graph& gr, const std::string& name) :
	processing_node(gr, name),
	worker(gr.new_thread_index(), name) { }


thread_index_type async_node::processing_thread() const {
	return worker_thread_();
}


void async_node::setup() {
	processing_node::setup();

	if(outputs().size() == 0) throw invalid_node_graph("async_node must have at least one output");
	
	bool different_output_reader_threads = false;
	thread_index_type first_output_reader_thread = outputs().front().reader_thread();
	for(output_index_type i = 1; (i < outputs().size()) && !different_output_reader_threads; ++i)
		if(outputs().at(i).reader_thread() != first_output_reader_thread) different_output_reader_threads = true;
	
	rqueue_variant variant = (different_output_reader_threads ? rqueue_variant::async_multiplex : rqueue_variant::async);

	node_request_connection& req_sender = request_sender();
	std::size_t capacity = req_sender.window().past + 1 + req_sender.window().future + prefetch_duration_;

	processing_node::setup_ring_(variant, capacity);
}


void async_node::launch() {
	worker_launch_();
}


void async_node::stop() {
	processing_node::queue_stop_();
}


void async_node::request(time_span span) {
	span.end += prefetch_duration_;
	processing_node::queue_request_(span);
}


void async_node::transitory_failure_() {
	std::this_thread::yield();
}


void async_node::worker_main_() {
	node::forward_launch_();
	
	for(;;) {
		bool succeeded;
		{
			rqueue_type::write_handle handle = rqueue_().write();
			if(handle.has_stopped()) break;
			node::forward_request_(time_span(handle.time(), handle.time() + 1));
			succeeded = processing_node::write_next_(handle);
		}
		if(! succeeded) transitory_failure_();
	}
	
	node::forward_stop_();
}


}
