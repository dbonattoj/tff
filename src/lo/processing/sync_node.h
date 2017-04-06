#ifndef TFF_SYNC_NODE_H_
#define TFF_SYNC_NODE_H_

#include "processing_node.h"
#include "../ring/ring.h"

namespace tff {

class sync_node : public processing_node {
private:
	void write_(rqueue_type::write_handle&);
	
public:
	explicit sync_node(node_graph&, const std::string& name);
	
	thread_index_type processing_thread() const override;
	
	void verify() const override;
	void setup() override;
	
	void request(time_span) override;
	void launch() override;
	void stop() override;
};

}

#endif
