#ifndef TFF_COMPOSITE_FILTER_H_
#define TFF_COMPOSITE_FILTER_H_

#include "filter.h"
#include "../filter_subgraph.h"

namespace tff {

class composite_filter : public filter {
private:
	filter_subgraph subgraph_;
};

}

#endif