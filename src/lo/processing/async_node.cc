#include "async_node.h"

namespace tff {

async_node::async_node(node_graph& gr, const std::string& name) :
	processing_node(gr, name),
	worker(gr.new_thread_index(), name) { }


thread_index_type async_node::input_reader_thread(input_index_type) const {
	return worker_thread_();
}


void async_node::setup() {
	processing_node::setup();
	
	bool different_output_reader_threads = false;
	thread_index_type first_output_reader_thread = outputs().front().reader_thread();
	for(output_index_type i = 1; (i < outputs().size()) && !different_output_reader_threads; ++i)
		if(outputs().at(i).reader_thread() != first_output_reader_thread) different_output_reader_threads = true;
	
	rqueue_variant variant = (different_output_reader_threads ? rqueue_variant::async_mpx : rqueue_variant::async);

	node_request_connection& req_sender = request_sender();
	std::size_t capacity = req_sender.past_window() + 1 + req_sender.future_window() + prefetch_duration_;

	processing_node::setup_ring_(variant, capacity);
}


void async_node::launch() {
	worker_launch_();
	processing_node::launch();
}


}