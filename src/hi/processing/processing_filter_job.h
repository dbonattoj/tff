#ifndef TFF_PROCESSING_FILTER_JOB_H_
#define TFF_PROCESSING_FILTER_JOB_H_

#include "../view_types.h"

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
	filter_input_full_view<Dim, Elem> in_full(filter_input<Dim, Elem>&);
	
	template<std::size_t Dim, typename Elem>
	filter_input_view<Dim, Elem> in(filter_input<Dim, Elem>&);
	
	template<std::size_t Dim, typename Elem>
	filter_input_view<Dim, Elem> in(filter_input<Dim, Elem>&, time_unit t);
	
	template<std::size_t Dim, typename Elem>
	filter_output_view<Dim, Elem> out(filter_output<Dim, Elem&>);
	
	void set_input_activated(filter_input_base&, bool activated);
	bool is_input_activated(const filter_input_base&) const;
	
	void activate(filter_input_base& in) { set_input_activated(in, true); }
	void deactivate(filter_input_base& in) { set_input_activated(in, false); }
};

}

#include "processing_filter_job.tcc"

#endif
