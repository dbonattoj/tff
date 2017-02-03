#ifndef TFF_RING_H_
#define TFF_RING_H_

#include "../../nd/nd.h"
#include "ring_format.h"
#include "opaque_ref_format.h"

#include <vector>

namespace tff {

/// Circular buffer containing the frames flowing through the graph.
/** \ref ring is accessed through \ref rqueue. */
class ring {
private:
	template<bool Mutable> class frame_view;
	template<bool Mutable> class wraparound_view;

	using frame_opaque_format_type = opaque_ref_format<opaque_ndarray_format>;
	using parameter_opaque_format_type = void;

	using frame_array_type = ndarray_opaque<1, frame_opaque_format_type>;
	using parameter_array_type = ndarray_opaque<1, parameter_opaque_format_type>;
	
	using index_type = std::ptrdiff_t;
	using frame_type = frame_view<true>;
	using const_frame_type = frame_view<false>;
	using wraparound_view_type = wraparound_view<true>;
	using const_wraparound_view_type = wraparound_view<false>;
	
	ring_format format_;
	std::size_t capacity_;
	std::vector<frame_array_type> frame_arrays_;
	std::vector<parameter_array_type> parameter_arrays_;
	
public:
	explicit ring(std::size_t capacity, const ring_format& format);
	
	std::size_t capacity() const { return capacity_; }
	
	frame_type operator[](index_type idx)
		{ return frame_type(*this, idx); }
	const_frame_type operator[](index_type idx)
		{ return const_frame_type(*this, idx); }
	
	wraparound_view_type wraparound_view(index_type start, std::size_t dur)
		{ return wraparound_view_type(*this, start, dur); }
	const_wraparound_view_type wraparound_view(index_type start, std::size_t dur) const
		{ return const_wraparound_view_type(*this, start, dur); }
	const_wraparound_view_type const_wraparound_view(index_type start, std::size_t dur) const
		{ return const_wraparound_view_type(*this, start, dur); }
};


template<bool Mutable>
class ring::frame_view {
public:
	using ndarray_frame_view_type = ndarray_opaque_view<0, Mutable, frame_opaque_format_type>;

private:
	ring& ring_;
	std::ptrdiff_t index_;
	
public:
	frame_view(ring& rng, std::ptrdiff_t idx) :
		ring_(rng), index_(idx) { }
	
	ndarray_frame_view_type ndarray(std::ptrdiff_t chan) const {
		return ring_.frame_arrays_.at(chan)[index];
	}
};


template<bool Mutable>
class ring::wraparound_view {
public:
	using ndarray_wraparound_view_type = ndarray_wraparound_opaque_view<1, Mutable, frame_opaque_format_type>;

private:
	ring& ring_;
	std::ptrdiff_t start_index_;
	std::size_t duration_;
	
public:
	wraparound_view(ring& rng, std::ptrdiff-t idx, std::size_t dur) :
		ring_(rng), start_index_(idx), duration_(dur) { }

	ndarray_wraparound_view_type ndarray(std::ptrdiff_t chan) const {
		return wraparound(
			ring_.frame_arrays_.at(chan).view(),
			make_ndptrdiff(start_index_),
			make_ndptrdiff(start_index_ + duration_)
		);
	}
};

}

#endif
