#include <catch.hpp>
#include <iostream>

#include "../src/hi/filter_graph.h"
#include "../src/hi/processing/filter_box.h"
#include "../src/hi/processing/processing_filter.h"
#include "../src/hi/processing/processing_filter_job.h"

using namespace tff;

class A : public filter_box {
public:
	output<1, int> out;
	
	void setup() {
		std::cout << "setup A" << std::endl;
		out.define_frame_shape(make_ndsize(1));
	}
	
	void process(processing_filter_job& job) {
		job.out(out)[0] = job.time();
	}
};

class B : public filter_box {
public:
	input<1, int> in;
	
	void setup() {
		std::cout << "setup B" << std::endl;
	}
	
	void process(processing_filter_job& job) {
		std::cout << "A got " << job.in(in)[0] << std::endl;
	}
};

TEST_CASE("filter") {
	filter_graph graph;
	
	auto& a = graph.add_processing_filter<A>();
	a.set_name("A");
	
	auto& b = graph.add_processing_filter<B>();
	b.set_name("B");
	
	b->in.connect(a->out);
	b.set_is_sink(true);
	
	graph.setup();

	graph.run_for(10);
}
