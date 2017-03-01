#include "processing_job.h"
#include "processing_node.h"
#include "../node_input.h"
#include "../node_output.h"

namespace tff {

processing_job::processing_job(processing_node& nd, time_unit t) :
	node_(nd),
	time_(t),
	input_views_(nd.inputs().size()),
	data_channel_views_(nd.data_channels_count()) { }


void processing_job::set_input_view(input_index_type idx, const const_data_window_view_type& view) {
	input_views_.at(idx).reset(view);
}


void processing_job::set_data_channel_view(data_channel_index_type idx, const mutable_data_frame_view_type& view) {
	data_channel_views_.at(idx).reset(view);
}


void processing_job::set_input_activated(input_index_type, bool activated) {
	
}


bool processing_job::is_input_activated(input_index_type) const {
	
}


}