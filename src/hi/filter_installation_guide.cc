#include "filter_installation_guide.h"
#include "filter_edge.h"
#include "processing/processing_filter.h"
#include "../lo/processing/processing_node.h"
#include "../lo/node_output.h"

namespace tff {

bool filter_installation_guide::has_processing_filter(const processing_filter_base& filt) const {
	auto it = local_processing_filter_nodes_.find(&filt);
	return (it != local_processing_filter_nodes_.end());
}


void filter_installation_guide::set_processing_filter_node(const processing_filter_base& filt, processing_node& nd) {
	auto result = local_processing_filter_nodes_.emplace(std::make_pair(&filt, &nd));
	Assert(result.second, "cannot insert processing node into guide multiple times for same filter");
}


processing_node& filter_installation_guide::processing_filter_node(const processing_filter_base& filt) const {
	auto it = local_processing_filter_nodes_.find(&filt);
	Assert(it != local_processing_filter_nodes_.end());
	return *(it->second);
}


void filter_installation_guide::set_edge_node_output(const filter_edge_base& edge, node_output& out) {
	auto result = edge_node_outputs_.emplace(std::make_pair(&edge, &out));
	Assert(result.second, "cannot insert node output multiple times for same edge");
}


node_output& filter_installation_guide::edge_node_output(const filter_edge_base& edge) const {
	auto it = edge_node_outputs_.find(&edge);
	Assert(it != edge_node_outputs_.end());
	return *(it->second);
}


}
