#include <catch.hpp>
#include <iostream>

#include "../src/hi/filter_graph.h"
#include "../src/hi/processing/filter_box.h"
#include "../src/hi/processing/processing_filter.h"
#include "../src/hi/processing/processing_filter_job.h"
#include "../src/lo/diagnostic/node_graph_visualization.h"
#include "../src/hi/diagnostic/filter_graph_visualization.h"

using namespace tff;


class Source : public filter_box {
public:
	output<1, int> out;
	
	void setup() {
		std::cout << "setup Source" << std::endl;
		out.define_frame_shape(make_ndsize(1));
	}
	
	void process(processing_filter_job& job) {
		job.out(out)[0] = job.time();
	}
};

class Tee : public filter_box {
public:
	input<1, int> in;
	output<1, int> out1;
	output<1, int> out2;
	
	void setup() {
		std::cout << "setup Tee" << std::endl;
		out1.define_frame_shape(in.frame_shape());
		out2.define_frame_shape(in.frame_shape());
	}
	
	void process(processing_filter_job& job) {
		job.out(out1) = job.in(in);
		job.out(out2) = job.in(in);
	}
};

class Passthrough : public filter_box {
public:
	input<1, int> in;
	output<1, int> out;
	
	void setup() {
		std::cout << "setup Passthrough" << std::endl;
		if(in.is_connected()) out.define_frame_shape(in.frame_shape());
		else out.define_frame_shape(make_ndsize(1));
	}
	
	void process(processing_filter_job& job) {
		job.out(out) = job.in(in);
	}
};

class Merge : public filter_box {
public:
	input<1, int> in1;
	input<1, int> in2;
	output<1, int> out;
	
	void setup() {
		std::cout << "setup Merge" << std::endl;
		out.define_frame_shape(in1.frame_shape());
	}
	
	void process(processing_filter_job& job) {
		job.out(out) = job.in(in1);
	}
};

TEST_CASE("filter") {
	filter_graph graph;
	
	auto& a = graph.add_processing_filter<Source>(); a.set_name("A");
	auto& b = graph.add_processing_filter<Passthrough>(); b.set_name("B");
	b->in.connect(a->out);
	b.set_is_pulled(true);
	
	graph.setup();

	graph.run_for(10);
	
	export_filter_graph_visualization(graph, "hi.gv");
	export_node_graph_visualization(graph.installed_node_graph(), "lo.gv");
}
