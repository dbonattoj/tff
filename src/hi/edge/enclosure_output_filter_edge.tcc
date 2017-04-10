#include "enclosure_output_filter_edge.h"

namespace tff {

template<std::size_t Output_dim, typename Output_elem>
void connect_enclosure_output(
	enclosure_filter_output<Output_dim, Output_elem>& external,
	filter_output<Output_dim, Output_elem>& internal
) {
	using edge_type = enclosure_output_filter_edge<Output_dim, Output_elem>;
	auto edge_ptr = std::make_unique<edge_type>(external, input);
	edge_type& edge = *edge_ptr;
	internal.set_edge(std::move(edge));
	external.internal_input_has_connected(edge);
}

}