#ifndef TFF_SINK_NODE_H_
#define TFF_SINK_NODE_H_

#include "node.h"
#include "ring/frame_metadata.h"
#include <string>

namespace tff {

class sink_node : public node {
public:
	sink_node(node_graph&, const std::string& name);

	void setup_graph();
	
	void setup() override;
	
	frame_state process(time_unit);
	frame_state process_next();

	thread_index_type input_reader_thread(input_index_type) const final override;
	thread_index_type request_sender_thread() const final override;
	node_read_handle read_output(time_span, output_index_type) final override;
};

}

#endif