#ifndef TFF_UTILITY_UNIQUE_PTR_VECTOR_H_
#define TFF_UTILITY_UNIQUE_PTR_VECTOR_H_

#include <vector>
#include <memory>
#include <stdexcept>
#include "indirect_iterator.h"

namespace tff {

/// Array holding unique pointers to items of differing types.
/** Interface similar to `std::vector`, except that items can be of different (polymorphic) types, derived from \a T.
 ** Items are stored on dynamically allocted memory, and owned by the contained.
 ** Replaces `std::vector<std::unique_ptr<T>>`. Iteration using `indirect_iterator`. */
template<typename T, typename Allocator = std::allocator<T>>
class unique_ptr_vector {
public:
	using value_type = T;
	using unique_ptr_type = std::unique_ptr<T>;
	using underlying_vector_type = std::vector<unique_ptr_type>;
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
	
	template<typename D>
	void initialize_(const D& item) {
		for(unique_ptr_type& ptr : vector_) *ptr = std::move(std::make_unique<D>(item));
	}

public:
	explicit unique_ptr_vector(const allocator_type& alloc = Allocator()) :
		vector_(alloc) { }
	
	template<typename D>
	explicit unique_ptr_vector(size_type count, const D& item, const allocator_type& alloc = allocator_type()) :
		vector_(count, unique_ptr_type(), alloc) { initialize_(item); }
	
	unique_ptr_vector(const unique_ptr_vector& other) = delete;
	unique_ptr_vector(unique_ptr_vector&& other) : vector_(std::move(other.vector_)) { }
	unique_ptr_vector(unique_ptr_vector&& other, const allocator_type& alloc) : vector_(std::move(other.vector_), alloc) { }

	unique_ptr_vector& operator=(const unique_ptr_vector&) = delete;
	unique_ptr_vector& operator=(unique_ptr_vector&&) = default;
	
	template<typename D>
	void assign(size_type count, const D& item = D()) {
		vector_.assign(count);
		initialize_(item);
	}
	
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
	const_iterator begin() const { return iterator(vector_.begin()); }
	const_iterator cbegin() const { return iterator(vector_.begin()); }

	iterator end() { return iterator(vector_.end()); }
	const_iterator end() const { return iterator(vector_.end()); }
	const_iterator cend() const { return iterator(vector_.end()); }

	reverse_iterator rbegin() { return iterator(vector_.rbegin()); }
	const_reverse_iterator rbegin() const { return iterator(vector_.rbegin()); }
	const_reverse_iterator crbegin() const { return iterator(vector_.rbegin()); }

	reverse_iterator rend() { return iterator(vector_.rend()); }
	const_reverse_iterator rend() const { return iterator(vector_.rend()); }
	const_reverse_iterator crend() const { return iterator(vector_.rend()); }

	bool empty() const { return vector_.empty(); }
	size_type size() const { return vector_.size(); }
	size_type max_size() const { return vector_.max_size(); }
	void reserve(size_type new_cap) { vector_.reserve(new_cap); }
	size_type capacity() const { return vector_.capacity(); }
	void shrink_to_fit() { vector_.shrink_to_fit(); }
	
	void clear() { vector_.clear(); }
	
	template<typename D> iterator insert(const_iterator pos, const D& item)
		{ return iterator(vector_.insert(pos.base(), std::make_unique<D>(item))); }
	template<typename D> iterator insert(const_iterator pos, D&& item)
		{ return iterator(vector_.insert(pos.base(), std::make_unique<D>(std::move(item)))); }
	
	template<typename D> iterator insert(const_iterator pos, size_type count, const D& item) {
		auto it = vector_.insert(pos.base(), count, unique_ptr_type());
		while(count--) *(it++) = std::move(std::make_unique<D>(item));
		return iterator(it);
	}

	template<typename D, typename... Args>
	iterator emplace(const_iterator pos, Args&&... args)
		{ return iterator(vector_.insert(pos.base(), std::make_unique<D>(std::forward<Args>(args)...))); }
	
	iterator erase(const_iterator pos)
		{ return iterator(vector_.erase(pos.base())); }
	iterator erase(const_iterator begin, const_iterator end)
		{ return iterator(vector_.erase(begin.base(), end.base())); }
	
	template<typename D>
	void push_back(const D& item) { vector_.push_back(std::make_unique<D>(item)); }
	template<typename D>
	void push_back(D&& item) { vector_.push_back(std::make_unique<D>(std::move(item))); }
	
	template<typename D, typename... Args>
	reference emplace_back(Args&&... args)
		{ vector_.push_back(std::make_unique<D>(std::forward<Args>(args)...)); return back(); }
	
	void pop_back() { vector_.pop_back(); }
	
	template<typename D>
	void resize(size_type count, const D& item) {
		std::ptrdiff_t more = count - size();
		vector_.resize(count, unique_ptr_type());
		if(more > 0) for(auto it = vector_.end() - more; it != vector_.end(); ++it)
			(*it) = std::move(std::make_unique<D>(item));
	}
	void resize(size_type count) {
		if(count > size()) throw std::invalid_argument("unique_ptr_vector cannot resize larger");
		vector_.resize(count);
	}
	
	void swap(unique_ptr_vector& other) { vector_.swap(other.vector_); }
};

template<typename T, typename Allocator>
void swap(unique_ptr_vector<T, Allocator>& a, unique_ptr_vector<T, Allocator>& b) {
	std::swap(a.base(), b.base());
}

}

#endif
