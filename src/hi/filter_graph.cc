#include "filter_graph.h"
#include "filter_installation_guide.h"

namespace tff {

void filter_graph::setup() {
	installed_node_graph_.emplace();
	installed_node_graph_->add_sink_node<sink_node>("sink");
	
	filter_installation_guide guide(*installed_node_graph_);
	for(const filter& filt : filters())
		if(filt.is_sink()) filt.propagate_install(guide);
}

}