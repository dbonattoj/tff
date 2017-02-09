#ifndef TFF_NODE_GRAPH_H_
#define TFF_NODE_GRAPH_H_

#include "../utility/unique_ptr_vector.h"
#include "node.h"
#include <vector>
#include <utility>

namespace tff {

class sink_node;

/// Low-level node graph.
/** Contains the nodes. Created during installation of high-level filter graph. */
class node_graph {
private:
	unique_ptr_vector<node> nodes_;
	sink_node* sink_ = nullptr;
	
	thread_index_type last_thread_index_ = 0;
	
	node_graph(const node_graph&) = delete;
	node_graph& operator=(const node_graph&) = delete;
	
public:
	node_graph();
	~node_graph();
	
	thread_index_type new_thread_index() { return ++last_thread_index_; }
	static thread_index_type root_thread_index() { return 0; }
	
	template<typename Node, typename... Args>
	Node& add_node(Args&&... args) {
		static_assert(std::is_base_of<node, Node>::value, "node must be derived from class `node`");
		return nodes_.emplace_back<Node>(std::forward<Args>(args)...);
	}
	
	template<typename Node, typename... Args>
	Node& add_sink_node(Args&&... args) {
		static_assert(std::is_base_of<sink_node, Node>::value, "sink node must be derived from class `sink_node`");
		Node& sink = add_node<Node>(std::forward<Args>(args)...);
		sink_ = &sink;
		return sink;
	}
};

}

#endif