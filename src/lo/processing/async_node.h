#ifndef TFF_ASYNC_NODE_H_
#define TFF_ASYNC_NODE_H_

#include "processing_node.h"
#include "../worker.h"

namespace tff {

class async_node : public processing_node, public worker {
private:
	time_unit prefetch_duration_ = 0;
	
	void worker_main_() override;
	
	void transitory_failure_();
	
public:
	async_node(node_graph&, const std::string& name);

	void set_prefetch_duration(time_unit dur) { prefetch_duration_ = dur; }
	time_unit prefetch_duration() const { return prefetch_duration_; }
	
	thread_index_type processing_thread() const override;
	
	void request(time_span) override;
	void setup() override;
	void launch() override;
};

}

#endif