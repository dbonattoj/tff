#ifndef TFF_FRAME_METADATA_H_
#define TFF_FRAME_METADATA_H_

namespace tff {

enum class frame_state {
	success,
	end_of_stream,
	failure
};

struct frame_metadata {
	frame_state state = frame_state::failure;
};

}

#endif