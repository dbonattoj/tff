#include <catch.hpp>
#include "utility/unique_ptr_vector.h"

using namespace tff;

struct A_nomove {
	int i;
	
	A_nomove(const A_nomove&) = delete;
	A_nomove(A_nomove&&) = delete;
	A_nomove& operator=(const A_nomove&) = delete;
	A_nomove& operator=(A_nomove&&) = delete;
	
	explicit A_nomove(int i_ = 3) : i(i_) { }
	virtual ~A_nomove() = default;
	virtual bool is_b() const { return false; }
};

struct B_nomove : public A_nomove {
	explicit B_nomove(int i_ = 5) : A_nomove(i_) { }
	
	bool is_b() const override { return true; }
};

TEST_CASE("unique_ptr_vector") {
	unique_ptr_vector<A_nomove> vec;
	REQUIRE(vec.size() == 0);
	
	SECTION("...") {
		B_nomove& b = vec.emplace_back<B_nomove>(123);
		REQUIRE(vec.size() == 1);
		REQUIRE(b.i == 123);
		REQUIRE(vec[0].is_b());
		REQUIRE(vec[0].i == 123);
	}
	
	SECTION("iteration") {
		for(int i = 0; i < 10; ++i) vec.emplace_back(i);
		
		int i = 0;
		for(A_nomove& a : vec) REQUIRE(a.i == i++);
		i = 0;
		for(const A_nomove& a : vec) REQUIRE(a.i == i++);
	}
}