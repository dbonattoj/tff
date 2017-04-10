#ifndef TFF_ENCLOSURE_OUTPUT_FILTER_EDGE_H_
#define TFF_ENCLOSURE_OUTPUT_FILTER_EDGE_H_

#include "filter_edge_base.h"
#include "../enclosure/enclosure_filter.h"

namespace tff {

/// Edge connecting an internal output of a filter in an enclosure, to an output of the enclosure.
template<std::size_t Output_dim, typename Output_elem>
class enclosure_output_filter_edge : public filter_edge_output_base<Output_dim, Output_elem> {
	using base_out = filter_edge_output_base<Output_dim, Output_elem>;
	
	/*
	|      internal filter      |
	|     [internal output]     |
    |             |             | enclosure filter
	|             |             |
	+-----[external output]-----+
	*/
	
public:
	using external_output_type = enclosure_filter_output<Output_dim, Output_elem>;
	using internal_output_type = filter_output<Output_dim, Output_elem>;
	
private:
	external_output_type& external_output_;

public:
	enclosure_output_filter_edge(external_output_type& external_out, internal_output_type& internal_out) :
		base_out(internal_out), external_output_(external_out) { }
	
	bool has_destination() const override { return false; }
};


template<std::size_t Output_dim, typename Output_elem>
void connect_enclosure_output(
	filter_output<Output_dim, Output_elem>& internal,
	enclosure_filter_output<Output_dim, Output_elem>& external
);

template<std::size_t Output_dim, typename Output_elem>
void disconnect_enclosure_output(
	filter_output<Output_dim, Output_elem>& internal,
	enclosure_filter_output<Output_dim, Output_elem>& external
);

}

#include "enclosure_output_filter_edge.tcc"

#endif
