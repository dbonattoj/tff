#include "node_graph_relation.h"
#include "node.h"

#include <set>
#include <functional>
#include <algorithm>
#include <iterator>

namespace tff {

bool precedes(const node& a, const node& b) {
	if(&a == &b) return true;
	for(const node_output& out : a.outputs())
		if(precedes(out.connected_node(), b)) return true;
	return false;
}


bool precedes_strict(const node& a, const node& b) {
	if(&a == &b) return false;
	for(const node_output& out : a.outputs())
		if(precedes(out.connected_node(), b)) return true;
	return false;
}


const node& first_successor(const node& nd) {
	Assert(nd.outputs().size() > 0);
	
	if(nd.outputs().size() == 1) return nd.outputs().front().connected_node();
	
	using nodes_set_type = std::set<const node*>;
	
	// collect_all_successors(nd2, vec): adds ptrs to all successor nodes of `nd2` (and also `nd2` itself) into `vec`
	std::function<void(const node&, nodes_set_type&)> collect_all_successors;
	collect_all_successors = [&](const node& nd2, nodes_set_type& vec) {
		vec.insert(&nd2);
		for(const node_output& out : nd2.outputs())
			collect_all_successors(out.connected_node(), vec);
	};

	// common_successors := successors of node connected to first output of nd
	nodes_set_type common_successors;
	collect_all_successors(nd.outputs().front()->connected_node(), common_successors);
	
	// for the other outputs...
	for(auto it = nd.outputs().cbegin() + 1; it < nd.outputs().cend(); ++it) {
		// out_successors := successors of node connected to output `*it`
		const node& connected_node = it->connected_node();
		nodes_set_type out_successors;
		collect_all_successors(connected_node, out_successors);
		
		// common_successors := intersection(common_successors, out_successors)
		nodes_set_type old_common_successors = common_successors;
		common_successors.clear();
		std::set_intersection(
			old_common_successors.cbegin(), old_common_successors.cend(),
			out_successors.cbegin(), out_successors.cend(),
			std::inserter(common_successors, common_successors.begin())
		);
	}
	Assert(common_successors.size() > 0);
	// common_successors = nodes that are successors of every output
		
	// find node in `common_successors` that is not preceded by any other
	// (precedes_strict forms a partial order)
	auto it = std::find_if(
		common_successors.cbegin(), common_successors.cend(),
		[&common_successors](const node* a) { return std::none_of(
			common_successors.cbegin(), common_successors.cend(),
			[a](const node* b) { return precedes_strict(*b, *a); }
		); }
	);
	Assert(it != common_successors.cend());

	return **it;
}


node& first_successor(node& nd) {
	return const_cast<node&>(first_successor(static_cast<const node&>(nd)));
}


}