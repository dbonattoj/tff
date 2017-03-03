#include "filter_edge.h"
#include "filter_input.h"
#include "filter_output.h"

namespace tff {

const filter& filter_edge_base::origin_filter() const {
	return this->origin().this_filter();
}


filter& filter_edge_base::origin_filter() {
	return this->origin().this_filter();
}


const filter& filter_edge_base::destination_filter() const {
	return this->destination().this_filter();
}


filter& filter_edge_base::destination_filter() {
	return this->destination().this_filter();
}

}