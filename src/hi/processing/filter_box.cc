#include "filter_box.h"
#include "processing_filter.h"

namespace tff {

namespace {

filter& processing_filter_being_constructed_() {
	return processing_filter_base::current_in_construction();
}

}

filter_box::filter_box() : filter(processing_filter_being_constructed_()) { }
filter_box::input::input() : filter_input(processing_filter_being_constructed_()) { }
filter_box::output::output() : filter_output(processing_filter_being_constructed_()) { }

}
