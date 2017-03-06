#ifndef TFF_FILTER_INSTALLATION_GUIDE_H_
#define TFF_FILTER_INSTALLATION_GUIDE_H_

#include <map>
#include <set>

namespace tff {

class node_graph;
class node_input;
class filter;
class processing_node;
class filter_edge_base;

class filter_installation_guide {
private:
	node_graph& node_graph_;
	std::set<const filter*> filters_to_install_;
	std::map<const filter*, processing_node*> filter_processing_nodes_;
	std::map<const filter_edge_base*, node_input*> edge_node_inputs_;


public:
	explicit filter_installation_guide(node_graph& gr) : node_graph_(gr) { }
	filter_installation_guide(const filter_installation_guide&) = default;
	
	node_graph& this_node_graph() const { return node_graph_; }
	
	bool has_filter_to_install(const filter&) const;
	void add_filter_to_install(const filter&);
	
	bool has_filter_processing_node(const filter&) const;
	void set_filter_processing_node(const filter&, processing_node&);
	processing_node& filter_processing_node(const filter&) const;
	
	bool has_edge(const filter_edge_base&) const;
	void set_edge_node_input(const filter_edge_base&, node_input&);
	node_input& edge_node_input(const filter_edge_base&) const;
};

}

#endif