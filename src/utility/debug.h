#ifndef TFF_UTILITY_DEBUG_H_
#define TFF_UTILITY_DEBUG_H_

#include "../config.h"
#if TFF_DEBUG_BUILD

#include <backward.hpp>

namespace tff {

using stack_trace = backward::StackTrace;

stack_trace get_stack_trace();
void print_stack_trace(stack_trace);

}

#endif
#endif