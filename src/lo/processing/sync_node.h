#ifndef TFF_SYNC_NODE_H_
#define TFF_SYNC_NODE_H_

#include "processing_node.h"

namespace tff {

class sync_node : public processing_node {
public:
	explicit sync_node(node_graph&);
	
	thread_index_type input_reader_thread(input_index_type) const override;
	
	void setup() override;
};

}

#endif
