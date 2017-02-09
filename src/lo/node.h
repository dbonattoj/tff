#ifndef TFF_LO_NODE_H_
#define TFF_LO_NODE_H_

#include "../common.h"
#include "types.h"
#include "node_request_connection.h"

#include <vector>
#include <string>

namespace tff {

class node_graph;
class node_input;
class node_output;
class node_read_handle;

/// Node in flow graph.
class node {
private:
	enum class stage { initial, request_connection, setup };
	
	node_graph& graph_;
	std::string name_;
	
	stage stage_ = stage::initial;
	
	std::vector<node_request_connection> request_receivers_;
	node_request_connection* request_sender_ = nullptr;
	
	std::vector<node_input> inputs_;
	std::vector<node_output> outputs_;
	
	bool request_chain_contains_(const node& q_indirect_sender) const;
	void add_request_receiver_(node& receiver, time_window window);
	bool accumulated_time_window_to_(const node& target_successor_node, time_window& out_window) const;
	void connect_to_request_sender_();
	void propagate_request_connections_();
	
	void propagate_setup_();

	node(const node&) = delete;
	node(node&&) = delete;
	node& operator=(const node&) = delete;
	node& operator=(node&&) = delete;
	
protected:
	node(node_graph&, const std::string& name);
	
	node_input& add_input_();
	node_output& add_output_();
	
	void sink_setup_();
	
public:
	virtual ~node();
	
	const node_graph& graph() const { return graph_; }
	node_graph& graph() { return graph_; }
	
	bool has_request_sender() const { return (request_sender_ != nullptr); }
	const node_request_connection& request_sender() const { Assert(has_request_sender()); return *request_sender_; }
	node_request_connection& request_sender() { Assert(has_request_sender()); return *request_sender_; }
	const node& request_sender_node() const { return request_sender().sender(); }
	node& request_sender_node() { return request_sender().sender(); }
	const auto& request_receivers() const { return request_receivers_; }

	auto& inputs() { return inputs_; } // TODO const_view to vector
	auto& outputs() { return outputs_; }
	const auto& inputs() const { return inputs_; }
	const auto& outputs() const { return outputs_; }
	bool is_source() const;
	bool is_sink() const;
	
	virtual void setup();
	virtual thread_index_type input_reader_thread(input_index_type) const = 0;
	
	virtual void request(time_span);
	virtual void launch();
	virtual void stop();
	
	virtual node_read_handle read_output(time_span, output_index_type) = 0;
};


}

#endif