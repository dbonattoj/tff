#include "sync_node.h"
#include "../node_output.h"

namespace tff {

void sync_node::setup() {
	processing_node::setup();
	
	node_request_connection& req_sender = request_sender();
	std::size_t capacity = req_sender.window().past + 1 + req_sender.window().future;
	
	processing_node::setup_ring_(rqueue_variant::sync, capacity);
}


thread_index_type sync_node::input_reader_thread(input_index_type) const {
	return outputs().front().reader_thread();
}


}