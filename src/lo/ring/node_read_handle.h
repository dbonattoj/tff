#ifndef TFF_NODE_READ_HANDLE_H_
#define TFF_NODE_READ_HANDLE_H_

#include "../../rqueue/rqueue.h"
#include "../types.h"
#include "ring.h"
#include <utility>

namespace tff {

/// Handle to segment of frames being read from a \ref node_output.
/** Object is an RAII resource lock: The segment remains locked from being overwritten in the node's ring for as
 ** long as the read handle exists. Wrapper of the rqueue's read handle. */
class node_read_handle {
private:
	rqueue_type::read_handle handle_;
	time_unit start_time_;
	channel_index_type ndarray_channel_index_;
	
	node_read_handle(const node_read_handle&) = delete;
	node_read_handle& operator=(const node_read_handle&) = delete;

public:
	node_read_handle(rqueue_type::read_handle&&, time_unit start_t, channel_index_type);
	
	node_read_handle(node_read_handle&&) = default;
	node_read_handle& operator=(node_read_handle&&) = default;
	
	bool valid() const;
	input_metadata_window_view_type metadata() const;
	input_ndarray_window_view_type view() const;
};

}

#endif