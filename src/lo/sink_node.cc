#include "sink_node.h"

namespace tff {

sink_node::sink_node(node_graph& gr, const std::string& name) :
	node(gr, name) { }


frame_state sink_node::process(time_unit t) {
	time_span span(t, t + 1);
	request(span);
	
	for(node_input& input : inputs()) {
		if(! input.is_activated()) continue;
		
		node_read_handle read_handle = input.read_frame(current_time_);
		if(! read_handle.valid()) throw flow_synchronization_error("transitory failure in sink node");
		
		frame_state input_frame_state_flag = read_handle.state().at_time(current_time_).flag;
		if(input_frame_state != frame_state_flag::success) return input_frame_state_flag;
	}

	return frame_state_flag::success;
}


frame_state sink_node::process_next() {
	return process(current_time() + 1);
}

void sink_node::setup() {
	if(outputs().size() != 0) throw invalid_flow_graph("sink_node must have no output");
}

	
time_unit sink_node::current_time() const {
	return current_time_;
}

thread_index_type sink_node::input_reader_thread(input_index_type) const {
	return graph().root_thread_index();
}

thread_index_type sink_node::request_sender_thread() const {
	return graph().root_thread_index();
}


}