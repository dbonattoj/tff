#ifndef TFF_PROCESSING_FILTER_H_
#define TFF_PROCESSING_FILTER_H_

#include "../filter.h"
#include "../../lo/processing/processing_handler.h"

namespace tff {

class processing_job;
class filter_installation_guide;

class processing_filter_base : public filter, public processing_handler {
protected:
	static thread_local processing_filter_base* current_in_construction_ = nullptr;

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
	processing_filter();
	
	const box_type& box() const { return *box_; }
	box_type& box() { return *box_; }
};

}

#include "processing_filter.tcc"

#endif