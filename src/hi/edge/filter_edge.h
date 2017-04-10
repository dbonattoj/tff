#ifndef TFF_FILTER_EDGE_H_
#define TFF_FILTER_EDGE_H_

#include "filter_edge_base.h"

namespace tff {

/// Edge connecting a filter output (_origin_) to a filter input (_destination_).
template<
	std::size_t Output_dim,
	typename Output_elem,
	std::size_t Input_dim,
	typename Input_elem,
	typename Caster
>
class filter_edge final :
	public filter_edge_output_base<Output_dim, Output_elem>,
	public filter_edge_input_base<Input_dim, Input_elem>
{
	using base_in = filter_edge_input_base<Input_dim, Input_elem>;
	using base_out = filter_edge_output_base<Output_dim, Output_elem>;

	/*
	origin filter
	 [output]
		|
		|  cast
		v
	 [input]
	destination filter
	*/

public:
	using typename base_out::output_type;
	using typename base_out::output_frame_shape_type;
	using typename base_out::output_full_view_type;
	using typename base_in::input_type;
	using typename base_in::input_frame_shape_type;
	using typename base_in::input_full_view_type;
	using caster_type = Caster;

private:
	caster_type caster_;
	
public:
	filter_edge(output_type& out, input_type& in, const caster_type& caster) :
		base_out(out), base_in(in), caster_(caster) { }

	input_full_view_type input_view_from_opaque(const const_data_window_view_type& out_opaque_vw) const override {
		output_full_view_type out_vw = from_opaque<Output_dim + 1, const Output_elem>(out_opaque_vw);
		return caster_.cast_view(out_vw);
	}

	input_frame_shape_type input_frame_shape() const override {
		output_frame_shape_type output_shp = base_out::origin().frame_shape();
		return caster_.cast_frame_shape(output_shp);
	}
};


template<std::size_t Output_dim, typename Output_elem, std::size_t Input_dim, typename Input_elem, typename Caster>
void connect_filters(
	filter_output<Output_dim, Output_elem>&,
	filter_input<Input_dim, Input_elem>&,
	const Caster&
);

template<std::size_t Dim, typename Elem>
void connect_filters(filter_output<Dim, Elem>&, filter_input<Dim, Elem>&);

template<std::size_t Output_dim, typename Output_elem, std::size_t Input_dim, typename Input_elem>
void disconnect_filters(
	filter_output<Output_dim, Output_elem>&,
	filter_input<Input_dim, Input_elem>&
);

}

#include "filter_edge.tcc"

#endif