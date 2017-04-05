#include "filter_rtti.h"

namespace tff {

namespace {
	template<typename T>
	bool is_a_or_derived_(const filter& filt) {
		return (dynamic_cast<const T*>(&filt) != nullptr);
	}
}

bool is_processing_filter(const filter& filt) {
	return is_a_or_derived_<processing_filter_base>(filt);
}


bool is_enclosure_filter(const filter& filt) {
	return is_a_or_derived_<enclosure_filter_base>(filt);
}

}