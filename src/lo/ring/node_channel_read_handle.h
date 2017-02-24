#ifndef TFF_NODE_CHANNEL_READ_HANDLE_H_
#define TFF_NODE_CHANNEL_READ_HANDLE_H_

#include "node_read_handle.h"
#include "../types.h"

namespace tff {

class node_channel_read_handle : public node_read_handle {
private:
	channel_index_type ndarray_channel_index_;
	
public:
	node_channel_read_handle(rqueue_type::read_handle&&, time_unit start_t, channel_index_type);

	input_ndarray_window_view_type view() const;
};

}

#endif