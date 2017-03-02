#include <catch.hpp>

#include "../src/hi/filter_graph.h"
#include "../src/hi/processing/filter_box.h"
#include "../src/hi/processing/processing_filter.h"
#include "../src/hi/processing/processing_filter_job.h"

using namespace tff;

class test_filter : public filter_box {
	input<1, int> in;
	output<1, int> out;
	
	void setup() {
		out.define_frame_shape(in.frame_shape());
	}
	
	void process(processing_filter_job& job) {
		job.out(out) = job.in(in);
	}
};

TEST_CASE("filter") {
	filter_graph graph;
	
	auto& a = graph.add_processing_filter<test_filter>();
	auto& b = graph.add_processing_filter<test_filter>();
	b.in.connect(a.out);
	
}