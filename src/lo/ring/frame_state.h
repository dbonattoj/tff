#ifndef TFF_FRAME_STATE_H_
#define TFF_FRAME_STATE_H_

namespace tff {

enum class frame_state_flag {
	success,
	end_of_stream,
	failure
};

struct frame_state {
	frame_state_flag flag = frame_state_flag::failure;
};

}

#endif