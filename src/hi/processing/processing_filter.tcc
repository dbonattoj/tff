#include "../../lo/processing/processing_node.h"
#include "../../lo/processing/sync_node.h"
#include "../../lo/processing/async_node.h"
#include "../filter_installation_guide.h"
#include <utility>

namespace tff {


template<typename Box> template<typename... Box_args>
void processing_filter<Box>::processing_filter(Box_args&&... args) {
	// filter_box (base class of concrete filter implemented by application), fetches current_in_construction_ in its
	// constructor. that way the concrete filter class doesn't need this processing_filter as constructor argument
	current_in_construction_ = this;
	box_.emplace(std::forward<Box_args>(args)...);
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
	guide.set_processing_filter_node(*this, *installed_node);
	
	// attach processing handler
	installed_node->set_handler(*this);
	
	// add inputs & connect
	for(const filter_input_base& in : inputs()) {
		if(! in.is_connected()) continue;
		node_input& nd_in = installed_node->add_input();
		
		Assert(in.input_index() != -1 || in.input_index() == nd_in.index(),
			"node_inputs for processing_filter_input must have same index");
		in.set_input_index(nd_in.index());
		
		nd_in.set_window(in.window());
		
		filter_edge_base& edge = in.edge();
		Assert(guide.has_edge(edge), "edge of processing_filter's input already must be guide");
		node_output& nd_out = guide.edge_node_output(edge);
		nd_in.connect(nd_out);
	}
	
	// add channel for each output
	for(const filter_output& out : outputs()) {
		// add data channel for the output
		opaque_ndarray_format data_format = out.data_format();
		data_channel_index_type chan_idx = installed_node->add_data_channel(data_format);

		Assert(out.data_channel_index() != -1 || out.data_channel_index() == chan_idx,
			"data channels for processing_filter_output must have same index");
		out.set_data_channel_index(chan_idx);
		
		// add output for each edge (for each output)
		std::size_t edges_count = out.edges_count();
		for(std::ptrdiff_t i = 0; i < edges_count; ++i) {
			const filter_edge_base& edge = out.edge_at(i);
			node_output& nd_out = nd->add_data_output(chan_idx);
			
			// put (edge, nd_out) into guide, so that successor filter's installed node can connect to it
			guide.set_edge_node_output(edge, nd_out);
		}
	}
	
	// if this is sink filter: add pull-only node output to connect to the node_graph's sink_node
	if(is_sink()) {
		node_output& pull_nd_out = nd->add_pull_only_output();
		guide.add_sink_pull_node_output(pull_nd_out);
	}
}


template<typename Box>
void processing_filter<Box>::box_pre_process_(processing_filter_job& job) {
	box_->pre_process(job);
}


template<typename Box>
void processing_filter<Box>::box_process_(processing_filter_job& job) {
	box_->process(job);
}


}