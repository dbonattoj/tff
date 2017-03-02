#ifndef TFF_RING_H_
#define TFF_RING_H_

#include "../types.h"
#include "../../nd/nd.h"
#include "frame_state.h"
#include "ring_format.h"
#include "opaque_ref_format.h"

#include <vector>
#include <type_traits>

#include <iostream>

namespace tff {

/// Circular buffer containing a fixed number of frames.
/** Number of _frames_ is fixed to _capacity_. Each _frame_ consists of several _channels_:
 ** - _State_ channel containing state information etc.
 ** - _Data_ channels containing opaque nd-array data with given format.
 ** - _Parameter_ channels...
 ** Provides wraparound view to circular buffer segment, using underlying \ref ndarray_wraparound_view.
 ** To be accessed through \ref rqueue<ring>. */
class ring {
private:
	template<bool Mutable> class frame_view_;
	template<bool Mutable> class wraparound_view_;

public:
	using data_channel_container_type = ndarray_opaque<1, propagated_opaque_format_type>;
	
	using index_type = std::ptrdiff_t;
	using frame_type = frame_view_<true>;
	using const_frame_type = frame_view_<false>;
	using wraparound_view_type = wraparound_view_<true>;
	using const_wraparound_view_type = wraparound_view_<false>;
	
private:
	std::size_t capacity_;
	
	ndarray<1, frame_state> state_channel_;
	std::vector<data_channel_container_type> data_channels_;
	
public:
	ring(std::size_t capacity, const ring_format& format);
	
	std::size_t capacity() const { return capacity_; }
	
	frame_type frame_view(index_type, time_unit);
	const_frame_type frame_view(index_type, time_unit) const;
	const_frame_type const_frame_view(index_type, time_unit) const;
	
	wraparound_view_type wraparound_view(index_type start_idx, time_unit start_t, std::size_t dur);
	const_wraparound_view_type wraparound_view(index_type start_idx, time_unit start_t, std::size_t dur) const;
	const_wraparound_view_type const_wraparound_view(index_type start_idx, time_unit start_t, std::size_t dur) const;
	
	std::size_t data_channels_count() const { return data_channels_.size(); }
};


/// View to one frame of \ref ring.
template<bool Mutable>
class ring::frame_view_ {
public:
	using ring_type = const_if<!Mutable, ring>;
	using state_view_type = const_if<!Mutable, frame_state>&;
	using data_view_type = std::conditional_t<Mutable, mutable_data_frame_view_type, const_data_frame_view_type>;

private:
	ring_type& ring_;
	index_type index_;
	time_unit time_;
	
public:
	frame_view_(ring_type& rng, index_type idx, time_unit t) :
		ring_(rng), index_(idx), time_(t) { }
	
	time_unit time() const { return time_; }
	
	state_view_type state() const {
		return ring_.state_channel_[index_];
	}
	
	data_view_type data(data_channel_index_type idx) const {
		return ring_.data_channels_.at(idx)[index_];
	}
};


/// View to segment of \ref ring.
template<bool Mutable>
class ring::wraparound_view_ {
public:
	using ring_type = const_if<!Mutable, ring>;
	using state_view_type = std::conditional_t<Mutable, mutable_state_window_view_type, const_state_window_view_type>;
	using data_view_type = std::conditional_t<Mutable, mutable_data_window_view_type, const_data_window_view_type>;

private:
	ring_type& ring_;
	index_type start_index_;
	time_unit start_time_;
	std::size_t duration_;
	
public:
	wraparound_view_(ring_type& rng, index_type start_idx, time_unit start_t, std::size_t dur) :
		ring_(rng), start_index_(start_idx), start_time_(start_t), duration_(dur) { }
	
	state_view_type state() const {
		auto non_timed_vw = wraparound(
			ring_.state_channel_.view(),
			make_ndptrdiff(start_index_),
			make_ndptrdiff(start_index_ + duration_)
		);
		return timed(non_timed_vw, start_time_);
	}
	
	data_view_type data(data_channel_index_type chan) const {
		auto non_timed_vw = wraparound(
			ring_.data_channels_.at(chan).view(),
			make_ndptrdiff(start_index_),
			make_ndptrdiff(start_index_ + duration_)
		);
		return timed(non_timed_vw, start_time_);
	}
};

}

#endif
