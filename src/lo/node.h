#ifndef TFF_LO_NODE_H_
#define TFF_LO_NODE_H_

#include "../utility/unique_ptr_vector.h"
#include "../common.h"

#include <vector>

namespace tff {

class node_graph;
class node_input;
class node_output;

class node {
private:
	struct requestee {
		node& preceding_node;
		time_unit past_window;
		time_unit future_window;
	};
	
	node_graph& graph_;
	std::vector<requestee> requestees_;
	unique_ptr_vector<node_input> inputs_;
	unique_ptr_vector<node_output> outputs_;

protected:
	explicit node(node_graph&);
	node(const node&) = delete;
	node(node&&) = delete;
	node& operator(const node&) = delete;
	node& operator(node&&) = delete;
	
	virtual void do_request(time_span) = 0;
	virtual void do_launch() = 0;
	virtual void do_stop() = 0;
	
public:
	virtual ~node() = default;
	
	void request(time_span);
	void launch();
	void stop();
	
};


class node_input {
private:
	node& node_;
	
	time_unit past_window_ = 0;
	time_unit future_window_ = 0;
	node_output* connected_output_ = nullptr;

public:
	virtual ~node_input() = default;
};


class node_output {
private:
	node& node_;
	
	
};

};

#endif