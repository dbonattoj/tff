#ifndef TFF_FILTER_SUBGRAPH_H_
#define TFF_FILTER_SUBGRAPH_H_

#include "filter.h"
#include "../utility/unique_ptr_vector.h"
#include "../utility/ref_vector.h"
#include <utility>

namespace tff {

template<typename Box> class processing_filter;
template<typename Box> class enclosure_filter;

class filter_subgraph {
private:
	unique_ptr_vector<filter> filters_;

public:
	const auto& filters() const { return filters_; }
	auto& filters() { return filters_; }

	ref_vector<filter> pulled_filters();
	ref_vector<const filter> pulled_filters() const;
	
	template<typename Box, typename... Box_args>
	processing_filter<Box>& add_processing_filter(Box_args&&... args) {
		using filter_type = processing_filter<Box>;
		return filters_.emplace_back<filter_type>(*this, std::forward<Box_args>(args)...);
	}
	
	template<typename Box, typename... Box_args>
	enclosure_filter<Box>& add_enclosure_filter(Box_args&&... args) {
		using filter_type = enclosure_filter<Box>;
		return filters_.emplace_back<filter_type>(*this, std::forward<Box_args>(args)...);
	}
};

}

#endif