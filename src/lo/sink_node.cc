#include "sink_node.h"
#include "node_input.h"
#include "node_output.h"
#include "node_graph.h"

#include <iostream>

namespace tff {

sink_node::sink_node(node_graph& gr) :
	node(gr, "sink") { }


frame_state_flag sink_node::process(time_unit t) {
	current_time_ = t;
	
	time_span span(t, t + 1);
	request(span);
	
	for(node_input& input : inputs()) {
		if(! input.is_activated()) continue;
		
		node_read_handle read_handle = input.read_frame(current_time_);
		if(! read_handle.valid()) std::cout << "sink trans fail reading " << current_time_ << " from " << input.connected_node().name() << std::endl;
		if(! read_handle.valid()) throw flow_synchronization_error("transitory failure in sink node");
		
		frame_state_flag input_frame_state = read_handle.state().at_time(current_time_).flag;
		if(input_frame_state != frame_state_flag::success) return input_frame_state;
	}

	return frame_state_flag::success;
}


frame_state_flag sink_node::process_next() {
	return process(current_time() + 1);
}


void sink_node::setup_graph() {
	node::sink_setup_();
}


void sink_node::setup() {
	node::setup();
	
	if(outputs().size() != 0) throw invalid_node_graph("sink_node must have no output");
}


node_input& sink_node::add_input() {
	return node::add_input_();
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


void sink_node::request(time_span span) {
	node::forward_request_(span);
}


void sink_node::launch() {
	node::forward_launch_();
}


void sink_node::stop() {
	node::forward_stop_();
}



}