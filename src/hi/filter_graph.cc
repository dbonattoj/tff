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
		// setup filters source-to-sink
		for(filter& sink : filters())
			if(sink.is_sink()) sink.sink_propagate_setup();

		// install filters sink-to-source
		filter_installation_guide guide(*installed_node_graph_);
		for(filter& sink : filters()) if(sink.is_sink()) {
			sink.sink_propagate_install(guide);

			processing_node& nd = guide.processing_filter_node(sink);
			node_output& pull_output = nd.add_pull_only_output();
			node_input& pull_input = installed_node_graph_->sink().add_input();
			pull_input.connect(pull_output);
		}
		
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