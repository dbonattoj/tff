#ifndef TFF_ENCLOSURE_BOX_H_
#define TFF_ENCLOSURE_BOX_H_

#include "enclosure_filter.h"

namespace tff {

/// Base for application-defined enclosure filter box.
class enclosure_box {
private:
	enclosure_filter_base& filter_;
	
protected:
	template<std::size_t Dim, typename Elem> using input = enclosure_filter_input<Dim, Elem>;
	template<std::size_t Dim, typename Elem> using output = enclosure_filter_output<Dim, Elem>;
	
	filter_subgraph& graph() { return filter_.subgraph(); }
	const filter_subgraph& graph() const { return filter_.subgraph(); }
	
	enclosure_box();
};

}

#endif
