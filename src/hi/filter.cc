#include "filter.h"
#include "filter_installation_guide.h"
#include "filter_input.h"

namespace tff {

void filter::register_input(filter_input_base& in) {
	inputs_.push_back(in);
}


void filter::register_output(filter_output_base& out) {
	outputs_.push_back(out);
}


void filter::propagate_setup_() {
	Assert(stage_ == stage::initial || stage_ == stage::was_setup);
	if(stage_ == stage::was_setup) return;
	for(filter_input_base& in : inputs()) {
		if(! in.is_connected()) continue;
		in.edge().origin_filter().propagate_setup_();
	}
	// direct predecessors are setup. now setup this
	this->setup_();
	stage_ == stage::was_setup;
}


void filter::propagate_install_(filter_installation_guide& guide) {
	Assert(stage_ == stage::was_setup || stage_ == stage::was_installed);
	
	if(stage_ == stage::was_setup) return;
	
	for(filter_output_base& out : outputs()) for(std::ptrdiff_t i = 0; i < out.edges_count(); ++i) {
		filter& destination_filter = out.edge_at(i).destination_filter();
		if(destination_filter.stage_ != stage::was_setup) return;
	}
	
	this->install_(guide);
	stage_ = stage::was_installed;
	
	for(filter_input_base& in : inputs()) {
		if(! in.is_connected()) continue;
		in.edge().origin_filter().propagate_install_(guide);
	}
}


void filter::sink_propagate_setup() {
	propagate_setup_();
}


void filter::sink_propagate_install(filter_installation_guide& guide) {
	propagate_install_(guide);
}

};