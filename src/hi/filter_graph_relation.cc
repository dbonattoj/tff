#include "filter_graph_relation.h"
#include "filter.h"
#include "filter_output.h"

namespace tff {

bool precedes(const filter& a, const filter& b, bool strict) {
	if(&a == &b) return !strict;
	for(const filter_output_base& out : a.outputs()) {
		for(std::ptrdiff_t i = 0; i < out.edges_count(); ++i) {
			const filter_edge_base& edge = out.edge_at(i);
			if(! edge.has_destination()) continue;
			else if(precedes(edge.destination_filter(), b)) return true;
		}
	}
	return false;
}


bool succeedes(const filter& a, const filter& b, bool strict) {
	return precedes(b, a, strict);
}

}
