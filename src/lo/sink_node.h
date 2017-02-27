#ifndef TFF_SINK_NODE_H_
#define TFF_SINK_NODE_H_

#include "node.h"
#include "lo/ring/frame_state.h"
#include <string>

namespace tff {

class sink_node : public node {
public:
	sink_node(node_graph&, const std::string& name);

	frame_state process(time_unit);
	frame_state process_next();

	void setup();
	
	node_input& add_input();
	
	time_unit current_time() const;

	thread_index_type input_reader_thread(input_index_type) const final override;
	thread_index_type request_sender_thread() const final override;
};

}

#endif