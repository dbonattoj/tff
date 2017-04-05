#include "assert.h"
#include "debug.h"
#include "../config.h"
#include <sstream>
#include <string>
#include <cstdio>


namespace tff {

std::string failed_assertion_exception::full_message_string_() const noexcept {
	std::ostringstream str;
	str << "failed assertion at " << source_filename_ << " line " << source_line_ << ": `" << source_condition_ << "`";
	if(! message().empty()) str << " (" << message() << ")";
	return str.str();
}


failed_assertion_exception::failed_assertion_exception
(const char* source_filename, int source_line, const char* source_condition, const char* message) :
	exception("failed_assertion", message),
	source_filename_(source_filename),
	source_line_(source_line),
	source_condition_(source_condition)
{
	#if TFF_DEBUG_BUILD
	std::fprintf(stderr, "%s\n", full_message_string_().c_str());
	print_stack_trace(trace());
	#endif
}


}