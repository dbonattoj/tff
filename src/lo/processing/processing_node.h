#ifndef TFF_PROCESSING_NODE_H_
#define TFF_PROCESSING_NODE_H_

#include "../node.h"
#include "../ring/ring.h"

namespace tff {

class processing_handler {
public:
	virtual void handler_pre_process_() = 0;
	virtual void handler_process_() = 0;
};


class processing_node : public node {
private:
	ring ring_;
	rqueue queue_;

	time_unit current_time_ = undefined_time;
	
};


class processing_node_input : public node_input {
	
};


class processing_node_output : public node_output {
	
};


};

#endif
