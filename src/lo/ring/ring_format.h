#ifndef TFF_RING_FORMAT_H_
#define TFF_RING_FORMAT_H_

#include "../types.h"
#include "../../nd.h"
#include <vector>

namespace tff {

class ring_format {
private:
	std::vector<opaque_ndarray_format> ndarray_channel_formats_;
	//std::vector<opaque_ndarray_format> parameter_channels_;
	
public:
	const opaque_ndarray_format& ndarray(channel_index_type i) const { return ndarray_channel_formats_.at(i); }
	std::size_t ndarrays_count() const { return ndarray_channel_formats_.size(); }

	std::ptrdiff_t add_ndarray(const opaque_ndarray_format& frm) {
		ndarray_channel_formats_.push_back(frm);
		return ndarray_channel_formats_.size();
	}
};

}

#endif