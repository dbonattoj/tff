#include "../config.h"
#if TFF_DEBUG_BUILD

#include "debug.h"
#include "exception.h"
#include <exception>
#include <cstdio>
#include <typeinfo>
#include <cstdlib>

namespace tff {

namespace {

[[noreturn]] void terminate_handler_() {
	std::exception_ptr ex = std::current_exception();
	try {
		if(ex) std::rethrow_exception(ex);
	} catch(const exception& ex) {
		std::fprintf(stderr, "uncaught exception (%s) with message:\n  %s\n", ex.type().c_str(), ex.what());
		print_stack_trace(ex.trace());
	} catch(const std::exception& ex) {
		std::fprintf(stderr, "uncaught exception (%s) with message:\n  %s", typeid(ex).name(), ex.what());
	} catch(...) {
		std::fprintf(stderr, "uncaught exception (unknown type)\n");
	}
	std::_Exit(EXIT_FAILURE);
}

void setup_debug_() {
	using namespace backward;
	std::set_terminate(&terminate_handler_);
	
	SignalHandling sh;
}

class setup_debug_caller_ {
public:
	setup_debug_caller_() { setup_debug_(); }
};
setup_debug_caller_ caller;

};

stack_trace get_stack_trace() {
	using namespace backward;
	constexpr std::size_t maximal_traces = 32;

	StackTrace st;
	st.load_here(maximal_traces);
	return st;
}


void print_stack_trace(stack_trace trace) {
	using namespace backward;

	Printer p;
	p.print(trace, stderr);
}

}

#endif