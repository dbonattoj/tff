#include "common.h"
#include "nd.h"
#include <algorithm>

namespace tff {

time_window max(const time_window& a, const time_window& b) {
	return time_window(std::max(a.past, b.past), std::max(a.future, b.future));
}


time_window min(const time_window& a, const time_window& b) {
	return time_window(std::min(a.past, b.past), std::min(a.future, b.future));
}


time_span expand(const time_span& span, const time_window& win, bool truncate_begin) {
	time_unit begin = span.start_time() - win.past;
	time_unit end = span.end_time() + win.future;
	if(truncate_begin && (begin < 0)) begin = 0;
	return time_span(begin, end);
}


}