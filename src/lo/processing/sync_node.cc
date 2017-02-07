#include "sync_node.h"

namespace tff {

void sync_node::setup() {
	processing_node::setup();
	
	node_request_connection& req_sender = request_sender();
	std::size_t capacity = req_sender.past_window() + 1 + req_sender.future_window();
	
	processing_node::setup_ring_(rqueue_variant::sync, capacity);
}

}