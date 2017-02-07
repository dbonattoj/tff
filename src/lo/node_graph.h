#ifndef TFF_NODE_GRAPH_H_
#define TFF_NODE_GRAPH_H_

#include "../utility/unique_ptr_vector.h"
#include "node.h"
#include <vector>

namespace tff {

class sink_node;

/// Low-level node graph.
/** Contains the nodes. Created during installation of high-level filter graph. */
class node_graph {
private:
	unique_ptr_vector<node> nodes_;
	sink_node* sink_ = nullptr;
	
public:
	node_graph();
	
	thread_index_type new_thread_index();
	static thread_index_type root_thread_index();
};

}

#endif