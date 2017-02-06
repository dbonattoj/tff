#ifndef TFF_NODE_READ_HANDLE_H_
#define TFF_NODE_READ_HANDLE_H_

#include "../../rqueue/rqueue.h"
#include "../types.h"
#include "ring.h"

namespace tff {

class node_read_handle {
public:
	using ndarray_view_type = ndarray_timed_wraparound_opaque_view<1, false, ndarray_opaque_format_type>;
	
private:
	rqueue_read_handle handle_;
	time_unit start_time_;
	channel_index_type ndarray_channel_index_;
	
	node_read_handle(const node_read_handle&) = delete;
	node_read_handle& operator=(const node_read_handle&) = delete;

public:
	node_read_handle(rqueue_read_handle&&, time_unit, channel_index_type);
	
	node_read_handle(node_read_handle&&) = default;
	node_read_handle& operator=(node_read_handle&&) = default;
	
	ndarray_view_type view() const {
		return timed(handle_.view().ndarray(ndarray_channel_index_), start_time_);
	}
};

}

#endif