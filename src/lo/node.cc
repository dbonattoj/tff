#include "node.h"

namespace tff {

node::node(node_graph& gr) : graph_(gr) { }


node_request_connection& node::add_request_receiver(node& receiver) {
	Assert(! receiver.has_request_sender());
	request_receivers_.emplace_back(*this, receiver);
	receiver.request_sender_ = this;
	return request_receivers_.back();
}


node_input& node::add_input_() {
	inputs_.emplace_back(*this, inputs_.size());
	return inputs_.back();
}


node_output& node::add_output_() {
	outputs_.emplace_back(*this, outputs_.size());
	return outputs_.back();
}


void node::request(time_span span) {
	if(span.begin < 0) span.begin = 0;
	for(const requestee& req : requestees_)
		req.preceding_node.request(time_span(span.begin - req.past_window, span.end + req.future_window));
}


void node::launch() {
	for(const requestee& req : requestees_) req.preceding_node.launch();
}


void node::stop() {
	for(const requestee& req : requestees_) req.preceding_node.stop();
}



bool node::requester_is_setup_() const {
	return has_request_sender() || is_sink();
}


bool node::request_chain_contains_(const node& q_indirect_sender) const {
	Assert(requester_is_setup_());
	if(this == &q_indirect_sender) return true;
	else if(has_request_sender()) return request_sender().sender().request_chain_contains_(q_indirect_sender);
	else return false;
}


void node::add_request_receiver_(node& receiver, time_unit past_window, time_unit future_window) {
}


bool node::accumulated_time_window_to_(const node& target_successor_node, time_window& out_window) const {
	out_window = time_window(0, 0);

	if(this == &target_successor_node) return true;
	else if(is_sink()) return false;
	
	bool any_connected_to_target = false;
	for(const node_output& out : outputs()) {
		const node_input& conn_in = out.connected_input();
		const node& conn_node = conn_in.this_node();
		
		time_window partial_win;
		bool connected_to_target = conn_node.accumulated_time_window_to_(target_successor_node, partial_win);
		if(connected_to_target) {
			out_window = max(out_window, max(conn_in.window(), partial_win));
			any_connected_to_target = true;
		}
	}
	
	return any_connected_to_target;
}


void node::connect_to_request_sender_() {
	Assert(! has_request_sender());
	if(outputs().size() == 1) {
		node_output& out = outputs().front();
		node_input& conn_in = out.connected_input();
		
		conn_in.this_node().add_request_receiver_(*this, conn_in.past_window(), conn_in.future_window());
		return;
	}
	

	node* candidate = &outputs().front().connected_node();
	for(;;) {
		bool rejected = false;
		for(output_index_type i = 1; !rejected && (i < outputs().size()); ++i) {
			if(! outputs().at(i).connected_node().request_chain_contains_(*candidate)) rejected = true;
		}
		if(rejected) candidate = &candidate->request_sender_node();
		else break;
	}
	
	
	
}



void node::propagate_request_connections_() {
	auto connected = [](const node& nd) {
		
	};
	
	auto request_chain_contains()
	
	
	for(const node_output& out : outputs()) {
		const node& direct_successor = out.connected_node();
		if(! connected(direct_successor)) return;
	}
	
	if(outputs().size() == 1) {
		
	}
	
	const node* candidate = &outputs().front().connected_node();
	for(;;) {
		
		if()
	}
	
	
	
	
	if(! has_request_sender()) return; // or sink
	

	for(const node_input& in : inputs()) {
		node& direct_predecessor = in.connected_node();
		
	}

}


void node::propagate_pre_setup_() {
	// do nothing if this node was already pre_setup
	if(stage_ == stage::was_pre_setup) return;
	
	// do nothing if any of its direct successors are not yet pre_setup
	for(const node_output& out : outputs()) {
		const node& direct_successor = out.connected_node();
		if(direct_successor.stage_ != stage::was_pre_setup) return;
	}

	// pre_setup this node
	Assert(stage_ == stage::was_constructed);
	this->pre_setup();
	stage_ = stage::was_pre_setup;
	
	// recursively attempt to pre_setup direct predecessors
	for(const node_input& in : inputs()) {
		node& direct_predecessor = in.connected_node();
		direct_predecessor.propagate_pre_setup_();
	}
}


void node::propagate_setup_() {
	// do nothing if this node was already setup
	if(stage_ == stage::was_setup) return;
	
	// recursively attempt to setup its direct predecessors
	for(const node_input& in : inputs()) {
		node& direct_predecessor = in.connected_node();
		direct_predecessor.propagate_setup_();
	}

	// setup this node
	Assert(stage_ == stage::was_pre_setup);
	this->setup();
	
	stage_ = stage::was_setup;
}


}