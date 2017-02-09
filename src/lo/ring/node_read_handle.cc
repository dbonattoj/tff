#include "node_read_handle.h"

namespace tff {

node_read_handle::node_read_handle(rqueue_type::read_handle&& handle, time_unit start_t, channel_index_type chan) :
	handle_(std::move(handle)),
	start_time_(start_t),
	ndarray_channel_index_(chan) { }
		

bool node_read_handle::valid() const {
	return handle_.valid();
}


input_metadata_window_view_type node_read_handle::metadata() const {
	Assert(valid());
	return timed(handle_.view().metadata(), start_time_);
}


input_ndarray_window_view_type node_read_handle::view() const {
	Assert(valid());
	return timed(handle_.view().ndarray(ndarray_channel_index_), start_time_);
}


}
