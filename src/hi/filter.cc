#include "filter.h"
#include "filter_installation_guide.h"
#include "filter_input.h"
#include "filter_edge.h"

namespace tff {

void filter::register_input(filter_input_base& in) {
	inputs_.push_back(in);
}


void filter::register_output(filter_output_base& out) {
	outputs_.push_back(out);
}


void filter::propagate_install_(filter_installation_guide& guide) const {
	if(was_installed_) return;
	for(filter_input_base& in : inputs()) {
		if(! in.is_connected()) continue;
		in.edge().origin_filter().propagate_install_(guide);
	}
	// direct predecessors are installed. now install this
	this->install_(guide);
	was_installed_ = true;
}


void sink_propagate_install(filter_installation_guide& guide) {
	propagate_install_(guide);
}

};