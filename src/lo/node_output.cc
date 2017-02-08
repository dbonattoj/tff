#include "node_output.h"
#include "node_input.h"
#include "node.h"

namespace tff {

void node_output::input_has_connected(const node_input& in) {
	connected_input_ = &in;
}


void node_output::input_has_disconnected() {
	connected_input_ = nullptr;
}
	

bool node_output::is_connected() const {
	return (connected_input_ != nullptr);
}


const node_input& node_output::connected_input() const {
	Assert(is_connected());
	return *connected_input_;
}


node_input& node_output::connected_input() {
	Assert(is_connected());
	return *connected_input_;
}


const node& node_output::connected_node() const {
	return connected_input().this_node();
}


node& node_output::connected_node() {
	return connected_input().this_node();
}


thread_index_type node_output::reader_thread() const {
	return connected_input().reader_thread();
}

	
node_read_handle node_output::read(time_span span) {
	return this_node().read_output(span, index_);
}


}