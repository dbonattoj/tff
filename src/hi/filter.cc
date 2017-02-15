#include "filter.h"
#include "filter_graph_installation.h"
#include "filter_input.h"

namespace tff {

void filter::register_input(filter_input_base& in) {
	inputs_.push_back(in);
}


void filter::register_output(filter_output_base& out) {
	outputs_.push_back(out);
}


void filter::propagate_install(filter_graph_installation_guide& guide) const {
	for(filter_input_base& in : inputs()) {
		if(! in.is_connected()) continue;
		in.connected_filter().propagate_install(guide);
	}
	
	this->install_(guide);
}

};