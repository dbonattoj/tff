#ifndef TFF_SINK_NODE_H_
#define TFF_SINK_NODE_H_

namespace tff {

sink_node::sink_node(node_graph& gr, const std::string& name) :
	processing_node(gr, name) { }


void sink_node::setup() {
	processing_node::setup();
	processing_node::setup_ring_(rqueue_variant::sync, 1);
}


void sink_node::setup_graph() {
	node::sink_setup_();
}


thread_index_type sink_node::input_reader_thread(input_index_type) const {
	return graph().root_thread_index();
}


auto sink_node::pull_next() -> pull_result {

}


auto sink_node::pull(time_unit t) -> pull_result {

}


void sink_node::seek(time_unit t) {

}


}

#endif