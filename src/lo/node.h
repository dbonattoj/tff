#ifndef TFF_LO_NODE_H_
#define TFF_LO_NODE_H_

#include "../common.h"
#include "../ring/read_handle.h"

#include <vector>
#include <string>

namespace tff {

class node_graph;
class node_input;
class node_output;

class node {
public:
	using ring_type = rqueue<ring>;
	
private:
	struct requestee {
		node& preceding_node;
		time_unit past_window = 0;
		time_unit future_window = 0;
		
		explicit requestee(node& prec) : preceding_node(prec) { }
	};
	
	node_graph& graph_;
	std::string name_;
	
	std::vector<requestee> requestees_;
	std::vector<node_input> inputs_;
	std::vector<node_output> outputs_;
	
protected:
	explicit node(node_graph&);
	node(const node&) = delete;
	node(node&&) = delete;
	node& operator(const node&) = delete;
	node& operator(node&&) = delete;
	
	node_input& add_input_();
	node_output& add_output_();
	
public:
	virtual ~node() = default;
	
	const auto& inputs() const { return inputs_; }
	const auto& outputs() const { return outputs_; }
	
	virtual void setup();
	
	virtual void request(time_span);
	virtual void launch();
	virtual void stop();
	
	virtual node_read_handle read_output(time_span, output_index_type) = 0;
};


class node_input {
private:
	node& node_;
	input_index_type index_;
	
	time_unit past_window_ = 0;
	time_unit future_window_ = 0;
	node_output* connected_output_ = nullptr;
	
	std::string name_;
	
	bool activated_ = false;

	node_input(const node_input&) = delete;
	node_input& operator=(const node_input&) = delete;
	
public:
	node_input(node& nd, input_index_type idx) : node_(nd), index_(ids) { }
	
	time_unit past_window() const { return past_window_; }
	time_unit future_window() const { return future_window_; }
	void set_past_window(time_unit);
	void set_future_window(time_unit);
	
	void connect(node_output&);
	void disconnect();
	bool is_connected() const;
	const node_output& connected_output() const;
	const node& connected_node() const;
	
	bool is_activated() const { return activated_; }
	void set_activated(bool);
	
	node_read_handle read_frame(time_unit);
};


class node_output {
private:
	node& node_;
	output_index_type index_;
	
	node_output(const node_output&) = delete;
	node_output& operator=(const node_output&) = delete;
	
public:
	node_output(node& nd, output_index_type idx) : node_(nd), index_(ids) { }
	
	node_read_handle read(time_span);
};

};

#endif