#ifndef TFF_FRAME_METADATA_H_
#define TFF_FRAME_METADATA_H_

namespace tff {

struct frame_metadata {
	enum frame_state {
		success,
		end_of_stream,
		failure
	};
	
	frame_state state = failure;
};

}

#endif