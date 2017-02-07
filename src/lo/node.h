#ifndef TFF_LO_NODE_H_
#define TFF_LO_NODE_H_

#include "../common.h"
#include "../ring/read_handle.h"
#include "node_request_connection.h"

#include <vector>
#include <string>

namespace tff {

class node_graph;
class node_input;
class node_output;

/// Node in flow graph.
class node {
public:
	using ring_type = rqueue<ring>;
	
private:
	enum class stage { was_constructed, was_pre_setup, was_setup };
	
	node_graph& graph_;
	std::string name_;
	
	stage stage_ = stage::was_constructed;
	
	std::vector<node_request_connection> request_receivers_;
	node_request_connection* request_sender_ = nullptr;
	
	std::vector<node_input> inputs_;
	std::vector<node_output> outputs_;
	
	/// Recursively pre-setup nodes in sink-to-source order.
	/** Must be called on sink node. Calls pre_setup() once on each node in graph, in an order such that when one node
	 ** is pre-setup, its successors have already been pre-setup. */
	void propagate_pre_setup_();
	
	/// Recursively setup nodes in source-to-sink order.
	/** Must be called on sink node. Calls setup() once on each node in graph, in an order such that when one node
	 ** is setup, its predecessors have already been setup */
	void propagate_setup_();

	node(const node&) = delete;
	node(node&&) = delete;
	node& operator(const node&) = delete;
	node& operator(node&&) = delete;
	
	bool requester_is_setup_() const;
	bool request_chain_contains_(const node&) const;
	void connect_to_request_sender_();
	
	void propagate_request_connections_();
	
protected:
	explicit node(node_graph&);
	
	node_input& add_input_();
	node_output& add_output_();
	
public:
	virtual ~node() = default;
	/*
	node_request_connection& add_request_receiver(node& receiver);
	bool has_request_sender() const { return (request_sender_ != nullptr); }
	*/
	const node_request_connection& request_sender() const { Assert(has_request_sender()); return *request_sender_; }
	node_request_connection& request_sender() { Assert(has_request_sender()); return *request_sender_; }
	const node& request_sender_node() const { request_sender().sender(); }
	node& request_sender_node() { request_sender().sender(); }
	const auto& request_receivers() const { return request_receivers_; }

	const auto& inputs() const { return inputs_; }
	const auto& outputs() const { return outputs_; }
	bool is_source() const { return (inputs_.size() == 0); }
	bool is_sink() const { return (outputs_.size() == 0); }
	
	virtual void pre_setup();
	virtual void setup();
	
	virtual void request(time_span);
	virtual void launch();
	virtual void stop();
	
	virtual node_read_handle read_output(time_span, output_index_type) = 0;
};


}

#endif