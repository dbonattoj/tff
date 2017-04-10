#include "enclosure_filter.h"
#include "../edge/enclosure_input_filter_edge.h"
#include "../edge/enclosure_output_filter_edge.h"
#include <utility>

namespace tff {

template<typename Box> template<typename... Box_args>
enclosure_filter<Box>::enclosure_filter(filter_subgraph& gr, Box_args&&... args) : enclosure_filter_base(gr) {
	current_in_construction_ = this;
	box_.emplace(std::forward<Box_args>(args)...);
	current_in_construction_ = nullptr;
}


//////////


template<std::size_t Input_dim, typename Input_elem>
enclosure_filter_input<Input_dim, Input_elem>::~enclosure_filter_input() { }


template<std::size_t Input_dim, typename Input_elem>
void enclosure_filter_input<Input_dim, Input_elem>::internal_edge_has_connected(internal_edge_type& edge) {
	internal_edges_.push_back(edge);
}


template<std::size_t Input_dim, typename Input_elem>
void enclosure_filter_input<Input_dim, Input_elem>::internal_edge_has_disconnected(internal_edge_type& edge) {
	auto cmp = [&edge](const internal_edge_type& q_edge) { return (&edge == &q_edge); };
	auto it = std::find_if(internal_edges_.begin(), internal_edges_.end(), cmp);
	Assert(it != internal_edges_.end());
	internal_edges_.erase(it);
}


//////////


template<std::size_t Output_dim, typename Output_elem>
enclosure_filter_output<Output_dim, Output_elem>::~enclosure_filter_output() { }


template<std::size_t Output_dim, typename Output_elem>
bool enclosure_filter_output<Output_dim, Output_elem>::frame_shape_is_defined() const {
	Assert(has_internal_edge());
	return internal_edge().origin().frame_shape_is_defined();
}


template<std::size_t Output_dim, typename Output_elem>
auto enclosure_filter_output<Output_dim, Output_elem>::frame_shape() const -> const frame_shape_type& {
	Assert(has_internal_edge());
	Assert(frame_shape_is_defined());
	return internal_edge().origin().frame_shape();
}


template<std::size_t Output_dim, typename Output_elem>
void enclosure_filter_output<Output_dim, Output_elem>::set_internal_edge(std::unique_ptr<internal_edge_type> edge) {
	internal_edge_ = std::move(edge);
}


template<std::size_t Output_dim, typename Output_elem>
void enclosure_filter_output<Output_dim, Output_elem>::delete_internal_edge() {
	internal_edge_.reset();
}


}

