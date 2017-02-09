#ifndef TFF_LO_TYPES_H_
#define TFF_LO_TYPES_H_

#include <cstddef>
#include "ring/opaque_ref_format.h"
#include "../nd/nd.h"
#include "../rqueue/rqueue.h"

namespace tff {

class ring;

using thread_index_type = int;

using rqueue_type = rqueue<ring>;

using input_index_type = std::ptrdiff_t;
using output_index_type = std::ptrdiff_t;
using channel_index_type = std::ptrdiff_t;

using propagated_opaque_format_type = opaque_ref_format<opaque_ndarray_format>;
using input_ndarray_window_view_type = ndarray_timed_wraparound_opaque_view<1, false, propagated_opaque_format_type>;
using input_ndarray_view_type = ndarray_opaque_view<0, false, propagated_opaque_format_type>;
using channel_ndarray_view_type = ndarray_opaque_view<0, true, propagated_opaque_format_type>;

}

#endif
