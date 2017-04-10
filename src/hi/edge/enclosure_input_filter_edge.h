#ifndef TFF_ENCLOSURE_INPUT_FILTER_EDGE_H_
#define TFF_ENCLOSURE_INPUT_FILTER_EDGE_H_

#include "../edge/filter_edge_base.h"

namespace tff {

template<std::size_t Output_dim, typename Output_elem>
class enclosure_filter_input;

/// Edge connecting an internal input of a filter in an enclosure, to an input of the enclosure.
template<std::size_t Input_dim, typename Input_elem>
class enclosure_input_filter_edge : public filter_edge_input_base<Input_dim, Input_elem> {
	using base_in = filter_edge_input_base<Input_dim, Input_elem>;

	/*
	+-----[external input]------+
    |             |             |
	|             |             | enclosure filter
	|     [internal input]      |
	|      internal filter      |
	*/
	
public:
	using external_input_type = enclosure_filter_input<Input_dim, Input_elem>;
	using internal_input_type = filter_input<Input_dim, Input_elem>;
	using typename base_in::input_frame_shape_type;
	using typename base_in::input_full_view_type;
	
private:
	external_input_type& external_input_;
	
public:
	enclosure_input_filter_edge(external_input_type& external_in, internal_input_type& internal_in) :
		base_in(internal_in), external_input_(external_in) { }
	
	bool has_origin() const override { return false; }
	
	input_full_view_type input_view_from_opaque(const const_data_window_view_type& out_opaque_vw) const override {
		return external_input_.edge().input_view_from_opaque(out_opaque_vw);
	}

	input_frame_shape_type input_frame_shape() const override {
		return external_input_.frame_shape();
	}
};


template<std::size_t Input_dim, typename Input_elem>
void connect_enclosure_input(
	enclosure_filter_input<Input_dim, Input_elem>& external,
	filter_input<Input_dim, Input_elem>& internal
);

}

#include "enclosure_input_filter_edge.tcc"

#endif
