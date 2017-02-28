#ifndef TFF_PROCESSING_FILTER_H_
#define TFF_PROCESSING_FILTER_H_

#include "../filter.h"
#include "../../lo/processing/processing_handler.h"
#include "processing_filter_job.h"
#include "../../lo/types.h"

namespace tff {

class processing_job;
class filter_installation_guide;

class processing_filter_base : public filter, public processing_handler {
protected:
	static thread_local processing_filter_base* current_in_construction_;

	void handler_pre_process_(processing_job&) override;
	void handler_process_(processing_job&) override;
	
	virtual void box_pre_process_(processing_filter_job&) = 0;
	virtual void box_process_(processing_filter_job&) = 0;
	
public:
	processing_filter_base& current_in_construction() {
		Assert(current_in_construction_ != nullptr);
		return *current_in_construction_;
	}
};


template<typename Box>
class processing_filter : public processing_filter_base {
public:
	using box_type = Box;
	
private:
	optional<box_type> box_;
	bool asynchronous_ = false;
	time_unit prefetch_duration_ = 0;

	void box_pre_process_(processing_filter_job&) override;
	void box_process_(processing_filter_job&) override;
	
protected:
	void install_(filter_installation_guide&) override;
	
public:
	template<typename... Box_args>
	explicit processing_filter(Box_args&&...);
	
	const box_type& box() const { return *box_; }
	box_type& box() { return *box_; }
	
	const box_type& operator*() const { return box(); }
	box_type& operator*() { return box(); }
	const box_type* operator->() const { return &box(); }
	box_type* operator->() { return &box(); }
};


/// Input of \ref processing_filter.
/** Corresponds to \ref node_input of a the installed \ref processing_node(s) for this filter. */
template<std::size_t Input_dim, typename Input_elem>
class processing_filter_input : public filter_input<Input_dim, Input_elem> {
private:
	input_index_type node_input_index_ = -1;
	
public:
	processing_filter_input() :
		filter_input::filter_input(processing_filter_base::current_in_construction()) { }
	
	input_index_type node_input_index() const { return node_input_index_; }
	void set_node_input_index(input_index_type idx) { node_input_index_ = idx; }
};


/// Output of \ref processing_filter.
/** Corresponds to data channel of a the installed \ref processing_node(s) for this filter. */
template<std::size_t Output_dim, typename Output_elem>
class processing_filter_output : public filter_output<Output_dim, Output_elem> {
private:
	data_channel_index_type node_data_channel_index_ = -1;
	
public:
	processing_filter_output() :
		filter_output::filter_output(processing_filter_base::current_in_construction()) { }
	
	data_channel_index_type node_data_channel_index() const { return node_data_channel_index_; }
	void set_node_data_channel_index(data_channel_index_type idx) { node_data_channel_index_ = idx; }
};


};

#include "processing_filter.tcc"

#endif