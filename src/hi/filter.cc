#include "filter.h"
#include "filter_installation_guide.h"
#include "filter_input.h"
#include "filter_output.h"
#include <iostream>

namespace tff {

void filter::register_input(filter_input_base& in) {
	inputs_.push_back(in);
}


void filter::register_output(filter_output_base& out) {
	outputs_.push_back(out);
}


void filter::propagate_prepare_install(filter_installation_guide& guide) const {
	if(guide.has_filter_to_install(*this)) return;

	guide.add_filter_to_install(*this);
	
	for(const filter_input_base& in : inputs()) {
		if(! in.is_connected()) continue;
		in.edge().origin_filter().propagate_prepare_install(guide);
	}
}


void filter::propagate_setup() {
	Assert(stage_ == stage::initial || stage_ == stage::was_setup);
	if(stage_ == stage::was_setup) return;
	for(filter_input_base& in : inputs()) {
		if(! in.is_connected()) continue;
		in.edge().origin_filter().propagate_setup();
	}
	// direct predecessors are setup, now setup this
	this->setup_();
	stage_ = stage::was_setup;
}


void filter::propagate_install(filter_installation_guide& guide) {
	std::cout << "propagate_install " << name() << std::endl;
	
	Assert(stage_ == stage::was_setup || stage_ == stage::was_installed);
	
	if(stage_ == stage::was_installed) return;
	
	Assert(stage_ == stage::was_setup);
	Assert(guide.has_filter_to_install(*this));
	
	for(filter_output_base& out : outputs()) for(std::ptrdiff_t i = 0; i < out.edges_count(); ++i) {
		filter& destination_filter = out.edge_at(i).destination_filter();
		if(destination_filter.stage_ != stage::was_installed && guide.has_filter_to_install(destination_filter)) return;
	}

	// direct successors are installed, now install this
	this->install_(guide);
	stage_ = stage::was_installed;
	
	for(filter_input_base& in : inputs()) {
		if(! in.is_connected()) continue;
		in.edge().origin_filter().propagate_install(guide);
	}
}

};