#ifndef TFF_FRAME_STATE_H_
#define TFF_FRAME_STATE_H_

#include <iostream>

namespace tff {

/*
struct frame_state_flag {
	enum value_type {
		undefined = 0,
		success = 1,
		end_of_stream = 2,
		failure = 3
	} value_;
	
	frame_state_flag() { }
	frame_state_flag(value_type val) : value_(val) {
		std::cout << this << " <--(c) " << (int)value_ << std::endl;
	}
	frame_state_flag(const frame_state_flag& flag) :
		frame_state_flag(flag.value_) { }

	frame_state_flag& operator=(value_type val) {
		std::cout << this << " <--(=) " << (int)val << std::endl;
		value_ = val;
		return *this;
	}
	frame_state_flag& operator=(const frame_state_flag& flag) {
		return operator=(flag.value_);
	}
	
	operator value_type () const {
		//std::cout << this << " = " << (int)value_ << std::endl;
		return value_;
	}
};
*/

enum class frame_state_flag {
	undefined,
	success,
	end_of_stream,
	failure
};

struct frame_state {
	frame_state_flag flag = frame_state_flag::undefined;
};

}

#endif