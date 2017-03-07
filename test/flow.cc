#include <catch.hpp>
#include <iostream>
#include "../src/lo/node_graph.h"
#include "../src/lo/node_input.h"
#include "../src/lo/node_output.h"
#include "../src/lo/processing/async_node.h"
#include "../src/lo/processing/sync_node.h"
#include "../src/lo/processing/processing_handler.h"
#include "../src/lo/processing/processing_job.h"
#include "../src/lo/sink_node.h"

#include "../src/lo/diagnostic/node_graph_visualization.h"

#include <mutex>

using namespace tff;

static std::mutex out_lock;

TEST_CASE("flow") {
	class handler : public processing_handler {
	private:
		std::string name_;
		std::size_t inputs_;
		
	public:
		time_unit end_time = -1;

		handler(const std::string& nm, std::size_t inputs) :
			name_(nm), inputs_(inputs) { }
		
		static const int& in(processing_job& job, std::ptrdiff_t idx) {
			return *reinterpret_cast<const int*>(job.input_view(idx).at_time(job.time()).start());
		}
		static int& chan(processing_job& job, std::ptrdiff_t idx) {
			return *reinterpret_cast<int*>(job.data_channel_view(idx).start());
		}
		
		void handler_pre_process_(processing_job&) override { }
		
		void handler_process_(processing_job& job) override {
			std::lock_guard<std::mutex> lock(out_lock);
		//	std::cout << name_ << " does " << job.time() << std::endl;
			if(end_time != -1 && job.time() >= end_time) {
				job.mark_end_of_stream();
				return;
			}
			for(std::ptrdiff_t i = 0; i < inputs_; ++i) if(in(job, i) != job.time()) {
				std::cerr << name_ << " failed" << std::endl;
				std::terminate();
			}
			chan(job, 0) = job.time();
		}
	};
	
	opaque_ndarray_format frm = default_opaque_ndarray_format<int>(make_ndsize(1));
	
	node_graph gr;
	
	auto& Sin1 = gr.sink().add_input();
	auto& Sin2 = gr.sink().add_input();
	auto& Sin3 = gr.sink().add_input();

	auto& A = gr.add_node<sync_node>("A");
	handler Ahand("A", 1);
	A.set_handler(Ahand);
	auto& Ain = A.add_input();
	data_channel_index_type Achan = A.add_data_channel(frm);
	auto& Aout = A.add_data_output(Achan);

	auto& B = gr.add_node<async_node>("B");
	handler Bhand("B", 3);
	B.set_handler(Bhand);
	auto& Bin1 = B.add_input();
	auto& Bin2 = B.add_input();
	auto& Bin3 = B.add_input();
	data_channel_index_type Bchan = B.add_data_channel(frm);
	auto& Bout = B.add_data_output(Bchan);

	auto& C = gr.add_node<async_node>("C");
	handler Chand("C", 1);
	C.set_handler(Chand);
	auto& Cin1 = C.add_input();
	data_channel_index_type Cchan = C.add_data_channel(frm);
	auto& Cout1 = C.add_data_output(Cchan);
	auto& Cout2 = C.add_data_output(Cchan);

	auto& D = gr.add_node<sync_node>("D");
	handler Dhand("D", 2);
	D.set_handler(Dhand);
	auto& Din1 = D.add_input();
	auto& Din2 = D.add_input();
	data_channel_index_type Dchan = D.add_data_channel(frm);
	auto& Dout = D.add_data_output(Bchan);

	auto& F = gr.add_node<async_node>("F");
	handler Fhand("F", 0);
	F.set_handler(Fhand);
	data_channel_index_type Fchan = F.add_data_channel(frm);
	auto& Fout1 = F.add_data_output(Fchan);
	auto& Fout2 = F.add_data_output(Fchan);
	auto& Fout3 = F.add_data_output(Fchan);

	auto& E = gr.add_node<sync_node>("E");
	handler Ehand("E", 1);
	E.set_handler(Ehand);
	auto& Ein = E.add_input();
	data_channel_index_type Echan = E.add_data_channel(frm);
	auto& Eout = E.add_data_output(Echan);
	
	auto& G = gr.add_node<sync_node>("G");
	handler Ghand("G", 0);
	G.set_handler(Ghand);
	data_channel_index_type Gchan = G.add_data_channel(frm);
	auto& Gout1 = G.add_data_output(Gchan);
	auto& Gout2 = G.add_data_output(Gchan);

	Sin1.set_window(time_window(1, 2));
	Ain.set_window(time_window(3, 0));
	Bin2.set_window(time_window(0, 10));
	
	Sin1.connect(Aout);
	Sin2.connect(Bout);
	Ain.connect(Cout1);
	Bin1.connect(Cout2);
	Bin2.connect(Dout);
	Bin3.connect(Eout);
	Cin1.connect(Fout1);
	Din1.connect(Fout2);
	Din2.connect(Gout1);
	Ein.connect(Gout2);
	Sin3.connect(Fout3);
	
	Fhand.end_time = 100;
	
	gr.setup();
	
	export_node_graph_visualization(gr, "lo.gv");

	gr.run();
	gr.seek(50);
	gr.run_for(10);
	gr.seek(70);
	gr.run();
}