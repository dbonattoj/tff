#ifndef TFF_UTILITY_EXCEPTION_H_
#define TFF_UTILITY_EXCEPTION_H_

#include <exception>
#include <string>
#include "../config.h"
#include "debug.h"

#define TFF_DEFINE_EXCEPTION(__name__) \
	class __name__ : public ::tff::exception { \
	public: \
		explicit __name__(const std::string& msg = "") : \
			exception(#__name__, msg) { } \
	};

namespace tff {

class exception : public std::exception {
private:
	std::string type_;
	std::string message_;
	
	#if TFF_DEBUG_BUILD
	stack_trace trace_;
	#endif
	
	mutable std::string full_message_;
	
protected:
	virtual std::string full_message_string_() const noexcept;
	
public:
	exception(const std::string& type, const std::string& msg);
	
	const char* what() const noexcept override;
	
	const std::string& type() const { return type_; }
	const std::string& message() const { return message_; }
	
	#if TFF_DEBUG_BUILD
	const stack_trace& trace() const { return trace_; }
	#endif
};


TFF_DEFINE_EXCEPTION(not_implemented);

TFF_DEFINE_EXCEPTION(invalid_node_graph);

TFF_DEFINE_EXCEPTION(invalid_filter_graph);
TFF_DEFINE_EXCEPTION(filter_box_error);

TFF_DEFINE_EXCEPTION(flow_synchronization_error);

}

#endif
