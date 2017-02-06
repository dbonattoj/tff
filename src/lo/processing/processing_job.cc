#include "processing_job.h"

namespace tff {

processing_job::processing_job(processing_node& nd, time_unit t) :
	node_(nd), time_(t)
{
	input_views_.reserve(nd.inputs().size());
	channel_views_.reserve(nd.channels_count());
}


void processing_job::set_input_view(input_index_type idx, const input_ndarray_window_view_type& view) {
	input_views_.at(idx).reset(view);
}


void processing_job::set_channel_view(channel_index_type idx, const channel_ndarray_view_type& view);
	channel_views_.at(idx).reset(view);
}


void set_input_activated(input_index_type, bool activated) {
	
}


bool is_input_activated(input_index_type) const {
	
}


}