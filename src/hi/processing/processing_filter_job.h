#ifndef TFF_PROCESSING_FILTER_JOB_H_
#define TFF_PROCESSING_FILTER_JOB_H_

#include "../../nd/nd.h"

namespace tff {

class processing_job;

class filter_input_base;
template<std::size_t Dim, typename Elem> class filter_input;
class filter_output_base;
template<std::size_t Dim, typename Elem> class filter_output;

class processing_filter_job {
private:
	processing_job& node_job_;
	
public:
	explicit processing_filter_job(processing_job& job) : node_job_(job) { }
	
	time_unit time() const;
	
	template<std::size_t Dim, typename Elem>
	ndarray_timed_wraparound_view<Dim + 1, const Elem> in_full(filter_input<Dim, Elem>&);
	
	template<std::size_t Dim, typename Elem>
	ndarray_view<Dim, const Elem> in(filter_input<Dim, Elem>&);
	
	template<std::size_t Dim, typename Elem>
	ndarray_view<Dim, const Elem> in(filter_input<Dim, Elem>&, time_unit t);
	
	template<std::size_t Dim, typename Elem>
	ndarray_view<Dim, Elem> out(filter_output<Dim, Elem&>);
	
	void set_activated(filter_input_base&, bool activated);
	bool is_activated(const filter_input_base&) const;
	
	void activate(filter_input_base& in) { set_activated(in, true); }
	void deactivate(filter_input_base& in) { set_activated(in, false); }
};

#include "processing_filter_job.tcc"

}

#endif
