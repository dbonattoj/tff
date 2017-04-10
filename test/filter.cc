#include <catch.hpp>
#include <iostream>

#include "../src/hi/filter_graph.h"
#include "../src/hi/edge/filter_edge.h"
#include "../src/hi/processing/filter_box.h"
#include "../src/hi/processing/processing_filter.h"
#include "../src/hi/processing/processing_filter_job.h"
#include "../src/lo/diagnostic/node_graph_visualization.h"
#include "../src/hi/diagnostic/filter_graph_visualization.h"
#include "../src/hi/enclosure/enclosure_box.h"

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
		out.define_frame_shape(make_ndsize(1));
	}
	
	void process(processing_filter_job& job) {
		job.out(out) = job.in(in1);
	}
};

class IC : public enclosure_box {
public:
	input<1, int> in;
	output<1, int> out;
	
	IC() {
		auto& a = graph().add_processing_filter<Passthrough>(); a.set_name("a");
		auto& b = graph().add_processing_filter<Passthrough>(); b.set_name("b");
		
		connect_enclosure_input(in, a->in);
		connect_filters(a->out, b->in);
		connect_enclosure_output(b->out, out);
	}
};

TEST_CASE("filter") {
	filter_graph graph;
	
	auto& a = graph.add_processing_filter<Source>(); a.set_name("A");
	auto& b = graph.add_processing_filter<Tee>(); b.set_name("B");
	auto& c = graph.add_enclosure_filter<IC>(); c.set_name("C");
	auto& d = graph.add_processing_filter<Merge>(); d.set_name("D");
	auto& e = graph.add_enclosure_filter<IC>(); e.set_name("E");
	auto& f = graph.add_processing_filter<Merge>(); f.set_name("F");
	
	connect_filters(a->out, b->in);
	connect_filters(b->out2, d->in2);
	connect_filters(c->out, d->in1);
	d->in2.set_window(time_window(2, 0));
	connect_filters(b->out1, e->in);
	connect_filters(e->out, f->in1);
	connect_filters(d->out, f->in2);
	connect_filters(e->out, c->in);
	f.set_is_pulled(true);
	b.set_asynchronous(true);
	
	export_filter_graph_visualization(graph, "hi.gv");
	
	graph.setup();

	export_node_graph_visualization(graph.installed_node_graph(), "lo.gv");
	
	graph.run_for(10);
		
}
