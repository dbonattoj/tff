#include "filter_box.h"
#include "processing_filter.h"

namespace tff {

filter_box::filter_box() :
	filter_(processing_filter_base::current_in_construction()) { }

}
