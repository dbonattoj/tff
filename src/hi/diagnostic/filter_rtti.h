#ifndef TFF_FILTER_RTTI_H_
#define TFF_FILTER_RTTI_H_

#include "../filter.h"
#include "../processing/processing_filter.h"
#include "../enclosure/enclosure_filter.h"

namespace tff {

bool is_processing_filter(const filter&);
bool is_enclosure_filter(const filter&);

template<typename Function>
void filter_downcast_dispatch(const filter& filt, Function&& func) {
	if(is_processing_filter(filt)) func(static_cast<const processing_filter_base&>(filt));
	else if(is_enclosure_filter(filt)) func(static_cast<const enclosure_filter_base&>(filt));
	else func(filt);
}

}

#endif