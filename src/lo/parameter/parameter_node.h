#ifndef TFF_PARAMETER_NODE_H_
#define TFF_PARAMETER_NODE_H_

#include "../node.h"
#include "../ring/node_read_handle.h"
#include "../types.h"
#include "../../common.h"
#include <string>

namespace tff {

class parameter_node : public node {
public:
	parameter_node(node_graph&, const std::string& name);
	
	void setup() override;
	void request(time_span) override;
	node_read_handle read_output(time_span, output_index_type) override;
};

}

#endif
