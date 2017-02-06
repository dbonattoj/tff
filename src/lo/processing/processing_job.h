#ifndef TFF_PROCESSING_JOB_H_
#define TFF_PROCESSING_JOB_H_

namespace tff {

class processing_job {
private:
	processing_node& node_;
	time_unit time_;
	std::vector<input_ndarray_window_view_type> input_views_;
	std::vector<channel_ndarray_view_type> channel_views_;
	
	bool end_of_stream_ = false;
	
	processing_job(const processing_job&) = delete;
	processing_job& operator=(const processing_job&) = delete;
	
public:
	processing_job(processing_node&, time_unit t);
	
	void set_input_view(input_index_type, const input_ndarray_window_view_type&);
	void set_channel_view(channel_index_type, const channel_ndarray_view_type&);
	
	time_unit time() const { return time_; }
	
	bool has_input_view(input_index_type idx) const
		{ return ! input_views_.at(idx).is_null(); }
	const input_ndarray_window_view_type& input_view(input_index_type idx) const
		{ return input_views_.at(idx); }
	
	bool has_channel_view(channel_index_type idx) const
		{ return ! channel_views_.at(idx).is_null(); }
	const channel_ndarray_view_type& channel_view(channel_index_type idx) const
		{ return channel_views_.at(idx); }

	bool end_of_stream_was_marked() const { return end_of_stream_; }
	void mark_end_of_stream() { end_of_stream_ = true; }
	
	void set_input_activated(input_index_type idx, bool activated);
	bool is_input_activated(input_index_type idx) const;
};

}

#endif
