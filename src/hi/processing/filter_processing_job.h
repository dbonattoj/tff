#ifndef TFF_FILTER_PROCESSING_JOB_H_
#define TFF_FILTER_PROCESSING_JOB_H_

#include "../../nd.h"

namespace tff {

class processing_job;

template<std::size_t Dim, typename Elem> class filter_input;
template<std::size_t Dim, typename Elem> class filter_output;

class filter_processing_job {
private:
	processing_job& node_job_;
	
public:
	explicit filter_processing_job(processing_job& job) : node_job_(job) { }
	
	template<std::size_t Dim, typename Elem>
	ndarray_timed_wraparound_view<Dim + 1, Elem> in_full(filter_input<Dim, Elem>&);
	
	
};

#include "filter_processing_job.tcc"

}

#endif
