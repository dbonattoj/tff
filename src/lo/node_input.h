#ifndef TFF_NODE_INPUT_H_
#define TFF_NODE_INPUT_H_

#include "../common.h"
#include "types.h"
#include <string>

namespace tff {

class node;
class node_output;
class node_read_handle;

class node_input {
private:
	node& node_;
	input_index_type index_;
	
	time_window window_;
	node_output* connected_output_ = nullptr;
	
	std::string name_;
	
	bool activated_ = false;

	node_input(const node_input&) = delete;
	node_input& operator=(const node_input&) = delete;
	
public:
	node_input(node& nd, input_index_type idx) :
		node_(nd), index_(idx) { }
	
	const node& this_node() const { return node_; }
	node& this_node() { return node_; }
	input_index_type index() const { return index_; }
	
	thread_index_type reader_thread() const;
	
	time_window window() const { return window_; }
	void set_window(time_window win) { window_ = win; }
	
	void connect(node_output&);
	void disconnect();
	bool is_connected() const;
	
	const node_output& connected_output() const;
	node_output& connected_output();
	const node& connected_node() const;
	node& connected_node();
	
	bool is_activated() const { return activated_; }
	void set_activated(bool);
	
	node_read_handle read_frame(time_unit);
};

}

#endif