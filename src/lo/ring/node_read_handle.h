#ifndef TFF_NODE_READ_HANDLE_H_
#define TFF_NODE_READ_HANDLE_H_

#include "../../rqueue/rqueue.h"
#include "../types.h"
#include "ring.h"
#include "node_read_guide.h"

namespace tff {

/// Handle to segment of frames being read from a \ref node_output.
/** Object is an RAII resource lock: The segment remains locked from being overwritten in the node's ring for as
 ** long as the read handle exists. Wrapper of the rqueue's read handle. */
class node_read_handle {
private:
	rqueue_type::read_handle handle_;
	node_read_guide guide_;
	
	node_read_handle(const node_read_handle&) = delete;
	node_read_handle& operator=(const node_read_handle&) = delete;

public:
	explicit node_read_handle(rqueue_type::read_handle&& handle, const node_read_guide& guide);
	
	node_read_handle(node_read_handle&&) = default;
	node_read_handle& operator=(node_read_handle&&) = default;
	
	bool valid() const;
	const_state_window_view_type state() const;
	const_data_window_view_type data() const;
};

}

#endif