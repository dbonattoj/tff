#include "sink_node.h"
#include "../node_graph.h"
#include "../../rqueue/rqueue.h"

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


auto sink_node::process_next() -> process_result {

}


void sink_node::seek(time_unit t) {

}

}
