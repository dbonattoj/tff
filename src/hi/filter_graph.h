#ifndef TFF_FILTER_GRAPH_H_
#define TFF_FILTER_GRAPH_H_

#include "../common.h"
#include "filter_subgraph.h"
#include "../lo/node_graph.h"
#include <set>

namespace tff {

class filter_graph : public filter_subgraph {
private:
	optional<node_graph> installed_node_graph_;
	
public:
	bool was_setup() const;
	void setup();
	
	time_unit current_time() const;
	void run_until(time_unit last_frame);
	void run_for(time_unit duration);
	void run();
	void seek(time_unit target_time);
};

}

#endif
