#include "common.h"
#include "nd/ndspan.h"
#include <algorithm>

namespace tff {

time_window operator+(const time_window& a, const time_window& b) {
	return time_window(a.past + b.past, a.future + b.future);
}

time_window max(const time_window& a, const time_window& b) {
	return time_window(std::max(a.past, b.past), std::max(a.future, b.future));
}


time_window min(const time_window& a, const time_window& b) {
	return time_window(std::min(a.past, b.past), std::min(a.future, b.future));
}


time_span expand(const time_span& span, const time_window& win) {
	time_unit begin = span.begin - win.past;
	time_unit end = span.end + win.future;
	return time_span(begin, end);
}


time_span expand(time_unit t, const time_window& win) {
	time_unit begin = t - win.past;
	time_unit end = t + 1 + win.future;
	return time_span(begin, end);
}


time_span truncate_begin(time_span span) {
	if(span.begin < 0) span.begin = 0;
	return span;
}


}