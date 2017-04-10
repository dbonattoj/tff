#include "filter_edge.h"
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
	using edge_type = filter_edge<Output_dim, Output_elem, Input_dim, Input_elem, Caster>;
	auto edge_ptr = std::make_unique<edge_type>(out, in, caster);
	filter_edge_type& edge = *edge_ptr;
	in.set_edge(std::move(edge));
	out.edge_has_connected(edge);
}

}