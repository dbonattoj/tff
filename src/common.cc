#include "common.h"
#include <algorithm>

namespace tff {

time_window max(const time_window& a, const time_window& b) {
	return time_window(std::max(a.past, b.past), std::max(a.future, b.future));
}


time_window min(const time_window& a, const time_window& b) {
	return time_window(std::min(a.past, b.past), std::min(a.future, b.future));
}


}