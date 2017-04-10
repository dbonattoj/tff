#include "node.h"
#include "node_input.h"
#include "node_output.h"
#include "../utility/exception.h"

namespace tff {

node::node(node_graph& gr, const std::string& nm) :
	graph_(gr), name_(nm) { }


node::~node() { }


node_input& node::add_input_() {
	inputs_.emplace_back(*this, inputs_.size());
	return inputs_.back();
}


node_output& node::add_output_(const node_read_guide& guide) {
	outputs_.emplace_back(*this, outputs_.size(), guide);
	return outputs_.back();
}


void node::verify() const {
	for(const node_output& out : outputs())
		if(! out.is_connected()) throw invalid_node_graph("all node outputs must be connected");

	for(const node_input& in : inputs())
		if(! in.is_connected()) throw invalid_node_graph("all node inputs must be connected");
}


void node::setup() { }


void node::forward_request_(time_unit t) {
	for(node_request_connection& req : request_receivers_) {
		time_span span = expand(t, req.window());
		req.receiver().request(truncate_begin(span));
	}
}


void node::forward_launch_() {
	for(node_request_connection& req : request_receivers_)
		req.receiver().launch();
}


void node::forward_stop_() {
	for(node_request_connection& req : request_receivers_)
		req.receiver().stop();
}


bool node::request_chain_contains_(const node& q_indirect_sender) const {
	Assert(has_request_sender() || is_sink());
	if(this == &q_indirect_sender) return true;
	else if(has_request_sender()) return request_sender().sender().request_chain_contains_(q_indirect_sender);
	else return false;
}


void node::add_request_receiver_(node& receiver, time_window window) {
	Assert(! receiver.has_request_sender());
	request_receivers_.emplace_back(*this, receiver, window);
	receiver.request_sender_ = &request_receivers_.back();
}


bool node::accumulated_time_window_to_(const node& target_successor_node, time_window& window) const {
	window = time_window(0, 0);

	if(this == &target_successor_node) return true;
	else if(is_sink()) return false;
	
	bool any_connected_to_target = false;
	for(const node_output& out : outputs()) {
		const node_input& conn_in = out.connected_input();
		const node& conn_node = conn_in.this_node();
		
		time_window partial_win(0, 0);
		bool connected_to_target = conn_node.accumulated_time_window_to_(target_successor_node, partial_win);

		if(connected_to_target) {
			window = max(window, partial_win + conn_in.window());
			any_connected_to_target = true;
		}
	}
	
	return any_connected_to_target;
}


void node::connect_to_request_sender_() {
	Assert(! has_request_sender());

	// find request sender for this node:
	// go down chain of successor's request senders ("request chains"),
	// and select first common request sender node
	node* sender = &outputs().front().connected_node();
	for(;;) {
		bool rejected = false;
		for(output_index_type i = 1; !rejected && (i < outputs().size()); ++i) {
			if(! outputs().at(i).connected_node().request_chain_contains_(*sender)) rejected = true;
		}
		if(rejected) sender = &sender->request_sender_node();
		else break;
	}
	
	// determine time window of request sender connection:
	// accumulated time window of all node connections between this node and the request sender node
	time_window request_window;
	bool ok = accumulated_time_window_to_(*sender, request_window);
	Assert(ok);
	
	// connect the request sender
	sender->add_request_receiver_(*this, request_window);
}


void node::propagate_request_connections_() {
	if(! is_sink()) {
		// skip if already connected to request sender
		if(has_request_sender()) return;

		// defer if direct successors of this node are not yet connected to request sender
		for(const node_output& out : outputs()) {
			const node& successor = out.connected_node();
			if(!successor.has_request_sender() && !successor.is_sink()) return;
		}
		
		// find and connect request sender for this node
		connect_to_request_sender_();
	}
	
	stage_ = stage::request_connection;
	
	// propagate to predecessors
	for(node_input& in : inputs())
		in.connected_node().propagate_request_connections_();
}


void node::propagate_setup_() {
	Assert(stage_ == stage::request_connection);
	for(node_request_connection& conn : request_receivers_) conn.receiver().propagate_setup_();
	this->setup();
	stage_ = stage::setup;
}


void node::sink_setup_() {
	Assert(is_sink());
	Assert(stage_ == stage::initial);
	propagate_request_connections_();
	propagate_setup_();
}


node_read_handle node::read(time_span, const node_read_guide&) {
	throw not_implemented("cannot read from sink_node");
}

}