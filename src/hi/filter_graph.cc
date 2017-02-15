#include "filter_graph.h"
#include "filter_graph_installation.h"

namespace tff {

filter_graph_installation filter_graph::install() const {
	filter_graph_installation installation;
	
	for(const filter& filt : filters())
		if(filt.is_sink()) filt.propagate_install();
	
	return installation;
}

}