#include "enclosure_filter.h"
#include "hi/edge/enclosure_input_filter_edge.h"
#include "hi/edge/enclosure_output_filter_edge.h"
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
void enclosure_filter_input<Input_dim, Input_elem>::internal_input_has_connected(internal_input_type& in) {
	if(&in.this_filter().graph() != &this_filter().subgraph())
		throw invalid_filter_graph("internal filter input in enclosure can only be attached to external input of same enclosure");
	
	using edge_type = enclosure_input_filter_edge<Input_dim, Input_elem>;
	auto edge = std::make_unique<edge_type>(*this, in);
	internal_edges_.push_back(*edge);
	in.set_edge(std::move(edge));
}


template<std::size_t Input_dim, typename Input_elem>
void enclosure_filter_input<Input_dim, Input_elem>::internal_input_has_disconnected(internal_input_type& in) {
	Assert(in.is_connected());
	auto cmp = [&in](const internal_edge_type& edge) { return (&edge == &in.edge()); };
	auto it = std::find_if(internal_edges_.begin(), internal_edges_.end(), cmp);
	Assert(it != internal_edges_.end());
	
	internal_edges_.erase(it);
	in.delete_edge();
}


//////////


template<std::size_t Output_dim, typename Output_elem>
enclosure_filter_output<Output_dim, Output_elem>::~enclosure_filter_output() { }


template<std::size_t Output_dim, typename Output_elem>
bool enclosure_filter_output<Output_dim, Output_elem>::frame_shape_is_defined() const {
	Assert(has_internal_edge());
	return internal_output().frame_shape_is_defined();
}


template<std::size_t Output_dim, typename Output_elem>
auto enclosure_filter_output<Output_dim, Output_elem>::frame_shape() const -> const frame_shape_type& {
	Assert(has_internal_edge());
	Assert(frame_shape_is_defined());
	return internal_output().frame_shape();
}


template<std::size_t Output_dim, typename Output_elem>
void enclosure_filter_output<Output_dim, Output_elem>::set_internal_edge(std::unique_ptr<internal_edge_type> edge) {
	internal_edge_ = std::move(edge);
}


template<std::size_t Output_dim, typename Output_elem>
void enclosure_filter_output<Output_dim, Output_elem>::delete_internal_edge() {
	internal_edge_.reset();
}



template<std::size_t Output_dim, typename Output_elem>
auto enclosure_filter_output<Output_dim, Output_elem>::internal_output() -> internal_output_type& {
	Assert(has_internal_edge());
	return internal_edge_->origin();
}


template<std::size_t Output_dim, typename Output_elem>
auto enclosure_filter_output<Output_dim, Output_elem>::internal_output() const -> const internal_output_type& {
	Assert(has_internal_edge());
	return internal_edge_->origin();
}


/*
template<std::size_t Output_dim, typename Output_elem>
void enclosure_filter_output<Output_dim, Output_elem>::attach_internal_output(internal_output_type& out) {
	if(&out.this_filter().graph() != &this_filter().subgraph())
		throw invalid_filter_graph("internal filter output in enclosure can only be attached to external output of same enclosure");
	
	internal_edge_ = std::make_unique<internal_edge_type>(*this, out);
	out.edge_has_connected(*internal_edge_);
}


template<std::size_t Output_dim, typename Output_elem>
void enclosure_filter_output<Output_dim, Output_elem>::detach_internal_output() {
	internal_output().edge_has_disconnected(*internal_edge_);
	internal_edge_.reset();
}
*/

}

