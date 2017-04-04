#include "filter_subgraph.h"

namespace tff {

ref_vector<filter> filter_subgraph::pulled_filters() {
	ref_vector<filter> out_filters;
	for(filter& filt : filters()) if(filt.is_pulled()) out_filters.push_back(filt);
	return out_filters;
}


ref_vector<const filter> filter_subgraph::pulled_filters() const {
	ref_vector<const filter> out_filters;
	for(filter& filt : filters()) if(filt.is_pulled()) out_filters.push_back(filt);
	return out_filters;
}

}