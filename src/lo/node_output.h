#ifndef TFF_NODE_OUTPUT_H_
#define TFF_NODE_OUTPUT_H_

namespace tff {

class node;
class node_input;

class node_output {
private:
	node& node_;
	output_index_type index_;
	
	const node_input* connected_input_ = nullptr;
	
	node_output(const node_output&) = delete;
	node_output& operator=(const node_output&) = delete;
	
public:
	node_output(node& nd, output_index_type idx) : node_(nd), index_(ids) { }
	
	const node& this_node() const { return node_; }
	node& this_node() { return node_; }
	output_index_type index() const { return index_; }

	void input_has_connected(const node_input&);
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