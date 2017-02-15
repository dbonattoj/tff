#include "node_request_connection.h"
#include "node.h"

namespace tff {

thread_index_type node_request_connection::sender_thread() const {
	return sender().request_sender_thread();
}

}