#ifndef TFF_COMMON_H_
#define TFF_COMMON_H_

#include "config.h"
#include "utility/assert.h"
#include "utility/exception.h"
#include <cstddef>

#include <optional.hpp>

namespace tff {

using std::experimental::optional;
using std::experimental::nullopt;

class time_span;

/// Signed type representing frame time index, duration or difference
using time_unit = std::ptrdiff_t;

constexpr time_unit undefined_time = -1;

struct time_span {
	time_unit begin;
	time_unit end;
	
	time_span() = default;
	time_span(time_unit b, time_unit e) : begin(b), end(e) { }
	
	time_unit duration() const { return end - begin; }
};


struct time_window {
	time_unit past;
	time_unit future;
	
	time_window() : past(0), future(0) { }
	time_window(time_unit past_, time_unit future_) : past(past_), future(future_)
		{ Assert(past >= 0 && future >= 0); }
};


time_window max(const time_window&, const time_window&);
time_window min(const time_window&, const time_window&);

time_span expand(const time_span&, const time_window&, bool truncate_begin = true);

};

#endif


