#include "processing_filter.h"

namespace tff {

thread_local processing_filter_base* processing_filter_base::current_in_construction_ = nullptr;


void processing_filter_base::handler_pre_process_(processing_job& nd_job) {
	processing_filter_job job(nd_job);
	return this->box_pre_process_(job);
}


void processing_filter_base::handler_process_(processing_job& nd_job) {
	processing_filter_job job(nd_job);
	return this->box_process_(job);
}


void processing_filter_base::setup_() {
	// setup processing box
	this->box_setup_();
	
	// verify that frame shapes for all outputs have been defined
	for(const filter_output_base& out : outputs())
		if(! out.frame_shape_is_defined())
			throw filter_box_error("filter box did not define frame shape for all connected outputs");
}


void processing_filter_base::install_(filter_installation_guide& guide) {
	node_graph& nd_graph = guide.this_node_graph();
	std::string node_name = guide.name_prefix() + name();
	
	bool need_asynchronous_node = asynchronous_;
	/*
	if(! need_asynchronous_node) {
		// check if asynchronous node is needed because successors are on different threads
		bool different_input_reader_threads = false;
		thread_index_type reader_thread = -1;
		for(filter_output_base& out : outputs()) {
			for(std::ptrdiff_t i = 0; i < out.edges_count(); ++i) {
				const filter_edge_base& edge = out.edge_at(i);
				if(! guide.has_filter_to_install(edge.destination_filter())) continue;
				Assert(guide.has_edge(edge), "node_input of filter successor must already be in guide");
				thread_index_type input_reader_thread = guide.edge_node_input(edge).reader_thread();
				if(reader_thread == -1) reader_thread = input_reader_thread;
				else if(reader_thread != input_reader_thread)
					different_input_reader_threads = true;
			}
		}
		if(is_pulled() && reader_thread != guide.this_node_graph().root_thread_index()) different_input_reader_threads = true;
		need_asynchronous_node = different_input_reader_threads;
	}
	*/
	// create processing node
	processing_node* installed_node = nullptr;
	if(need_asynchronous_node) {
		async_node& nd = nd_graph.add_node<async_node>(node_name);
		nd.set_prefetch_duration(prefetch_duration_);
		installed_node = &nd;
	} else {
		sync_node& nd = nd_graph.add_node<sync_node>(node_name);
		installed_node = &nd;
	}
	guide.set_filter_processing_node(*this, *installed_node);
	
	
	if(is_pulled()) {
		node_output& pull_output = installed_node->add_pull_only_output();
		node_input& pull_input = guide.this_node_graph().sink().add_input();
		pull_input.connect(pull_output);
	}
	
	// attach processing handler
	installed_node->set_handler(*this);

	// add inputs
	for(filter_input_base& in : inputs()) {
		if(! in.is_connected()) continue;
		node_input& nd_in = installed_node->add_input();
		
		Assert(in.input_index() == -1 || in.input_index() == nd_in.index(),
			"node_inputs for processing_filter_input must have same index");
		in.set_input_index(nd_in.index());
		
		nd_in.set_window(in.window());
		
		// put (edge, nd_in) into guide, so that predecessor filter's installed node can connect to it
		filter_edge_base& edge = in.edge();
		guide.add_successor_edge_node_input(edge, nd_in);
	}
	
	// add channel for each output
	for(filter_output_base& out : outputs()) {
		// add data channel for the output
		opaque_ndarray_format data_format = out.data_format();
		data_channel_index_type chan_idx = installed_node->add_data_channel(data_format);

		Assert(out.data_channel_index() == -1 || out.data_channel_index() == chan_idx,
			"data channels for processing_filter_output must have same index");
		out.set_data_channel_index(chan_idx);
		
		// add output for each edge (for each output)
		for(std::ptrdiff_t i = 0; i < out.edges_count(); ++i) {
			const filter_edge_base& edge = out.edge_at(i);
			for(node_input& nd_in : guide.successor_edge_node_inputs(edge)) {
				node_output& nd_out = installed_node->add_data_output(chan_idx);
				nd_in.connect(nd_out);
			}
		}
	}
}



}