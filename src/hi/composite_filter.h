#ifndef TFF_COMPOSITE_FILTER_H_
#define TFF_COMPOSITE_FILTER_H_

#include "filter.h"
#include "filter_subgraph.h"
#include <memory>

namespace tff {

class composite_filter : public filter {
private:
	std::shared_ptr<filter_subgraph> subgraph_;
	int parallelization_factor_ = 1;
	
	explicit composite_filter(const composite_filter&);
	
public:
	composite_filter();
	~composite_filter() override;
	
	filter_subgraph& subgraph() { return *subgraph_; }
	const filter_subgraph& subgraph() const { return *subgraph_; }
	
	composite_filter shallow_copy() const;
};

}

#endif