#ifndef TFF_FILTER_VIEW_TYPES_H_
#define TFF_FILTER_VIEW_TYPES_H_

#include "../nd/ndarray_view.h"
#include "../nd/ndarray_timed_wraparound_view.h"

namespace tff {

template<std::size_t Dim, typename Elem>
using filter_input_full_view = ndarray_timed_wraparound_view<Dim + 1, const Elem>;

template<std::size_t Dim, typename Elem>
using filter_input_view = ndarray_view<Dim, const Elem>;

template<std::size_t Dim, typename Elem>
using filter_output_view = ndarray_view<Dim, Elem>;

}

#endif
