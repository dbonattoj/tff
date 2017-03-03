#include "processing_filter_job.h"
#include "../../lo/processing/processing_job.h"
#include "../filter_input.h"

namespace tff {

inline processing_filter_job::processing_filter_job(processing_job& job) :
	node_job_(job) { }


inline time_unit processing_filter_job::time() const {
	return node_job_.time();
}

	
template<std::size_t Dim, typename Elem>
filter_input_full_view<Dim, Elem> processing_filter_job::in_full(filter_input<Dim, Elem>& in) {
	auto opaque_vw = node_job_.input_view(in.input_index());
	return in.edge().input_view_from_opaque(opaque_vw);
}
	

template<std::size_t Dim, typename Elem>
filter_input_view<Dim, Elem> processing_filter_job::in(filter_input<Dim, Elem>& in) {
	return in_full(in).at_time(time()).non_wraparound();
}
	

template<std::size_t Dim, typename Elem>
filter_input_view<Dim, Elem> processing_filter_job::in(filter_input<Dim, Elem>& in, time_unit t) {
	return in_full(in).at_time(t).non_wraparound();
}
	

template<std::size_t Dim, typename Elem>
filter_output_view<Dim, Elem> processing_filter_job::out(filter_output<Dim, Elem>& out) {
	auto opaque_vw = node_job_.data_channel_view(out.data_channel_index());
	return from_opaque<Dim, Elem>(opaque_vw);
}
	

inline void processing_filter_job::set_input_activated(filter_input_base& in, bool activated) {
	return node_job_.set_input_activated(in.input_index(), activated);
}


inline bool processing_filter_job::is_input_activated(const filter_input_base& in) const {
	return node_job_.is_input_activated(in.input_index());
}
	


}