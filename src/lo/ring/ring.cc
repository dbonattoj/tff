#include "ring.h"

namespace tff {

ring::ring(std::size_t capacity, const ring_format& format) :
	capacity_(capacity),
	metadata_channel_(make_ndsize(capacity))
{
	ndarray_channels_.reserve(format.ndarrays_count());
	for(std::ptrdiff_t i = 0; i < format.ndarrays_count(); ++i) {
		propagated_opaque_format_type frame_format(format.ndarray(i));
		ndarray_channels_.emplace_back(make_ndsize(capacity), frame_format);
	}
}
	
auto ring::operator[](index_type idx) -> frame_type {
	return frame_type(*this, idx);
}

auto ring::operator[](index_type idx) const -> const_frame_type {
	return const_frame_type(*this, idx);
}
	
auto ring::wraparound_view(index_type start, std::size_t dur) -> wraparound_view_type {
	return wraparound_view_type(*this, start, dur);
}

auto ring::wraparound_view(index_type start, std::size_t dur) const -> const_wraparound_view_type {
	return const_wraparound_view_type(*this, start, dur);
}

auto ring::const_wraparound_view(index_type start, std::size_t dur) const -> const_wraparound_view_type {
	return const_wraparound_view_type(*this, start, dur);
}
	
	
}