#include <catch.hpp>
#include "../src/lo/node_graph.h"
#include "../src/lo/node_input.h"
#include "../src/lo/node_output.h"
#include "../src/lo/processing/sync_node.h"
#include "../src/lo/processing/sink_node.h"

#include "../src/lo/diagnostic/node_graph_visualization.h"

using namespace tff;

TEST_CASE("flow") {
	opaque_ndarray_format frm(1, 1, 1, true, make_ndsize_dyn(100));
	
	node_graph gr;
	sink_node& S = gr.add_sink_node<sink_node>("S");
	node_input& Sin1 = S.add_input();
	node_input& Sin2 = S.add_input();

	sync_node& A = gr.add_node<sync_node>("A");
	node_input& Ain = A.add_input();
	channel_index_type Achan = A.add_channel(frm);
	node_output& Aout = A.add_output(Achan);

	sync_node& B = gr.add_node<sync_node>("B");
	node_input& Bin = B.add_input();
	channel_index_type Bchan = B.add_channel(frm);
	node_output& Bout = B.add_output(Bchan);

	sync_node& M = gr.add_node<sync_node>("M");
	channel_index_type Mchan = M.add_channel(frm);
	node_output& Mout1 = M.add_output(Mchan);
	node_output& Mout2 = M.add_output(Mchan);
	
	Sin1.connect(Aout);
	Sin2.connect(Bout);
	Ain.connect(Mout1);
	Bin.connect(Mout2);
	
	gr.setup();
	
	export_node_graph_visualization(gr, "lo.gv");
}