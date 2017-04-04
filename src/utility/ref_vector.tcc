#include "ref_vector.h"

namespace tff {

template<typename To, typename From, typename Allocator>
ref_vector<To, Allocator> static_ref_vector_cast(const ref_vector<From, Allocator>& in) {
	ref_vector<To, Allocator> out(in.size());
	std::transform(
		in.begin(), in.end(), std::inserter(out, out.end()),
		[](From& item) { return static_cast<To&>(item); }
	);
	return out;
}


template<typename To, typename From, typename Allocator>
ref_vector<To, Allocator> dynamic_ref_vector_cast(const ref_vector<From, Allocator>& in) {
	ref_vector<To, Allocator> out(in.size());
	std::transform(
		in.begin(), in.end(), std::inserter(out, out.end()),
		[](From& item) { return dynamic_cast<To&>(item); }
	);
	return out;
}


template<typename To, typename From, typename Allocator>
ref_vector<To, Allocator> const_ref_vector_cast(const ref_vector<From, Allocator>& in) {
	ref_vector<To, Allocator> out(in.size());
	std::transform(
		in.begin(), in.end(), std::inserter(out, out.end()),
		[](From& item) { return const_cast<To&>(item); }
	);
	return out;
}


template<typename To, typename From, typename Allocator>
ref_vector<To, Allocator> reinterpret_ref_vector_cast(const ref_vector<From, Allocator>& in) {
	ref_vector<To, Allocator> out(in.size());
	std::transform(
		in.begin(), in.end(), std::inserter(out, out.end()),
		[](From& item) { return reinterpret_cast<To&>(item); }
	);
	return out;
}


}