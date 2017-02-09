#ifndef TFF_UTILITY_REF_VECTOR_H_
#define TFF_UTILITY_REF_VECTOR_H_

#include <vector>
#include "indirect_iterator.h"

namespace tff {

/// Array holding references to items.
/** Interface similar to `std::vector`, except that items are not copied and instead references are taken.
 ** Cannot default-construct items, they must always point to an existing object.
 ** Replaces `std::vector<T*>` or `std::vector<std:reference_wrapper<T>>`. Iteration using `indirect_iterator`. */
template<typename T, typename Allocator = std::allocator<T>>
class ref_vector {
public:
	using value_type = T;
	using underlying_vector_type = std::vector<T*>;
	using allocator_type = Allocator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = indirect_iterator<typename underlying_vector_type::iterator>;
	using const_iterator = indirect_iterator<typename underlying_vector_type::const_iterator>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	
private:
	underlying_vector_type vector_;

public:
	explicit ref_vector(const allocator_type& alloc = Allocator()) :
		vector_(alloc) { }
	explicit ref_vector(size_type count, T& ref, const allocator_type& alloc = allocator_type()) :
		vector_(count, &ref, alloc) { }
	ref_vector(const ref_vector& other) : vector_(other.vector_) { }
	ref_vector(const ref_vector& other, const allocator_type& alloc) : vector_(other.vector_, alloc) { }
	ref_vector(ref_vector&& other) : vector_(std::move(other.vector_)) { }
	ref_vector(ref_vector&& other, const allocator_type& alloc) : vector_(std::move(other.vector_), alloc) { }

	ref_vector& operator=(const ref_vector&) = default;
	ref_vector& operator=(ref_vector&&) = default;
	
	void assign(size_type count, const T& ref) { vector_.assign(count, &ref); }
	
	const underlying_vector_type& base() const { return vector_; }
	underlying_vector_type& base() { return vector_; }
	
	allocator_type get_allocator() const { return vector_.get_allocator(); }
	
	reference at(size_type pos) { return *vector_.at(pos); }
	const_reference at(size_type pos) const { return *vector_.at(pos); }
	reference operator[](size_type pos) { return *vector_.operator[](pos); }
	const_reference operator[](size_type pos) const { return *vector_.operator[](pos); }
	reference front() { return *vector_.front(); }
	const_reference front() const { return *vector_.front(); }
	reference back() { return *vector_.back(); }
	const_reference back() const { return *vector_.back(); }
	reference data() { return *vector_.data(); }
	const_reference data() const { return *vector_.data(); }
	
	iterator begin() { return iterator(vector_.begin()); }
	const_iterator begin() const { return const_iterator(vector_.cbegin()); }
	const_iterator cbegin() const { return const_iterator(vector_.cbegin()); }

	iterator end() { return iterator(vector_.end()); }
	const_iterator end() const { return const_iterator(vector_.cend()); }
	const_iterator cend() const { return const_iterator(vector_.cend()); }

	reverse_iterator rbegin() { return reverse_iterator(vector_.rbegin()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(vector_.rbegin()); }
	const_reverse_iterator crbegin() const { return const_reverse_iterator(vector_.rbegin()); }

	reverse_iterator rend() { return reverse_iterator(vector_.rend()); }
	const_reverse_iterator rend() const { return const_reverse_iterator(vector_.rend()); }
	const_reverse_iterator crend() const { return const_reverse_iterator(vector_.rend()); }

	bool empty() const { return vector_.empty(); }
	size_type size() const { return vector_.size(); }
	size_type max_size() const { return vector_.max_size(); }
	void reserve(size_type new_cap) { vector_.reserve(new_cap); }
	size_type capacity() const { return vector_.capacity(); }
	void shrink_to_fit() { vector_.shrink_to_fit(); }
	
	void clear() { vector_.clear(); }
	iterator insert(const_iterator pos, const T& ref)
		{ return iterator(vector_.insert(pos.base(), &ref)); }
	iterator insert(const_iterator pos, size_type count, const T& ref)
		{ return iterator(vector_.insert(pos.base(), count, &ref)); }
	iterator emplace(const_iterator pos, const T& ref)
		{ return iterator(vector_.insert(pos.base(), &ref)); }
	iterator erase(const_iterator pos)
		{ return iterator(vector_.erase(pos.base())); }
	iterator erase(const_iterator begin, const_iterator end)
		{ return iterator(vector_.erase(begin.base(), end.base())); }
	void push_back(const T& ref) { vector_.push_back(&ref); }
	reference emplace_back(const T& ref) { vector_.push_back(&ref); return back(); }
	void pop_back() { vector_.pop_back(); }
	void resize(size_type count, const T& ref) { vector_.resize(count, &ref); }
	void resize(size_type count) {
		if(count > size()) throw std::invalid_argument("unique_ptr_vector cannot resize larger");
		vector_.resize(count);
	}
	void swap(ref_vector& other) { vector_.swap(other.vector_); }
};

template<typename T, typename Allocator>
void swap(ref_vector<T, Allocator>& a, ref_vector<T, Allocator>& b) {
	std::swap(a.base(), b.base());
}

}

#endif
