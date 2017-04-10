#include "enclosure_output_filter_edge.h"

namespace tff {

template<std::size_t Output_dim, typename Output_elem>
void connect_enclosure_output(
	filter_output<Output_dim, Output_elem>& internal,
	enclosure_filter_output<Output_dim, Output_elem>& external
) {
	using edge_type = enclosure_output_filter_edge<Output_dim, Output_elem>;
	auto edge_ptr = std::make_unique<edge_type>(external, internal);
	edge_type& edge = *edge_ptr;
	external.set_internal_edge(std::move(edge_ptr));
	internal.edge_has_connected(edge);
}

template<std::size_t Output_dim, typename Output_elem>
void disconnect_enclosure_output(
	filter_output<Output_dim, Output_elem>& internal,
	enclosure_filter_output<Output_dim, Output_elem>& external
) {
	internal.edge_has_disconnected(external.internal_edge());
	external.delete_internal_edge();
}

};