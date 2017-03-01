#include "node_input.h"
#include "node.h"
#include "node_output.h"
#include "ring/node_read_handle.h"

namespace tff {

thread_index_type node_input::reader_thread() const {
	return node_.input_reader_thread(index_);
}

	
void node_input::connect(node_output& out) {
	Assert(! is_connected());
	connected_output_ = &out;
	connected_output_->input_has_connected(*this);
}


void node_input::disconnect() {
	Assert(is_connected());
	connected_output_->input_has_disconnected();
	connected_output_ = nullptr;
}


bool node_input::is_connected() const {
	return (connected_output_ != nullptr);
}

	
const node_output& node_input::connected_output() const {
	Assert(is_connected());
	return *connected_output_;
}


node_output& node_input::connected_output() {
	Assert(is_connected());
	return *connected_output_;
}


const node& node_input::connected_node() const {
	return connected_output().this_node();
}


node& node_input::connected_node() {
	return connected_output().this_node();
}

	
void node_input::set_activated(bool act) {
	activated_ = act;
}


node_read_handle node_input::read_frame(time_unit t) {
	time_span span(t - window_.past, t + window_.future + 1);
	span = truncate(span);
	return connected_output_->read(span);
}

}