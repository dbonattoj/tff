#include "exception.h"
#include "debug.h"
#include <cstdio>

namespace tff {

std::string exception::full_message_string_() const noexcept {
	return type_ + (message_.empty() ? std::string() : " : " + message_);
}
	

const char* exception::what() const noexcept {
	if(full_message_.empty()) full_message_ = this->full_message_string_();
	return full_message_.c_str();
}
	

#if TFF_DEBUG_BUILD
exception::exception(const std::string& type, const std::string& msg) :
	type_(type), message_(msg), trace_(get_stack_trace()) { }
#else
exception::exception(const std::string& type, const std::string& msg) :
	type_(type), message_(msg) { }
#endif


}