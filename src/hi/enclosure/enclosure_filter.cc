#include "enclosure_filter.h"
#include "../filter_installation_guide.h"
#include <algorithm>

namespace tff {

thread_local enclosure_filter_base* enclosure_filter_base::current_in_construction_ = nullptr;


ref_vector<filter> enclosure_filter_base::root_internal_filters_() {
	ref_vector<filter> root_filters;
	
	// add new_filter to root_filters, if it is not already
	auto add = [&root_filters](filter& new_filter) {
		auto cmp = [&new_filter](const filter& f) { return (&f == &new_filter); };
		auto it = std::find_if(root_filters.begin(), root_filters.end(), cmp);
		if(it == root_filters.end()) root_filters.push_back(new_filter);
	};
	
	// add internal filters that are attached to an enclosure output
	for(filter_output_base& out : outputs())
		if(out.edges_count() > 0) add(out.internal_edge().origin_filter());
	
	// add pulled internal filters
	for(filter& pulled_filter : subgraph().pulled_filters())
		add(pulled_filter);
	
	return root_filters;
}

void enclosure_filter_base::setup_() {
	// verify
	if(subgraph().pulled_filters().size() > 0 && !is_pulled())
		throw invalid_filter_graph("enclosure_filter subgraph cannot have pulled filters if the enclosure_filter is not pulled itself");
	
	for(filter_output_base& out : outputs())
		if((out.edges_count() > 0) && ! out.has_internal_edge())
			throw invalid_filter_graph("all connected outputs of enclosure_filter must be attached to an internal output");
	
	// setup internal filters in source-to-sink order
	ref_vector<filter> root_filters = root_internal_filters_();
	for(filter& root_internal_filter : root_filters)
		root_internal_filter.propagate_setup();
	
	// verify that frame shapes for all attached outputs have been defined
	for(const filter_output_base& out : outputs())
		if(out.has_internal_edge())
			if(! out.internal_edge().origin().frame_shape_is_defined())
				throw filter_box_error("filters in enclosure did not define frame shape for all attached outputs");
}


void enclosure_filter_base::install_(filter_installation_guide& external_guide) {
	// TODO parallelization: install multiple times + insert splitter node

	// prepare internal guide
	std::string internal_name_prefix = name() + "/";
	filter_installation_guide internal_guide(external_guide.this_node_graph(), internal_name_prefix);
	
	// for enclosure output edges: put successor node_inputs outside the enclosure into internal guide
	for(const filter_output_base& out : outputs()) {
		if(! out.has_internal_edge()) continue;
		
		ref_vector<node_input> successor_node_inputs;
		for(std::ptrdiff_t i = 0; i < out.edges_count(); ++i) {
			const filter_edge_base& edge = out.edge_at(i);
			ref_vector<node_input> ins = external_guide.successor_edge_node_inputs(edge);
			successor_node_inputs.insert(successor_node_inputs.end(), ins.begin(), ins.end());
		}
		if(successor_node_inputs.empty()) continue;
		
		const filter_edge_base& internal_edge = out.internal_edge();
		for(node_input& successor_node_input : successor_node_inputs) {
			internal_guide.add_successor_edge_node_input(internal_edge, successor_node_input);
		}
	}
		
	// install non-orphaned internal filters in sink-to-source order
	ref_vector<filter> root_filters = root_internal_filters_();
	for(filter& root_internal_filter : root_filters)
		root_internal_filter.propagate_install(internal_guide);
			
	// for enclosure input edges: put node_inputs inside the enclosure into external guide
	for(const filter_input_base& in : inputs()) {
		for(std::ptrdiff_t i = 0; i < in.internal_edges_count(); ++i) {
			const filter_edge_base& internal_edge = in.internal_edge_at(i);
			ref_vector<node_input> node_ins = internal_guide.successor_edge_node_inputs(internal_edge);
			for(node_input& node_in : node_ins) external_guide.add_successor_edge_node_input(in.edge(), node_in);
		}
	}
}


}