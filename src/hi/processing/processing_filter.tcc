#include "../../lo/processing/processing_node.h"
#include "../../lo/processing/sync_node.h"
#include "../../lo/processing/async_node.h"
#include "../../lo/sink_node.h"
#include "../../lo/node_graph.h"
#include "../../lo/node_input.h"
#include "../../lo/node_output.h"
#include "../filter_installation_guide.h"
#include "../filter_output.h"
#include <utility>
#include <iostream>

namespace tff {


template<typename Box> template<typename... Box_args>
processing_filter<Box>::processing_filter(filter_subgraph& gr, Box_args&&... args) : processing_filter_base(gr) {
	// filter_box (base class of concrete filter implemented by application), fetches current_in_construction_ in its
	// constructor. that way the concrete filter class doesn't need this processing_filter as constructor argument
	current_in_construction_ = this;
	box_.emplace(std::forward<Box_args>(args)...);
	current_in_construction_ = nullptr;
}


template<typename Box>
void processing_filter<Box>::box_pre_process_(processing_filter_job& job) {
	box_->pre_process(job);
}


template<typename Box>
void processing_filter<Box>::box_process_(processing_filter_job& job) {
	box_->process(job);
}


template<typename Box>
void processing_filter<Box>::box_setup_() {
	box_->setup();
}


/////


template<std::size_t Input_dim, typename Input_elem>
void processing_filter_input<Input_dim, Input_elem>::connect(filter_output<Input_dim, Input_elem>& out) {
	using caster_type = filter_null_edge_caster<Input_dim, Input_elem>;
	using edge_type = filter_edge<Input_dim, Input_elem, Input_dim, Input_elem, caster_type>;
	base::set_edge_( std::make_unique<edge_type>(out, *this) );
	out.edge_has_connected(base::edge());
}


template<std::size_t Input_dim, typename Input_elem>
void processing_filter_input<Input_dim, Input_elem>::disconnect() {
	auto edge = base::edge();
	edge.origin().edge_has_disconnected(edge);
	base::delete_edge();
}


/////


template<std::size_t Output_dim, typename Output_elem>
void processing_filter_output<Output_dim, Output_elem>::define_frame_shape(const frame_shape_type& shp) {
	frame_shape_ = shp;
}


template<std::size_t Output_dim, typename Output_elem>
bool processing_filter_output<Output_dim, Output_elem>::frame_shape_is_defined() const {
	return frame_shape_.has_value();
}


template<std::size_t Output_dim, typename Output_elem>
auto processing_filter_output<Output_dim, Output_elem>::frame_shape() const -> const frame_shape_type& {
	Assert(frame_shape_is_defined());
	return *frame_shape_;
}


template<std::size_t Output_dim, typename Output_elem>
opaque_ndarray_format processing_filter_output<Output_dim, Output_elem>::data_format() const {
	Assert(frame_shape_is_defined());
	return default_opaque_ndarray_format<Output_elem>(*frame_shape_);
}



}