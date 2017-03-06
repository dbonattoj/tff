#include "filter_graph.h"
#include "filter_installation_guide.h"
#include "../lo/sink_node.h"
#include <exception>

namespace tff {


bool filter_graph::was_setup() const {
	return installed_node_graph_.has_value();
}


ref_vector<filter> filter_graph::pulled_filters_() {
	ref_vector<filter> pulled_filters;
	for(filter& filt : filters()) if(filt.is_pulled()) pulled_filters.push_back(filt);
	return pulled_filters;
}


void filter_graph::setup() {
	installed_node_graph_.emplace();
	
	try {
		ref_vector<filter> pulled = pulled_filters_();
		filter_installation_guide guide(*installed_node_graph_);
	
		// install filters sink-to-source
		for(const filter& pulled_filt : pulled)
			pulled_filt.propagate_prepare_install(guide);
	
		// setup filters source-to-sink
		for(filter& pulled_filt : pulled)
			pulled_filt.propagate_setup();

		// install filters sink-to-source
		for(filter& pulled_filt : pulled)
			pulled_filt.propagate_install(guide);
		
		// setup the node graph
		installed_node_graph_->setup();
		
		
	} catch(const std::exception&) {
		installed_node_graph_.reset();
		throw;
	}
}



time_unit filter_graph::current_time() const {
	Assert(was_setup());
	return installed_node_graph_->current_time();
}


void filter_graph::run_until(time_unit last_frame) {
	Assert(was_setup());
	return installed_node_graph_->run_until(last_frame);
}


void filter_graph::run_for(time_unit duration) {
	Assert(was_setup());
	return installed_node_graph_->run_for(duration);
}


void filter_graph::run() {
	Assert(was_setup());
	return installed_node_graph_->run();
}


void filter_graph::seek(time_unit target_time) {
	Assert(was_setup());
	installed_node_graph_->seek(target_time);
}


}