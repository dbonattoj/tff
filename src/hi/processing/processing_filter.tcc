#include "../../lo/processing/processing_node.h"
#include "../../lo/processing/sync_node.h"
#include "../../lo/processing/async_node.h"
#include "../filter_installation_guide.h"

namespace tff {


template<typename Box>
void processing_filter<Box>::processing_filter() {
	current_in_construction_ = this;
	box_.emplace();
	current_in_construction_ = nullptr;
}


template<typename Box>
void processing_filter<Box>::install_(filter_installation_guide& guide) {
	// setup filter box
	box_->setup();
	for(const filter_output_base& out : outputs())
		if(! out.frame_shape_is_defined())
			throw filter_box_error("filter box did not define frame shape for all connected outputs");

	
	node_graph& nd_graph = guide.this_node_graph();
	
	// create processing node
	processing_node* installed_node = nullptr;
	if(asynchronous_) {
		async_node& nd = nd_graph.add_node<async_node>(node_name);
		nd.set_prefetch_duration(prefetch_duration_);
		installed_node = &nd;
	} else {
		sync_node& nd = nd_graph.add_node<sync_node>(node_name);
		installed_node = &nd;
	}
	guide.set_processing_filter_node(*this, *nd);
	
	// attach processing handler
	nd.set_handler(*this);
	
	// add inputs & connect
	for(const filter_input_base& in : inputs()) {
		if(! in.is_connected()) continue;
		node_input& nd_in = nd->add_input();
		
		nd_in.set_window(in.window());
		
		filter_edge_base& edge = in.edge();
		node_output& nd_out = guide.edge_node_output(edge);
		nd_in.connect(nd_out);
	}
	
	// add channel for each output
	for(const filter_output& out : outputs()) {
		opaque_ndarray_format frame_format = out.ndarray_format();
		channel_index_type chan_idx = nd->add_channel(frame_format);
		
		// add output for each edge (for each output)
		std::size_t edges_count = out.edges_count();
		for(std::ptrdiff_t i = 0; i < edges_count; ++i) {
			const filter_edge_base& edge = out.edge_at(i);
			node_output& nd_out = nd->add_output(chan_idx);
			
			guide.set_edge_node_output(edge, nd_out);
		}
	}
}


}