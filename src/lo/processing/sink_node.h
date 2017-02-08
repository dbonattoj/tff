#ifndef TFF_SINK_NODE_H_
#define TFF_SINK_NODE_H_

#include "processing_node.h"
#include <string>

namespace tff {

class node_graph;

class sink_node : public processing_node {
public:
	enum process_result {
		success,
		failure,
		end_of_stream
	};
	
	sink_node(node_graph&, const std::string& name);
	
	thread_index_type input_reader_thread(input_index_type) const override;
	
	void setup_graph();
	
	void setup() override;
	
	process_result process_next();
	void seek(time_unit);
};

}

#endif
