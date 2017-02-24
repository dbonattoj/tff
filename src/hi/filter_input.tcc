#include "filter_edge_caster.h"

namespace tff {

template<std::size_t Input_dim, typename Input_elem>
bool filter_input<Input_dim, Input_elem>::is_connected() const {
	return (edge_.get() != nullptr);
}


template<std::size_t Input_dim, typename Input_elem>
const filter_edge_base& filter_input<Input_dim, Input_elem>::edge() const {
	return *edge_;
}


template<std::size_t Input_dim, typename Input_elem>
auto filter_input<Input_dim, Input_elem>::frame_shape() const -> const frame_shape_type& {
	Assert(is_connected());
	Assert(connected_output().frame_shape_is_defined());
	return edge_->input_frame_shape();
}

	
template<std::size_t Input_dim, typename Input_elem>
void filter_input<Input_dim, Input_elem>::connect(filter_output<size_t, Input_elem>& out) {
	using caster_type = filter_null_edge_caster<Input_dim, Input_elem>;
	using edge_type = filter_edge<Input_dim, Input_elem, Input_dim, Input_elem, caster_type>;
	edge_ = std::make_unique<edge_type>(*this, out);
	out.edge_has_connected(*edge_);
}


};
