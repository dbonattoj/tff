#ifndef TFF_FILTER_GRAPH_H_
#define TFF_FILTER_GRAPH_H_

#include "filter_subgraph.h"

namespace tff {

class filter_graph_installation;

class filter_graph : public filter_subgraph {
public:
	filter_graph();
	
	filter_graph_installation install() const;
};

}

#endif
