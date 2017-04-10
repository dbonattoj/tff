#include "filter_input.h"
#include "edge/filter_edge_caster.h"
#include "edge/filter_edge_base.h"

namespace tff {

template<std::size_t Input_dim, typename Input_elem>
bool filter_input<Input_dim, Input_elem>::is_connected() const {
	return (edge_.get() != nullptr);
}


template<std::size_t Input_dim, typename Input_elem>
auto filter_input<Input_dim, Input_elem>::edge() const -> const edge_base_type& {
	return *edge_;
}
template<std::size_t Input_dim, typename Input_elem>
auto filter_input<Input_dim, Input_elem>::edge() -> edge_base_type& {
	return *edge_;
}


template<std::size_t Input_dim, typename Input_elem>
auto filter_input<Input_dim, Input_elem>::frame_shape() const -> frame_shape_type {
	Assert(is_connected());
	return edge().input_frame_shape();
}


template<std::size_t Input_dim, typename Input_elem>
void filter_input<Input_dim, Input_elem>::set_edge(std::unique_ptr<edge_base_type> edge) {
	edge_ = std::move(edge);
}


template<std::size_t Input_dim, typename Input_elem>
void filter_input<Input_dim, Input_elem>::delete_edge() {
	edge_.reset();
}


};
