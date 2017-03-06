#include "filter_installation_guide.h"
#include "filter_edge.h"
#include "filter.h"
#include "../lo/processing/processing_node.h"
#include "../lo/node_input.h"

namespace tff {

bool filter_installation_guide::has_filter_to_install(const filter& filt) const {
	auto it = filters_to_install_.find(&filt);
	return (it != filters_to_install_.end());
}


void filter_installation_guide::add_filter_to_install(const filter& filt) {
	filters_to_install_.insert(&filt);
}


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
