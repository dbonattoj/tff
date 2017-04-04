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


/// Signed type representing frame time index, duration or difference
using time_unit = std::ptrdiff_t;

constexpr time_unit undefined_time = -1;

struct time_span {
	time_unit begin;
	time_unit end;
	
	time_span() : begin(undefined_time), end(undefined_time) { }
	time_span(time_unit begin_, time_unit end_) : begin(begin_), end(end_)
		{ Assert_crit(begin_ <= end_); }
	
	time_unit duration() const { return end - begin; }
};


struct time_window {
	time_unit past;
	time_unit future;
	
	time_window() : past(0), future(0) { }
	time_window(time_unit past_, time_unit future_) : past(past_), future(future_)
		{ Assert_crit(past >= 0 && future >= 0); }
	
	time_unit duration() const { return past + 1 + future; }
};

time_window operator+(const time_window&, const time_window&);

time_window max(const time_window&, const time_window&);
time_window min(const time_window&, const time_window&);

time_span expand(const time_span&, const time_window&);
time_span expand_truncate(const time_span&, const time_window&);
time_span truncate(time_span);

};

#endif


