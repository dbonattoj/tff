#ifndef TFF_NODE_OUTPUT_H_
#define TFF_NODE_OUTPUT_H_

#include "types.h"

namespace tff {

class node;
class node_input;
class node_read_handle;

class node_output {
private:
	node& node_;
	output_index_type index_;
	
	node_input* connected_input_ = nullptr;
	
	node_output(const node_output&) = delete;
	node_output& operator=(const node_output&) = delete;
	
public:
	node_output(node& nd, output_index_type idx) :
		node_(nd), index_(idx) { }
	
	const node& this_node() const { return node_; }
	node& this_node() { return node_; }
	output_index_type index() const { return index_; }

	void input_has_connected(node_input&);
	void input_has_disconnected();
	
	bool is_connected() const;
	
	const node_input& connected_input() const;
	node_input& connected_input();
	const node& connected_node() const;
	node& connected_node();
	
	thread_index_type reader_thread() const;
	
	node_read_handle read(time_span);
};

}

#endif