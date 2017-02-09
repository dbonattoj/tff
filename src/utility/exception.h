#ifndef TFF_UTILITY_EXCEPTION_H_
#define TFF_UTILITY_EXCEPTION_H_

#include <exception>
#include <stdexcept>

#define TFF_DEFINE_EXCEPTION(__name__, __base__) \
	class __name__ : public ::tff::exception_derived<__base__> { \
		using base = ::tff::exception_derived<__base__>; \
	public: \
		__name__(const std::string& what) : \
			base(std::string(#__name__) + ": " + what) { } \
	};

namespace tff {

template<typename Base>
class exception_derived : public Base {
	using base = Base;

public:
	explicit exception_derived(const std::string& what) : base(what) { }
};

TFF_DEFINE_EXCEPTION(failed_assertion, std::runtime_error);

}

#endif
