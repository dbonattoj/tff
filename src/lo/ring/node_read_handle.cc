#include "node_read_handle.h"
#include <utility>

namespace tff {

node_read_handle::node_read_handle(rqueue_type::read_handle&& handle, const node_read_guide& guide) :
	handle_(std::move(handle_)), guide_(guide) { }


bool node_read_handle::valid() const {
	return handle_.valid();
}


node_read_guide::kind node_read_handle::kind() const {
	return guide_.kind;
}


const_state_window_view_type node_read_handle::state() const {
	return handle_.view().state();
}


const_data_window_view_type node_read_handle::data() const {
	Assert(guide_.kind == node_read_guide::data);
	return handle_.view().ndarray(guide_.data_channel_index);
}


}