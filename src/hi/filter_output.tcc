#include "filter_graph_installation.h"
#include "../nd/nd.h"

namespace tff {

template<std::size_t Output_dim, typename Output_elem>
opaque_ndarray_format filter_output<Output_dim, Output_elem>::ndarray_format(const filter_output_format& frm) const {
	return default_opaque_ndarray_format<Output_elem>(frm.frame_shape);
}

}
