#include "node_graph_visualization.h"
#include "../node_graph.h"
#include "../node.h"
#include "../node_input.h"
#include "../node_output.h"
#include "../node_request_connection.h"
#include "../processing/processing_node.h"

namespace tff {

namespace {
	std::vector<std::string> html_colors_ {
		"blue",
		"red",
		"forestgreen",
		"orange",
		"deeppink",
		"turquoise",
		"salmon",
		"lightskyblue3",
		"wheat4",
		"yellowgreen",
		"gray33",
		"indigo"
	};
};

void node_graph_visualization::generate_node_(std::ostream& str, const node& nd) {
	const std::string& node_uid = uids_.uid(nd, "node");
	str << '\t' << node_uid << " [shape=plaintext label=<\n";
	str << R"(<TABLE BORDER="0" CELLBORDER="0" CELLSPACING="0" CELLPADDING="0">)";

	str << R"(<TR><TD BORDER="0"><TABLE BORDER="0" CELLBORDER="0" CELLSPACING="0" CELLPADDING="0"><TR>)";
	str << R"(<TD WIDTH="20"></TD>)";
	for(const node_input& in : nd.inputs()) {
		const std::string& input_uid = uids_.uid(in, "in");
		if(in != nd.inputs().begin()) str << R"(<TD WIDTH="10"></TD>)";
		str << R"(<TD BORDER="1" CELLPADDING="1">)";
		str << R"(<FONT POINT-SIZE="10">)" << filter_name_(in.name()) << R"(</FONT>)";
		str << R"(</TD>)";
	}
	str << R"(<TD WIDTH="20"></TD>)";
	str << R"(</TR></TABLE></TD></TR>)";
	
	str << R"(<TR><TD BORDER="1" STYLE="ROUNDED" CELLPADDING="4" COLOR="black">)";
	if(const auto* proc_nd = dynamic_cast<const processing_node*>(&nd)) generate_processing_node_body_(str, *proc_nd);
	else throw std::runtime_error("unknown node type");
	str << R"(</TD></TR>)";
	
	str << R"(<TR><TD BORDER="0"><TABLE BORDER="0" CELLBORDER="0" CELLSPACING="0" CELLPADDING="0"><TR>)";
	str << R"(<TD WIDTH="20"></TD>)";
	for(const node_input& out : nd.outputs()) {
		const std::string& output_uids = uids_.uid(out, "out");
		if(in != nd.outputs().begin()) str << R"(<TD WIDTH="10"></TD>)";
		str << R"(<TD BORDER="1" CELLPADDING="1">)";
		str << R"(<FONT POINT-SIZE="10">)" << filter_name_(out.name()) << R"(</FONT>)";
		str << R"(</TD>)";
	}
	str << R"(<TD WIDTH="20"></TD>)";
	str << R"(</TR></TABLE></TD></TR>)";
	
	str << R"(</TABLE>)";
	str << "\n>];\n";
}


void node_graph_visualization::generate_processing_node_body_(std::ostream& str, const processing_node& nd) {
	str << "Node";
}

	
void node_graph_visualization::generate_node_input_connections_(std::ostream& str, const node& nd) const {
	for(const node_input& in : nd.inputs()) {
		const node_output& conn_out = in.connected_output();
		
		const std::string& in_uid = uid_.uid(in, "in");
		const std::string& conn_out_uid = uid_.uid(conn_out, "out");
		
		const std::string& in_node_uid = uid_.uid(in.this_node(), "node");
		const std::string& conn_out_node_uid = uid_.uid(conn_out.this_node(), "node");

		std::string label = time_window_string_(in.window()) + "  ";

		std::string arrow_shape, style;
		arrow_shape = "normal";
		style = "";

		str << '\t' << in_node_uid << ':' << in_uid << " -> " << conn_out_uid << ':' << conn_out_node_uid << " [";
		str << "style=\"" << style << "\", ";
		str << "arrowhead=\"" << arrow_shape << "\", ";
		str << "headlabel=<" << label << ">, ";
		str << "fontsize=10, labelangle=45, labeldistance=2.0";
		str << "];\n";
	}
}


void node_graph_visualization::generate_node_request_receiver_connections_(std::ostream& str, const node& nd) const {
	for(const node_request_connection& req : nd.request_receivers()) {
		std::string sender_nd_uid = uid_.uid(req.sender(), "node");
		std::string receiver_nd_uid = uid_.uid(req.receiver(), "node");
		std::string label = time_window_string_(req.window()) + "  ";

		std::string arrow_shape, style;
		arrow_shape = "vee";
		style = "";
		
		str << '\t' << sender_nd_uid << ":ne" << " -> " << receiver_nd_uid << ":se [";
		str << "style=\"" << style << "\", ";
		str << "arrowhead=\"" << arrow_shape << "\", ";
		str << "headlabel=<" << label << ">, ";
		str << "fontsize=10, labelangle=45, labeldistance=2.0";
		str << "];\n";
	}
}


void node_graph_visualization::generate_ranks_(std::ostream& str) const {
	
}

	
static std::string node_graph_visualization::time_window_string_(const time_window& win) {
	if(win.past == 0 && win.future == 0) return "";
	else if(win.past != 0) return "[-" + std::to_string(win.past) << "]";
	else if(win.future != 0) return "[+" + std::to_string(win.future) << "]";
	else return "[-" + std::to_string(win.past) << ",+" << std::to_string(win.future) << "]";
}


static std::string node_graph_visualization::thread_index_color_(thread_index_type tid) const {
	if(! thread_index_colors_) return "black";
	else if(tid == graph_.root_thread_index()) return "black";
	else return html_colors_.at( (tid - 1) % html_colors_.size() );
}


static std::string node_graph_visualization::filter_name_(const std::string& name) {
	if(name.empty()) return "&nbsp;&nbsp;&nbsp;&nbsp;";
	else return name;
}


node_graph_visualization::node_graph_visualization(const node_graph& gr) :
	graph_(gr) { }


void node_graph_visualization::generate(std::ostream& str) {
	str << "digraph " << graph_id_ << "{\n";
	str << "\trankdir=TB\n";
	for(const node& nd : graph_.nodes()) generate_node_(str, nd);
	for(const node& nd : graph_.nodes()) generate_node_input_connections_(str, nd);
	for(const node& nd : graph_.nodes()) generate_node_request_receiver_connections_(str, nd);
	generate_ranks_(str_);
	str << "}" << std::endl;
}


}