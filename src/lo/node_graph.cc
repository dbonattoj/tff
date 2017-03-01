#include "node_graph.h"
#include "sink_node.h"

namespace tff {

node_graph::node_graph() {
	sink_node& sink_nd = add_node<sink_node>();
	sink_ = &sink_nd;
}

node_graph::~node_graph() {
	if(was_setup_) stop();
}
		

void node_graph::setup() {
	Assert(! was_setup_);
	Assert(sink_ != nullptr);
	sink_->setup_graph();
	was_setup_ = true;
}


void node_graph::launch() {
	Assert(was_setup_);

	if(launched_) return;
	sink_->launch();
	launched_ = true;
}


void node_graph::stop() {
	Assert(was_setup_);
	
	if(! launched_) return;
	sink_->stop();
	launched_ = false;
}


time_unit node_graph::current_time() const {
	Assert(was_setup_);
	return sink_->current_time();
}


void node_graph::run_until(time_unit last_frame) {
	Assert(was_setup_);
	if(last_frame < next_run_start_time_) return;
	launch();
	frame_state_flag state = sink_->process(next_run_start_time_);
	while(state == frame_state_flag::success && current_time() < last_frame)
		state = sink_->process_next();
}


void node_graph::run_for(time_unit duration) {
	Assert(was_setup_);
	run_until(current_time() + duration);
}


void node_graph::run() {
	Assert(was_setup_);
	launch();
	frame_state_flag state = sink_->process(next_run_start_time_);
	while(state == frame_state_flag::success)
		state = sink_->process_next();
}


void node_graph::seek(time_unit t) {
	Assert(was_setup_);
	next_run_start_time_ = t;
}


}