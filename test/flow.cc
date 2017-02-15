#include <catch.hpp>
#include "../src/lo/node_graph.h"
#include "../src/lo/node_input.h"
#include "../src/lo/node_output.h"
#include "../src/lo/processing/async_node.h"
#include "../src/lo/processing/sync_node.h"
#include "../src/lo/processing/sink_node.h"

#include "../src/lo/diagnostic/node_graph_visualization.h"

using namespace tff;

TEST_CASE("flow") {
	opaque_ndarray_format frm(1, 1, 1, true, make_ndsize_dyn(100));
	
	node_graph gr;
	
	auto& S = gr.add_sink_node<sink_node>("S");
	auto& Sin1 = S.add_input();
	auto& Sin2 = S.add_input();

	auto& A = gr.add_node<sync_node>("A");
	auto& Ain = A.add_input();
	channel_index_type Achan = A.add_channel(frm);
	auto& Aout = A.add_output(Achan);

	auto& B = gr.add_node<async_node>("B");
	auto& Bin1 = B.add_input();
	auto& Bin2 = B.add_input();
	auto& Bin3 = B.add_input();
	channel_index_type Bchan = B.add_channel(frm);
	auto& Bout = B.add_output(Bchan);

	auto& C = gr.add_node<async_node>("C");
	auto& Cin = C.add_input();
	channel_index_type Cchan = C.add_channel(frm);
	auto& Cout1 = C.add_output(Cchan);
	auto& Cout2 = C.add_output(Cchan);

	auto& D = gr.add_node<sync_node>("D");
	auto& Din1 = D.add_input();
	auto& Din2 = D.add_input();
	channel_index_type Dchan = D.add_channel(frm);
	auto& Dout = D.add_output(Bchan);

	auto& F = gr.add_node<async_node>("F");
	channel_index_type Fchan = F.add_channel(frm);
	auto& Fout1 = F.add_output(Fchan);
	auto& Fout2 = F.add_output(Fchan);

	auto& E = gr.add_node<sync_node>("E");
	auto& Ein = E.add_input();
	channel_index_type Echan = E.add_channel(frm);
	auto& Eout = E.add_output(Echan);
	
	auto& G = gr.add_node<sync_node>("G");
	channel_index_type Gchan = G.add_channel(frm);
	auto& Gout1 = G.add_output(Gchan);
	auto& Gout2 = G.add_output(Gchan);

	Sin1.set_window(time_window(1, 2));
	Ain.set_window(time_window(3, 0));
	Bin2.set_window(time_window(0, 10));
	
	Sin1.connect(Aout);
	Sin2.connect(Bout);
	Ain.connect(Cout1);
	Bin1.connect(Cout2);
	Bin2.connect(Dout);
	Bin3.connect(Eout);
	Cin.connect(Fout1);
	Din1.connect(Fout2);
	Din2.connect(Gout1);
	Ein.connect(Gout2);
	
	gr.setup();
	
	gr.launch();
	
	export_node_graph_visualization(gr, "lo.gv");
}