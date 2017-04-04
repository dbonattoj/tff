#include "enclosure_box.h"
#include "enclosure_filter.h"

namespace tff {

enclosure_box::enclosure_box() :
	filter_(enclosure_filter_base::current_in_construction()) { }

}
