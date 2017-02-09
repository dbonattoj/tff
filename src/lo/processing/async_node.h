#ifndef TFF_ASYNC_NODE_H_
#define TFF_ASYNC_NODE_H_

#include "processing_node.h"
#include "../worker.h"

namespace tff {

class async_node : public processing_node, public worker {
private:
	time_unit prefetch_duration_ = 0;
	
public:
	async_node(node_graph&, const std::string& name);

	void set_prefetch_duration(time_unit dur) { prefetch_duration_ = dur; }
	time_unit prefetch_duration() const { return prefetch_duration_; }
	
	thread_index_type input_reader_thread(input_index_type) const override;
	
	void request(time_span) override;
	void setup() override;
	void launch() override;
};

}

#endif