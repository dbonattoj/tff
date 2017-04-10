#include "filter_edge.h"
#include "filter_edge_caster.h"
#include "../filter_output.h"
#include "../filter_input.h"

namespace tff {

template<
	std::size_t Output_dim,
	typename Output_elem,
	std::size_t Input_dim,
	typename Input_elem,
	typename Caster
>
void connect_filters(
	filter_output<Output_dim, Output_elem>& out,
	filter_input<Input_dim, Input_elem>& in,
	const Caster& caster
) {
	Assert(! in.is_connected(), "filter input must not already be connected");

	if(&out.this_filter().graph() != &in.this_filter().graph())
		throw invalid_filter_graph("filter edge must be between filters in same subgraph");

	if(succeedes(out.this_filter(), in.this_filter()))
		throw invalid_filter_graph("filter edge origin filter must not succeede destination filter");


	using edge_type = filter_edge<Output_dim, Output_elem, Input_dim, Input_elem, Caster>;
	auto edge_ptr = std::make_unique<edge_type>(out, in, caster);
	edge_type& edge = *edge_ptr;
	in.set_edge(std::move(edge_ptr));
	out.edge_has_connected(edge);
}


template<std::size_t Dim, typename Elem>
void connect_filters(
	filter_output<Dim, Elem>& out,
	filter_input<Dim, Elem>& in
) {
	using caster_type = filter_null_edge_caster<Dim, Elem>;
	connect_filters(out, in, caster_type());
}


template<std::size_t Output_dim, typename Output_elem, std::size_t Input_dim, typename Input_elem>
void disconnect_filters(
	filter_output<Output_dim, Output_elem>& out,
	filter_input<Input_dim, Input_elem>& in
) {
	Assert(&in.edge().origin() == &out, "filter input must be connected to the given output");
	out.edge_has_disconnected(in.edge());
	in.delete_edge();
}

};