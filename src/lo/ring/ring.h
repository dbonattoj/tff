#ifndef TFF_RING_H_
#define TFF_RING_H_

#include "../types.h"
#include "../../nd/nd.h"
#include "ring_format.h"
#include "opaque_ref_format.h"

#include <vector>

namespace tff {

/// Circular buffer containing a fixed number of frames.
/** Number of _frames_ is fixed to _capacity_. Each _frame_ consists of several _channels_:
 ** - _Metadata_ channel containing state information etc.
 ** - _Ndarray_ channels containing opaque ndarray shaped data with given format.
 ** - _Parameter_ channels...
 ** Also contains \ref ring_format object which holds frame format information.
 ** Provides wraparound view to circular buffer segment, using underlying \ref ndarray_wraparound_view.
 ** To be accessed through \ref rqueue<ring>. */
class ring {
public:
	enum frame_state {
		success,
		end_of_stream,
		failure
	};
	
	struct frame_metadata {
		frame_state state;
	};
	
private:
	template<bool Mutable> class frame_view_;
	template<bool Mutable> class wraparound_view_;

public:
	using ndarray_opaque_format_type = opaque_ref_format<opaque_ndarray_format>;
	//using parameter_opaque_format_type = void;

	using ndarray_array_type = ndarray_opaque<1, ndarray_opaque_format_type>;
	//using parameter_array_type = ndarray_opaque<1, parameter_opaque_format_type>;
	
	using index_type = std::ptrdiff_t;
	using frame_type = frame_view_<true>;
	using const_frame_type = frame_view_<false>;
	using wraparound_view_type = wraparound_view_<true>;
	using const_wraparound_view_type = wraparound_view_<false>;
	
private:
	ring_format format_;
	std::size_t capacity_;
	
	ndarray<1, frame_metadata> metadata_channel_;
	std::vector<ndarray_array_type> ndarray_channels_;
	//std::vector<parameter_array_type> parameter_channels_;
	
public:
	ring(std::size_t capacity, const ring_format& format);
	
	std::size_t capacity() const { return capacity_; }
	
	frame_type operator[](index_type);
	const_frame_type operator[](index_type) const;
	
	wraparound_view_type wraparound_view(index_type start, std::size_t dur);
	const_wraparound_view_type wraparound_view(index_type start, std::size_t dur) const;
	const_wraparound_view_type const_wraparound_view(index_type start, std::size_t dur) const;
	
	std::size_t ndarray_channels_count() const { return ndarray_channels_.size(); }
};


template<bool Mutable>
class ring::frame_view_ {
public:
	using ring_type = std::conditional_t<Mutable, ring, const ring>;
	using metadata_view_type = std::conditional_t<Mutable, frame_metadata&, const frame_metadata&>;
	using ndarray_view_type = ndarray_opaque_view<0, Mutable, ndarray_opaque_format_type>;

private:
	ring_type& ring_;
	std::ptrdiff_t index_;
	
public:
	frame_view_(ring_type& rng, std::ptrdiff_t idx) :
		ring_(rng), index_(idx) { }
	
	metadata_view_type metadata() const {
		return ring_.metadata_channel_[index_];
	}
	
	ndarray_view_type ndarray(channel_index_type chan) const {
		return ring_.ndarray_channels_.at(chan)[index_];
	}
};


template<bool Mutable>
class ring::wraparound_view_ {
public:
	using ring_type = std::conditional_t<Mutable, ring, const ring>;
	using metadata_view_type = ndarray_wraparound_view<1, std::conditional_t<Mutable, frame_metadata, const frame_metadata>>;
	using ndarray_view_type = ndarray_wraparound_opaque_view<1, Mutable, ndarray_opaque_format_type>;

private:
	ring_type& ring_;
	std::ptrdiff_t start_index_;
	std::size_t duration_;
	
public:
	wraparound_view_(ring_type& rng, std::ptrdiff_t idx, std::size_t dur) :
		ring_(rng), start_index_(idx), duration_(dur) { }
	
	metadata_view_type metadata() const {
		return wraparound(
			ring_.metadata_channel_,
			make_ndptrdiff(start_index_),
			make_ndptrdiff(start_index_ + duration_)
		);
	}
	
	ndarray_view_type ndarray(channel_index_type chan) const {
		return wraparound(
			ring_.ndarray_channels_.at(chan).view(),
			make_ndptrdiff(start_index_),
			make_ndptrdiff(start_index_ + duration_)
		);
	}
};

}

#endif
