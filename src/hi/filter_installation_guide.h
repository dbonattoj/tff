#ifndef TFF_FILTER_INSTALLATION_GUIDE_H_
#define TFF_FILTER_INSTALLATION_GUIDE_H_

#include <map>

namespace tff {

class node_graph;
class node_output;
class processing_filter_base;
class processing_node;
class filter_edge_base;

class filter_installation_guide {
private:
	node_graph& node_graph_;
	std::map<const processing_filter_base*, processing_node*> local_processing_filter_nodes_; // TODO remove "local_"
	std::map<const filter_edge_base*, node_output*> edge_node_outputs_;

public:
	explicit filter_installation_guide(node_graph& gr) : node_graph_(gr) { }
	filter_installation_guide(const filter_installation_guide&) = default;
	
	node_graph& this_node_graph() const { return node_graph_; }
	
	bool has_processing_filter(const processing_filter_base&) const;

	void set_processing_filter_node(const processing_filter_base&, processing_node&);
	processing_node& processing_filter_node(const processing_filter_base&) const;
	
	void set_edge_node_output(const filter_edge_base&, node_output&);
	node_output& edge_node_output(const filter_edge_base&) const;
};

}

#endif