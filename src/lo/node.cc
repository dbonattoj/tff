#include "node.h"

namespace tff {


node_input& node::add_input_() {
	inputs_.emplace_back(*this, inputs_.size());
	return inputs_.back();
}


node_output& node::add_output_() {
	outputs_.emplace_back(*this, outputs_.size());
	return outputs_.back();
}


void node::request(time_span span) {
	if(span.begin < 0) span.begin = 0;
	for(const requestee& req : requestees_)
		req.preceding_node.request(time_span(span.begin - req.past_window, span.end + req.future_window));
}


void node::launch() {
	for(const requestee& req : requestees_) req.preceding_node.launch();
}


void node::stop() {
	for(const requestee& req : requestees_) req.preceding_node.stop();
}


//////////


node_read_handle node_input::read_frame(time_unit t) {
	time_span span(t - past_window_, t + 1 + future_window_);
	return connected_output_->read(span);
}


}