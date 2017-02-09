#include "sink_node.h"
#include "../node_graph.h"
#include "../ring/ring.h"
#include "../ring/frame_metadata.h"
#include "../../rqueue/rqueue.h"

namespace tff {

sink_node::sink_node(node_graph& gr, const std::string& name) :
	processing_node(gr, name) { }


void sink_node::setup() {
	processing_node::setup();
	processing_node::setup_ring_(rqueue_variant::sync, 1);
	rqueue_().set_sync_writer(std::bind(&sink_node::write_, this, std::placeholders::_1));
}


void sink_node::write_(rqueue_type::write_handle& handle) {
	if(handle.has_stopped()) return;
	processing_node::write_next_(handle);
}


void sink_node::setup_graph() {
	node::sink_setup_();
}


thread_index_type sink_node::input_reader_thread(input_index_type) const {
	return graph().root_thread_index();
}


frame_state sink_node::process(time_unit t) {
	time_span span(t, t + 1);
	request(span);
	rqueue_type::read_handle handle = rqueue_().read(span);
	Assert(handle.valid());
	return handle.view().metadata().at(0).state;
}


frame_state sink_node::process_next() {
	return process(current_time() + 1);
}

}
