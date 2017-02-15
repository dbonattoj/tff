#include "../../lo/processing/processing_node.h"
#include "../../lo/processing/sync_node.h"
#include "../../lo/processing/async_node.h"

namespace tff {


template<typename Box>
void processing_filter<Box>::install_(filter_graph_installation_guide& guide) const {
	std::string node_name = "";
	
	// create processing node
	processing_node* installed_node = nullptr;
	if(asynchronous_) {
		async_node& nd = guide.graph.add_node<async_node>(node_name);
		installed_node = &nd;
	} else {
		sync_node& nd = guide.graph.add_node<sync_node>(node_name);
		installed_node = &nd;
	}
	
	// add channel for each output
	for(const filter_output& out : outputs()) {
		opaque_ndarray_format frame_format = out.ndarray_format();
	}
}

}