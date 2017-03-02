#include "ring.h"

namespace tff {

ring::ring(std::size_t capacity, const ring_format& format) :
	capacity_(capacity),
	state_channel_(make_ndsize(capacity_))
{
	data_channels_.reserve(format.data_channels_count());
	for(std::ptrdiff_t i = 0; i < format.data_channels_count(); ++i) {
		propagated_opaque_format_type frame_format(format.data_channel(i));
		data_channels_.emplace_back(make_ndsize(capacity_), frame_format);
	}
}


auto ring::frame_view(index_type idx, time_unit t) -> frame_type {
	return frame_type(*this, idx, t);
}
auto ring::frame_view(index_type idx, time_unit t) const -> const_frame_type {
	return const_frame_type(*this, idx, t);
}
auto ring::const_frame_view(index_type idx, time_unit t) const -> const_frame_type {
	return const_frame_type(*this, idx, t);
}
	

auto ring::wraparound_view(index_type start_idx, time_unit start_t, std::size_t dur) -> wraparound_view_type {
	return wraparound_view_type(*this, start_idx, start_t, dur);
}
auto ring::wraparound_view(index_type start_idx, time_unit start_t, std::size_t dur) const -> const_wraparound_view_type {
	return const_wraparound_view_type(*this, start_idx, start_t, dur);
}
auto ring::const_wraparound_view(index_type start_idx, time_unit start_t, std::size_t dur) const -> const_wraparound_view_type {
	return const_wraparound_view_type(*this, start_idx, start_t, dur);
}

	
}