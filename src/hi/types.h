#ifndef TFF_HI_TYPES_H_
#define TFF_HI_TYPES_H_

#include <cstddef>
#include "../nd/nd.h"

namespace tff {

template<std::size_t Dim, typename Elem>
using input_frame_window_view_type = ndarray_timed_wraparound_view<Dim + 1, const Elem>;

template<std::size_t Dim, typename Elem>
using input_frame_view_type = ndarray_view<Dim, const Elem>;

template<std::size_t Dim, typename Elem>
using output_frame_view_type = ndarray_view<Dim, Elem>;

}

#endif