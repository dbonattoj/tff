#ifndef TFF_FILTER_INSTALLATION_GUIDE_H_
#define TFF_FILTER_INSTALLATION_GUIDE_H_

#include <map>
#include <string>
#include "../utility/ref_vector.h"

namespace tff {

class node_graph;
class node_input;
class filter;
class processing_node;
class filter_edge_base;

/// Temporary object maintained during filter graph installation to store (local) mapping to node graph.
class filter_installation_guide {
private:
	node_graph& node_graph_;
	std::string name_prefix_;
	
	std::map<const filter*, processing_node*> filter_processing_nodes_;
	std::multimap<const filter_edge_base*, node_input*> successor_edge_node_inputs_;

public:
	explicit filter_installation_guide(node_graph& gr, const std::string& name_prefix = "") :
		node_graph_(gr), name_prefix_(name_prefix) { }
	filter_installation_guide(const filter_installation_guide&) = default;
	
	node_graph& this_node_graph() const { return node_graph_; }
	const std::string& name_prefix() const { return name_prefix_; }
	
	bool has_filter_processing_node(const filter&) const;
	void set_filter_processing_node(const filter&, processing_node&);
	processing_node& filter_processing_node(const filter&) const;
	
	bool has_successor_edge_node_inputs(const filter_edge_base&) const;
	void add_successor_edge_node_input(const filter_edge_base&, node_input&);
	ref_vector<node_input> successor_edge_node_inputs(const filter_edge_base&) const;
};

}

#endif