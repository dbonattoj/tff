#include "common.h"
#include "nd/ndspan.h"
#include <algorithm>

namespace tff {

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


time_span expand_truncate(const time_span& span, const time_window& win) {
	time_span expanded_span = expand(span, win);
	if(expanded_span.begin < 0) expanded_span.begin = 0;
	return expanded_span;
}


}