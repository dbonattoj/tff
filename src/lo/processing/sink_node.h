#ifndef TFF_SINK_NODE_H_
#define TFF_SINK_NODE_H_

#include "processing_node.h"
#include "../ring/ring.h"
#include "../ring/frame_metadata.h"
#include <string>

namespace tff {

class node_graph;

class sink_node : public processing_node {
private:
	void write_(rqueue_type::write_handle&);
	
public:
	sink_node(node_graph&, const std::string& name);
	
	thread_index_type input_reader_thread(input_index_type) const override;
	
	void setup_graph();
	
	void setup() override;
	
	frame_state process(time_unit);
	frame_state process_next();
};

}

#endif
