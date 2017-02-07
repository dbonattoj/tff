#ifndef TFF_ASYNC_NODE_H_
#define TFF_ASYNC_NODE_H_

#include "processing_node.h"
#include "../worker.h"

namespace tff {

class async_node : public processing_node, public worker {
private:
	time_unit prefetch_duration_ = 0;
};

};

#endif