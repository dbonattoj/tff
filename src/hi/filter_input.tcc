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
void filter_input<Input_dim, Input_elem>::connect(filter_output<Input_dim, Input_elem>& out) {
	using caster_type = filter_null_edge_caster<Input_dim, Input_elem>;
	using edge_type = filter_edge<Input_dim, Input_elem, Input_dim, Input_elem, caster_type>;
	std::unique_ptr<edge_type> edge = std::make_unique<edge_type>(out, *this);
	out.edge_has_connected(*edge);
	edge_ = std::move(edge);
}


};
