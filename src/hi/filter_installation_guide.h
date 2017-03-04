#ifndef TFF_FILTER_INSTALLATION_GUIDE_H_
#define TFF_FILTER_INSTALLATION_GUIDE_H_

#include <map>
#include <vector>
#include "../utility/ref_vector.h"

namespace tff {

class node_graph;
class node_input;
class processing_filter_base;
class processing_node;
class filter_edge_base;

class filter_installation_guide {
private:
	node_graph& node_graph_;
	std::map<const processing_filter_base*, processing_node*> processing_filter_nodes_;
	std::map<const filter_edge_base*, node_input*> edge_node_inputs_;


public:
	explicit filter_installation_guide(node_graph& gr) : node_graph_(gr) { }
	filter_installation_guide(const filter_installation_guide&) = default;
	
	node_graph& this_node_graph() const { return node_graph_; }
	
	bool has_processing_filter(const processing_filter_base&) const;
	void set_processing_filter_node(const processing_filter_base&, processing_node&);
	processing_node& processing_filter_node(const processing_filter_base&) const;
	
	bool has_edge(const filter_edge_base&) const;
	void set_edge_node_input(const filter_edge_base&, node_input&);
	node_input& edge_node_input(const filter_edge_base&) const;
};

}

#endif