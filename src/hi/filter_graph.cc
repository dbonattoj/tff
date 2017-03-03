#include "filter_graph.h"
#include "filter_installation_guide.h"
#include "../lo/sink_node.h"
#include <exception>

namespace tff {


bool filter_graph::was_setup() const {
	return installed_node_graph_.has_value();
}


void filter_graph::setup() {
	installed_node_graph_.emplace();
	
	try {
		filter_installation_guide guide(*installed_node_graph_);
		for(filter& filt : filters())
			if(filt.is_sink()) filt.sink_propagate_install(guide);
		
		for(node_output* pull_nd_out : guide.sink_pull_node_outputs()) {
			node_input& sink_nd_in = installed_node_graph_->sink().add_input();
			sink_nd_in.connect(*pull_nd_out);
		}
		
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