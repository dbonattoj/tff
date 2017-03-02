#include "filter_input.h"
#include "filter.h"

namespace tff {

filter_input_base::filter_input_base(filter& filt) : filter_(filt) {
	filt.register_input(*this);
}

}