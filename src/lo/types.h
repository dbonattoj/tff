#ifndef TFF_NODE_TYPES_H_
#define TFF_NODE_TYPES_H_

#include <cstddef>
#include "ring/opaque_ref_format.h"
#include "../nd/nd.h"
#include "../rqueue/rqueue.h"
#include "lo/ring/frame_state.h"

namespace tff {

class ring;
using rqueue_type = rqueue<ring>;

using thread_index_type = int;

using input_index_type = std::ptrdiff_t;
using output_index_type = std::ptrdiff_t;
using data_channel_index_type = std::ptrdiff_t;
//using parameter_channel_index_type = std::ptrdiff_t;

using propagated_opaque_format_type = opaque_ref_format<opaque_ndarray_format>;

using const_data_window_view_type = ndarray_timed_wraparound_opaque_view<1, false, propagated_opaque_format_type>;
using mutable_data_window_view_type = ndarray_timed_wraparound_opaque_view<1, true, propagated_opaque_format_type>;

using const_data_frame_view_type = ndarray_opaque_view<0, false, propagated_opaque_format_type>;
using mutable_data_frame_view_type = ndarray_opaque_view<0, true, propagated_opaque_format_type>;

using const_state_window_view_type = ndarray_timed_wraparound_view<1, const frame_state>;
using mutable_state_window_view_type = ndarray_timed_wraparound_view<1, frame_state>;

}

#endif
// TODO rename lo -> node; hi -> filter