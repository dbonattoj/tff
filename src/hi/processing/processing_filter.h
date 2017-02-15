#ifndef TFF_PROCESSING_FILTER_H_
#define TFF_PROCESSING_FILTER_H_

#include "../filter.h"
#include "../../lo/processing/processing_handler.h"

namespace tff {

class filter_graph_installation;
class processing_job;

template<typename Box>
class processing_filter : public filter, private processing_handler {
public:
	using box_type = Box;
	
private:
	optional<box_type> box_;
	bool asynchronous_ = false;
	time_unit prefetch_duration_ = 0;
	
	void handler_pre_process_(processing_job&) override;
	void handler_process_(processing_job&) override;
	
protected:
	void install_(filter_graph_installation_guide&) const override;
	
public:
	processing_filter();
	
	const box_type& box() const { return *box_; }
	box_type& box() { return *box_; }
};

}

#include "processing_filter.tcc"

#endif