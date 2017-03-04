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
	static processing_filter_base& current_in_construction() {
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
	void setup_() override;
	void install_(filter_installation_guide&) override;
	
public:
	template<typename... Box_args>
	explicit processing_filter(Box_args&&...);
	
	const box_type& box() const { return *box_; }
	box_type& box() { return *box_; }
	
	void set_asynchronous(bool async) { asynchronous_ = async; }
	bool is_asynchronous() const { return asynchronous_; }
	
	const box_type& operator*() const { return box(); }
	box_type& operator*() { return box(); }
	const box_type* operator->() const { return &box(); }
	box_type* operator->() { return &box(); }
};


/// Input of \ref processing_filter.
/** Corresponds to \ref node_input of a the installed \ref processing_node(s) for this filter. */
template<std::size_t Input_dim, typename Input_elem>
class processing_filter_input : public filter_input<Input_dim, Input_elem> {
	using base = filter_input<Input_dim, Input_elem>;
	
private:
	input_index_type input_index_ = -1;
	
public:
	processing_filter_input() :
		base(processing_filter_base::current_in_construction()) { }
	
	input_index_type input_index() const final override { return input_index_; }
	void set_input_index(input_index_type idx) final override { input_index_ = idx; }
};


/// Output of \ref processing_filter.
/** Corresponds to data channel of a the installed \ref processing_node(s) for this filter. */
template<std::size_t Output_dim, typename Output_elem>
class processing_filter_output : public filter_output<Output_dim, Output_elem> {
	using base = filter_output<Output_dim, Output_elem>;
	
private:
	data_channel_index_type data_channel_index_ = -1;
	
public:
	processing_filter_output() :
		base(processing_filter_base::current_in_construction()) { }
	
	data_channel_index_type data_channel_index() const final override { return data_channel_index_; }
	void set_data_channel_index(data_channel_index_type idx) final override { data_channel_index_ = idx; }
};

}

#include "processing_filter.tcc"

#endif