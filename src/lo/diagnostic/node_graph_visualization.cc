#include "node_graph_visualization.h"

namespace tff {

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
	str << "\n>];\n\n";
}


void node_graph_visualization::generate_processing_node_body_(std::ostream& str, const processing_node& nd) {
	str << "Node";
}

	
void node_graph_visualization::generate_node_input_connections_(std::ostream& str, const node& nd) const {
	
}


void node_graph_visualization::generate_node_request_receiver_connections_(std::ostream& str, const node& nd) const {
	
}


void node_graph_visualization::generate_ranks_(std::ostream& str) const {
	
}

	
static std::string node_graph_visualization::time_window_string_(const time_window& win) {
	
}


static std::string node_graph_visualization::thread_index_color_(thread_index_type th) const {
	
}


static std::string node_graph_visualization::filter_name_(const std::string& name) {
	if(name.empty()) return "&nbsp;&nbsp;&nbsp;&nbsp;";
	else return name;
}


node_graph_visualization::node_graph_visualization(const node_graph& gr) :
	graph_(gr) { }


void node_graph_visualization::generate(std::ostream& str) {
	str << "digraph " << graph_id_ << "{\n";
	str << "rankdir=TB\n";
	for(const node& nd : graph_.nodes()) generate_node_(str, nd);
	for(const node& nd : graph_.nodes()) generate_node_input_connections_(str, nd);
	for(const node& nd : graph_.nodes()) generate_node_request_receiver_connections_(str, nd);
	generate_ranks_(str_);
	str << "}" << std::endl;
}


}