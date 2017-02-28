#ifndef TFF_FILTER_SUBGRAPH_H_
#define TFF_FILTER_SUBGRAPH_H_

#include "processing/processing_filter.h"
#include "../utility/unique_ptr_vector.h"
#include <utility>

namespace tff {

class filter_subgraph {
private:
	unique_ptr_vector<filter> filters_;

public:
	const auto& filters() const { return filters_; }
	auto& filters() { return filters_; }
	
	template<typename Box, typename... Box_args>
	processing_filter<Box>& add_processing_filter(Box_args&&... args) {
		using filter_type = processing_filter<Box>;
		return filters_.emplace_back<filter_type>(std::forward<Box_args>(args)...);
	}
};

}

#endif