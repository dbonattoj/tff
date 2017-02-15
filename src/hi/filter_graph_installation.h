#ifndef TFF_FILTER_GRAPH_INSTALLATION_H_
#define TFF_FILTER_GRAPH_INSTALLATION_H_

#include <map>
#include "../nd/ndcoord_dyn.h"

namespace tff {

class node_graph;
class processing_node;
class filter;

struct filter_output_format {
	ndsize_dyn frame_shape;
};

struct filter_graph_installation_guide {
	node_graph& graph;
	std::map<const filter*, processing_node*> processing_filter_nodes;
	std::map<const filter_output*, filter_output_format> output_frame_formats;
};


class filter_graph_installation {
private:
	node_graph node_graph_;
	
public:
	
};

}

#endif