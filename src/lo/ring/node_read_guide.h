#ifndef TFF_NODE_READ_GUIDE_H_
#define TFF_NODE_READ_GUIDE_H_

#include "../../common.h"

namespace tff {

/// Specifies which part of a node's output frame a \ref node_read_handle or a \ref node_output refers to.
class node_read_guide {
private:
	enum { pull_only_, data_ } kind_;
	union {
		data_channel_index_type data_channel_index_;
	};
	
	node_read_guide() = default;
	
public:
	static node_read_guide pull_only() {
		node_read_guide guide;
		guide.kind_ = pull_only_;
		return guide;
	}
	static node_read_guide data(data_channel_index_type chan_idx) {
		node_read_guide guide;
		guide.kind_ = data_;
		guide.data_channel_index_ = chan_idx;
		return guide;
	}
	
	node_read_guide(const node_read_guide&) = default;
	node_read_guide& operator=(const node_read_guide&) = default;
	
	bool is_pull_only() const { return (kind_ == pull_only_); }
	
	bool has_data_channel() const { return (kind_ == data_); }
	data_channel_index_type data_channel() const {
		Assert(has_data_channel());
		return data_channel_index_;
	}
};

};

#endif