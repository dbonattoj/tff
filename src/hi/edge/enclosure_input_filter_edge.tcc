#include "enclosure_input_filter_edge.h"

namespace tff {

template<std::size_t Input_dim, typename Input_elem>
void connect_enclosure_input(
	enclosure_filter_input<Input_dim, Input_elem>& external,
	filter_input<Input_dim, Input_elem>& internal
) {
	using edge_type = enclosure_input_filter_edge<Input_dim, Input_elem>;
	auto edge_ptr = std::make_unique<edge_type>(external, input);
	edge_type& edge = *edge_ptr;
	internal.set_edge(std::move(edge));
	external.internal_input_has_connected(edge);
}

}