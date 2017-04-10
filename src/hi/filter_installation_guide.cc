#include "filter_installation_guide.h"
#include "edge/filter_edge_base.h"
#include "filter.h"
#include "../lo/processing/processing_node.h"
#include "../lo/node_input.h"

namespace tff {

bool filter_installation_guide::has_filter_processing_node(const filter& filt) const {
	auto it = filter_processing_nodes_.find(&filt);
	return (it != filter_processing_nodes_.end());
}


void filter_installation_guide::set_filter_processing_node(const filter& filt, processing_node& nd) {
	auto result = filter_processing_nodes_.emplace(std::make_pair(&filt, &nd));
	Assert(result.second, "cannot insert processing node into guide multiple times for same filter");
}


processing_node& filter_installation_guide::filter_processing_node(const filter& filt) const {
	auto it = filter_processing_nodes_.find(&filt);
	Assert(it != filter_processing_nodes_.end());
	return *(it->second);
}


bool filter_installation_guide::has_successor_edge_node_inputs(const filter_edge_base& edge) const {
	auto it = successor_edge_node_inputs_.find(&edge);
	return (it != successor_edge_node_inputs_.end());
}


void filter_installation_guide::add_successor_edge_node_input(const filter_edge_base& edge, node_input& in) {
	successor_edge_node_inputs_.emplace(std::make_pair(&edge, &in));
}


ref_vector<node_input> filter_installation_guide::successor_edge_node_inputs(const filter_edge_base& edge) const {
	auto it_range = successor_edge_node_inputs_.equal_range(&edge);
	ref_vector<node_input> inputs;
	for(auto it = it_range.first; it != it_range.second; ++it)
		inputs.push_back(*it->second);
	return inputs;
}

}
