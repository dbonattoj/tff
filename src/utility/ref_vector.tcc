#include "ref_vector.h"
#include <type_traits>
#include <iterator>

namespace tff {

namespace detail {

template<typename To, typename From, typename Allocator, typename Cast>
ref_vector<To, Allocator> ref_vector_cast_(const ref_vector<From, Allocator>& in, const Cast& cast) {
	ref_vector<To, Allocator> out(in.size());
	std::transform(in.begin(), in.end(), std::inserter(out, out.end()), cast);
	return out;
}


template<typename Input_it>
class ref_vector_ptr_iterator_wrapper : private Input_it {
	using base = Input_it;
	using actual_value_type = typename base::value_type;
	
public:
	using iterator_category = std::input_iterator_tag;
	using value_type = actual_value_type*;
	using pointer = actual_value_type**;
	using reference = actual_value_type*;
	using typename base::difference_type;
	
	using base::base;
	
	actual_value_type* operator*() const { return &base::operator*(); }
	actual_value_type* operator->() const { return &base::operator*(); }
	
	bool operator==(const ref_vector_ptr_iterator_wrapper& other) const {
		return (static_cast<const base&>(*this) == static_cast<const base&>(other));
	}
	bool operator!=(const ref_vector_ptr_iterator_wrapper& other) const {
		return (static_cast<const base&>(*this) != static_cast<const base&>(other));
	}

	ref_vector_ptr_iterator_wrapper& operator++() {
		base::operator++();
		return *this;
	}
	ref_vector_ptr_iterator_wrapper operator++(int) {
		return ref_vector_ptr_iterator_wrapper(base::operator++(int()));
	}
};


template<typename Input_it>
auto ref_vector_wrap_input(Input_it it) {
	return ref_vector_ptr_iterator_wrapper<Input_it>(it);
}


}

template<typename T, typename Allocator> template<typename It>
ref_vector<T, Allocator>::ref_vector(It begin, It end, const allocator_type& alloc) :
	vector_(detail::ref_vector_wrap_input(begin), detail::ref_vector_wrap_input(end), alloc) { }


template<typename T, typename Allocator> template<typename It>
auto ref_vector<T, Allocator>::insert(iterator pos, It begin, It end) -> iterator {
	auto it = vector_.insert(pos.base(), detail::ref_vector_wrap_input(begin), detail::ref_vector_wrap_input(end));
	return iterator(it);
}


template<typename To, typename From, typename Allocator>
ref_vector<To, Allocator> static_ref_vector_cast(const ref_vector<From, Allocator>& in) {
	return detail::ref_vector_cast_<To>(in, [](From& item) { return static_cast<To&>(item); });
}


template<typename To, typename From, typename Allocator>
ref_vector<To, Allocator> dynamic_ref_vector_cast(const ref_vector<From, Allocator>& in) {
	return detail::ref_vector_cast_<To>(in, [](From& item) { return dynamic_cast<To&>(item); });
}


template<typename To, typename From, typename Allocator>
ref_vector<To, Allocator> const_ref_vector_cast(const ref_vector<From, Allocator>& in) {
	return detail::ref_vector_cast_<To>(in, [](From& item) { return const_cast<To&>(item); });
}


template<typename To, typename From, typename Allocator>
ref_vector<To, Allocator> reinterpret_ref_vector_cast(const ref_vector<From, Allocator>& in) {
	return detail::ref_vector_cast_<To>(in, [](From& item) { return reinterpret_cast<To&>(item); });
}


}