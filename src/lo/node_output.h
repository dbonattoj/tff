#ifndef TFF_NODE_OUTPUT_H_
#define TFF_NODE_OUTPUT_H_

#include "types.h"
#include "ring/node_read_guide.h"

namespace tff {

class node;
class node_input;
class node_read_handle;

class node_output {
private:
	node& node_;
	output_index_type index_;
	node_read_guide guide_;
	
	std::string name_ = "out";
	
	node_input* connected_input_ = nullptr;
	
	node_output(const node_output&) = delete;
	node_output& operator=(const node_output&) = delete;
	
public:
	node_output(node& nd, output_index_type idx, const node_read_guide& guide) :
		node_(nd), index_(idx), guide_(guide) { }
	
	const node& this_node() const { return node_; }
	node& this_node() { return node_; }
	output_index_type index() const { return index_; }

	const std::string& name() const { return name_; }
	void set_name(const std::string& nm) { name_ = nm; }
	
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