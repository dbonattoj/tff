#include "filter_edge.h"
#include "filter_input.h"
#include "filter_output.h"

namespace tff {

const filter_output_base& filter_edge_base::origin() const {
	throw not_implemented("no filter edge origin");
}

filter_output_base& filter_edge_base::origin() {
	throw not_implemented("no filter edge origin");
}

const filter& filter_edge_base::origin_filter() const {
	return this->origin().this_filter();
}


filter& filter_edge_base::origin_filter() {
	return this->origin().this_filter();
}



const filter_input_base& filter_edge_base::destination() const {
	throw not_implemented("no filter edge origin");
}

filter_input_base& filter_edge_base::destination() {
	throw not_implemented("no filter edge origin");
}

const filter& filter_edge_base::destination_filter() const {
	return this->destination().this_filter();
}

filter& filter_edge_base::destination_filter() {
	return this->destination().this_filter();
}

}
