#include "filter_edge.h"
#include "../nd/nd.h"

namespace tff {


template<std::size_t Output_dim, typename Output_elem>
void filter_output<Output_dim, Output_elem>::edge_has_connected(edge_base_type& edge) {
	edge_base_type.push_back(edge);
}


template<std::size_t Output_dim, typename Output_elem>
std::size_t filter_output<Output_dim, Output_elem>::edges_count() const {
	return edges_.size();
}


template<std::size_t Output_dim, typename Output_elem>
const filter_edge_base& filter_output<Output_dim, Output_elem>::edge_at(std::ptrdiff_t i) const {
	return edges_.at(i);
}


template<std::size_t Output_dim, typename Output_elem>
void filter_output<Output_dim, Output_elem>::define_frame_shape(const frame_shape_type& shp) {
	frame_shape_ = shp;
}


template<std::size_t Output_dim, typename Output_elem>
bool filter_output<Output_dim, Output_elem>::frame_shape_is_defined() const {
	return frame_shape_.has_value();
}


template<std::size_t Output_dim, typename Output_elem>
auto filter_output<Output_dim, Output_elem>::frame_shape() -> const frame_shape_type& {
	Assert(frame_shape_is_defined());
	return *frame_shape;
}


template<std::size_t Output_dim, typename Output_elem>
opaque_ndarray_format filter_output<Output_dim, Output_elem>::data_format() const {
	Assert(frame_shape_is_defined());
	return default_opaque_ndarray_format<Output_elem>(*frame_shape_);
}

}
