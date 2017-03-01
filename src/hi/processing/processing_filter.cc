#include "processing_filter.h"

namespace tff {

thread_local processing_filter_base* processing_filter_base::current_in_construction_ = nullptr;


void processing_filter_base::handler_pre_process_(processing_job& nd_job) {
	processing_filter_job job(nd_job);
	return this->box_pre_process_(job);
}


void processing_filter_base::handler_process_(processing_job& nd_job) {
	processing_filter_job job(nd_job);
	return this->box_process_(job);
}


}