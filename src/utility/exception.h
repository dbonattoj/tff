#ifndef TFF_UTILITY_EXCEPTION_H_
#define TFF_UTILITY_EXCEPTION_H_

#include <exception>
#include <stdexcept>

#define TFF_DEFINE_EXCEPTION(__name__, __base__) \
	class __name__ : public ::tff::exception_derived<__base__> { \
		using base = ::tff::exception_derived<__base__>; \
	public: \
		explicit __name__(const std::string& what = "") : \
			base(what.empty() ? std::string(#__name__) : std::string(#__name__) + ": " + what) { } \
	};

namespace tff {

template<typename Base>
class exception_derived : public Base {
	using base = Base;

public:
	explicit exception_derived(const std::string& what) : base(what) { }
};

TFF_DEFINE_EXCEPTION(failed_assertion, std::runtime_error);
TFF_DEFINE_EXCEPTION(not_implemented, std::logic_error);

TFF_DEFINE_EXCEPTION(invalid_node_graph, std::logic_error);

TFF_DEFINE_EXCEPTION(invalid_filter_graph, std::logic_error);
TFF_DEFINE_EXCEPTION(filter_box_error, std::runtime_error);

TFF_DEFINE_EXCEPTION(flow_synchronization_error, std::runtime_error);

}

#endif
