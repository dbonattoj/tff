#include "filter_graph_visualization.h"
#include "../filter.h"
#include "../filter_output.h"
#include "../filter_input.h"
#include "../filter_edge.h"
#include "../filter_subgraph.h"

#include <fstream>

namespace tff {

void filter_graph_visualization::generate_filter_(std::ostream& str, const filter& filt) {
	const std::string& node_uid = uids_.uid(filt, "filter");
	str << '\t' << node_uid << " [shape=plaintext label=<\n";
	str << R"(<TABLE BORDER="0" CELLBORDER="0" CELLSPACING="0" CELLPADDING="0">)";

	// inputs
	str << R"(<TR><TD BORDER="0"><TABLE BORDER="0" CELLBORDER="0" CELLSPACING="0" CELLPADDING="0"><TR>)";
	str << R"(<TD WIDTH="20"></TD>)";
	for(const filter_input_base& in : filt.inputs()) {
		const std::string& input_uid = uids_.uid(in, "in");
		if(&in != &*filt.inputs().begin()) str << R"(<TD WIDTH="10"></TD>)";
		str << R"(<TD BORDER="1" CELLPADDING="1" PORT=")" << input_uid << R"(">)";
		str << R"(<FONT POINT-SIZE="10">)" << name_(in.name()) << R"(</FONT>)";
		str << R"(</TD>)";
	}
	str << R"(<TD WIDTH="20"></TD>)";
	str << R"(</TR></TABLE></TD></TR>)";
	
	// body
	str << R"(<TR><TD BORDER="1" CELLPADDING="4" COLOR="black">)";
	str << name_(filt.name());
	if(filt.is_pulled()) str << R"(<BR/><FONT POINT-SIZE="10">(pulled)</FONT>)";
	str << R"(</TD></TR>)";
	
	// outputs
	str << R"(<TR><TD BORDER="0"><TABLE BORDER="0" CELLBORDER="0" CELLSPACING="0" CELLPADDING="0"><TR>)";
	str << R"(<TD WIDTH="20"></TD>)";
	for(const filter_output_base& out : filt.outputs()) {
		const std::string& output_uid = uids_.uid(out, "out");
		if(&out != &*filt.outputs().begin()) str << R"(<TD WIDTH="10"></TD>)";
		str << R"(<TD BORDER="1" CELLPADDING="1" PORT=")" << output_uid << R"(">)";
		str << R"(<FONT POINT-SIZE="10">)" << name_(out.name()) << R"(</FONT>)";
		str << R"(</TD>)";
	}
	str << R"(<TD WIDTH="20"></TD>)";
	str << R"(</TR></TABLE></TD></TR>)";
	
	str << R"(</TABLE>)";
	str << "\n>];\n";
}

	
void filter_graph_visualization::generate_filter_input_connections_(std::ostream& str, const filter& filt) {
	for(const filter_input_base& in : filt.inputs()) {
		if(! in.is_connected()) continue;
		
		const filter_output_base& conn_out = in.edge().origin();
		
		const std::string& in_uid = uids_.uid(in, "in");
		const std::string& conn_out_uid = uids_.uid(conn_out, "out");
		
		const std::string& in_filter_uid = uids_.uid(in.this_filter(), "filter");
		const std::string& conn_out_filter_uid = uids_.uid(conn_out.this_filter(), "filter");

		std::string label = time_window_string_(in.window()) + "  ";

		std::string arrow_shape, style;
		arrow_shape = "normal";
		style = "";
		
		str << '\t' << conn_out_filter_uid << ':' << conn_out_uid << " -> " << in_filter_uid << ':' << in_uid << " [";
		str << "style=\"" << style << "\", ";
		str << "arrowhead=\"" << arrow_shape << "\", ";
		str << "headlabel=<" << label << ">, ";
		str << "fontsize=10, labelangle=45, labeldistance=2.0";
		str << "];\n";
	}
}


void filter_graph_visualization::generate_ranks_(std::ostream& str) {
	str << "\t{rank=sink;";
	for(const filter& filt : graph_.filters()) if(filt.is_pulled()) {
		str << ' ' << uids_.uid(filt, "filter");
	}
	str << "}\n";
}

	
std::string filter_graph_visualization::time_window_string_(const time_window& win) {
	if(win.past == 0 && win.future == 0) return "";
	else if(win.future == 0) return "[-" + std::to_string(win.past) + "]";
	else if(win.past == 0) return "[+" + std::to_string(win.future) + "]";
	else return "[-" + std::to_string(win.past) + ",+" + std::to_string(win.future) + "]";
}


std::string filter_graph_visualization::name_(const std::string& name) {
	if(name.empty()) return "&nbsp;&nbsp;&nbsp;&nbsp;";
	else return name;
}


filter_graph_visualization::filter_graph_visualization(const filter_subgraph& gr) :
	graph_(gr) { }


void filter_graph_visualization::generate(std::ostream& str) {
	str << "digraph " << graph_id_ << "{\n";
	str << "\trankdir=TB\n";
	for(const filter& filt : graph_.filters()) generate_filter_(str, filt);
	for(const filter& filt : graph_.filters()) generate_filter_input_connections_(str, filt);
	generate_ranks_(str);
	str << "}" << std::endl;
}


//////////


void export_filter_graph_visualization(const filter_subgraph& gr, const std::string& filename) {
	filter_graph_visualization vis(gr);
	std::ofstream str(filename);
	vis.generate(str);
}

}