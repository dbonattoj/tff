#ifndef TFF_LO_TYPES_H_
#define TFF_LO_TYPES_H_

namespace tff {

using thread_index_type = int;
constexpr thread_index_type undefined_thread_index = -1;

using rqueue_type = rqueue<ring>;
using rqueue_read_handle = rqueue<ring>::read_handle;
using rqueue_write_handle = rqueue<ring>::write_handle;

using input_index_type = std::ptrdiff_t;
using output_index_type = std::ptrdiff_t;
using channel_index_type = std::ptrdiff_t;

using input_ndarray_window_view_type = ndarray_timed_wraparound_opaque_view<1, false, ndarray_opaque_format_type>;
using input_ndarray_view_type = ndarray_opaque_view<0, false, ndarray_opaque_format_type>;
using channel_ndarray_view_type = ndarray_opaque_view<0, true, ndarray_opaque_format_type>;

}

#endif
