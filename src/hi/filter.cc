#include "filter.h"
#include "filter_installation_guide.h"
#include "filter_input.h"
#include "filter_output.h"
#include "filter_subgraph.h"

#include <iostream>

namespace tff {


filter::filter(filter_subgraph& gr) : graph_(gr) { }

void filter::register_input(filter_input_base& in) {
	inputs_.push_back(in);
}


void filter::register_output(filter_output_base& out) {
	outputs_.push_back(out);
}


void filter::propagate_setup() {
	Assert(stage_ == stage::initial || stage_ == stage::was_setup);
	
	// stop propagation if already setup
	if(stage_ == stage::was_setup) return;
	
	// first propagate setup to direct predecessors
	for(filter_input_base& in : inputs()) {
		if(! in.is_connected() || ! in.edge().has_origin()) continue;
		in.edge().origin_filter().propagate_setup();
	}
	
	// direct predecessors are setup, now setup this
	this->setup_();
	stage_ = stage::was_setup;
}


void filter::propagate_install(filter_installation_guide& guide) {
	// filter cannot be orphaned (stage left on initial)
	Assert(stage_ == stage::was_setup || stage_ == stage::was_installed);
	
	// stop propagation if already installed
	if(stage_ == stage::was_installed) return;
	
	// delay if non-orphaned direct successors are not yet installed
	for(filter_output_base& out : outputs()) for(std::ptrdiff_t i = 0; i < out.edges_count(); ++i) {
		if(! out.edge_at(i).has_destination()) continue;
		filter& destination_filter = out.edge_at(i).destination_filter();
		if(destination_filter.stage_ == stage::was_setup) return;
	}

	// non-orphaned direct successors are installed, now install this
	this->install_(guide);
	stage_ = stage::was_installed;
	
	// now propagate installation to direct predecessors
	for(filter_input_base& in : inputs()) {
		if(! in.is_connected() || ! in.edge().has_origin()) continue;
		in.edge().origin_filter().propagate_install(guide);
	}
}

};