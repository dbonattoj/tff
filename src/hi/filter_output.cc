#include "filter_output.h"
#include "filter.h"

namespace tff {

filter_output_base::filter_output_base(filter& filt) : filter_(filt) {
	filt.register_output(*this);
}

}