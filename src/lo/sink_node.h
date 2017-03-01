#ifndef TFF_SINK_NODE_H_
#define TFF_SINK_NODE_H_

#include "node.h"
#include "ring/frame_state.h"
#include <string>

namespace tff {

class sink_node final : public node {
private:
	time_unit current_time_ = -1;
	
public:
	explicit sink_node(node_graph&);

	void setup_graph();
	frame_state_flag process(time_unit);
	frame_state_flag process_next();
	
	void setup() override;
	
	node_input& add_input();
	
	time_unit current_time() const;

	thread_index_type input_reader_thread(input_index_type) const final override;
	thread_index_type request_sender_thread() const final override;
	
	void request(time_span) override;
	void launch() override;
	void stop() override;

};

}

#endif