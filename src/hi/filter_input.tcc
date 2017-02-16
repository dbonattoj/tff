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
template<std::size_t Output_dim, typename Output_elem>
void filter_input<Input_dim, Input_elem>::connect(filter_output<Output_dim, Output_elem>& out) {
	static_assert(Input_dim == Output_dim, "input and output must have same dimension");
	using edge_type = filter_direct_edge<Output_dim, Output_elem, Input_elem>;
	edge_ = std::move(std::make_unique<edge_type>(*this, out));
	out.edge_has_connected(*edge_);
}


};