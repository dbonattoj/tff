#ifndef TFF_FILTER_EDGE_CASTER_H_
#define TFF_FILTER_EDGE_CASTER_H_

#include "view_types.h"

namespace tff {

template<std::size_t Dim, typename Elem>
class filter_null_edge_caster {
public:
	using output_frame_shape_type = ndsize<Dim>;
	using output_full_view_type = filter_input_full_view<Dim, Elem>;
	using input_frame_shape_type = output_frame_shape_type;
	using input_full_view_type = output_full_view_type;
	
	const input_full_view_type& cast_view(const output_full_view_type& vw) const {
		return vw;
	}
	
	const input_frame_shape_type& cast_frame_shape(const output_frame_shape_type& shp) const {
		return shp;
	}
};

}

#endif