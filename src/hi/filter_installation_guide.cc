#include "filter_installation_guide.h"
#include "filter_edge.h"
#include "processing/processing_filter.h"
#include "../lo/processing/processing_node.h"
#include "../lo/node_input.h"

namespace tff {

bool filter_installation_guide::has_processing_filter(const processing_filter_base& filt) const {
	auto it = processing_filter_nodes_.find(&filt);
	return (it != processing_filter_nodes_.end());
}


void filter_installation_guide::set_processing_filter_node(const processing_filter_base& filt, processing_node& nd) {
	auto result = processing_filter_nodes_.emplace(std::make_pair(&filt, &nd));
	Assert(result.second, "cannot insert processing node into guide multiple times for same filter");
}


processing_node& filter_installation_guide::processing_filter_node(const processing_filter_base& filt) const {
	auto it = processing_filter_nodes_.find(&filt);
	Assert(it != processing_filter_nodes_.end());
	return *(it->second);
}


bool filter_installation_guide::has_edge(const filter_edge_base& edge) const {
	auto it = edge_node_inputs_.find(&edge);
	return (it != edge_node_inputs_.end());
}


void filter_installation_guide::set_edge_node_input(const filter_edge_base& edge, node_input& in) {
	auto result = edge_node_inputs_.emplace(std::make_pair(&edge, &in));
	Assert(result.second, "cannot insert node input multiple times for same edge");
}


node_input& filter_installation_guide::edge_node_input(const filter_edge_base& edge) const {
	auto it = edge_node_inputs_.find(&edge);
	Assert(it != edge_node_inputs_.end());
	return *(it->second);
}


}
