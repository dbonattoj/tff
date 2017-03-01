#ifndef TFF_RING_FORMAT_H_
#define TFF_RING_FORMAT_H_

#include "../types.h"
#include "../../nd/opaque_format/ndarray.h"
#include <vector>

namespace tff {

class ring_format {
private:
	std::vector<opaque_ndarray_format> data_channel_formats_;
	
public:
	const opaque_ndarray_format& data_channel(data_channel_index_type i) const
		{ return data_channel_formats_.at(i); }
	std::size_t data_channels_count() const
		{ return data_channel_formats_.size(); }

	data_channel_index_type add_data_channel(const opaque_ndarray_format& frm) {
		data_channel_formats_.push_back(frm);
		return data_channel_formats_.size() - 1;
	}
};

}

#endif