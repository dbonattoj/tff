#include "sync_node.h"
#include "../node_output.h"
#include <functional>

namespace tff {

sync_node::sync_node(node_graph& gr, const std::string& name) :
	processing_node(gr, name) { }

void sync_node::setup() {
	processing_node::setup();
	
	node_request_connection& req_sender = request_sender();
	std::size_t capacity = req_sender.window().past + 1 + req_sender.window().future;
	
	processing_node::setup_ring_(rqueue_variant::sync, capacity);
	
	rqueue_().set_sync_writer(std::bind(&sync_node::write_, this, std::placeholders::_1));
}


void sync_node::write_(rqueue_type::write_handle& handle) {
	if(handle.has_stopped()) return;
	processing_node::write_next_(handle);
}


thread_index_type sync_node::input_reader_thread(input_index_type) const {
	return outputs().front().reader_thread();
}


}