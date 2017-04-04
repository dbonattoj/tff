#include "filter_input.h"
#include "filter_edge_caster.h"
#include "filter_edge.h"

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
	Assert(edge().origin().frame_shape_is_defined());
	return edge().input_frame_shape();
}


template<std::size_t Input_dim, typename Input_elem>
void filter_input<Input_dim, Input_elem>::set_edge_(std::unique_ptr<edge_base_type> edge) {
	edge_ = std::move(edge);
}


template<std::size_t Input_dim, typename Input_elem>
void filter_input<Input_dim, Input_elem>::delete_edge_() {
	edge_.reset();
}


};
