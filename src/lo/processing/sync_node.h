#ifndef TFF_SYNC_NODE_H_
#define TFF_SYNC_NODE_H_

#include "processing_node.h"

namespace tff {

class sync_node : public processing_node {
public:
	explicit sync_node(node_graph&);
	
	void setup() override;
};

}

#endif
