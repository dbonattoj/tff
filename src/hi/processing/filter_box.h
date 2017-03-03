#ifndef TFF_FILTER_BOX_H_
#define TFF_FILTER_BOX_H_

#include "processing_filter.h"

namespace tff {

class processing_filter_job;

/// Base class for application filter implementation.
class filter_box {
private:
	filter& filter_;
	
protected:
	template<std::size_t Dim, typename Elem> using input = processing_filter_input<Dim, Elem>;
	template<std::size_t Dim, typename Elem> using output = processing_filter_output<Dim, Elem>;
	
	filter_box();
	virtual ~filter_box() = default;
	
public:
	void setup() { }
	void pre_process(processing_filter_job&) { }
	void process(processing_filter_job&) { }
};

}

#endif