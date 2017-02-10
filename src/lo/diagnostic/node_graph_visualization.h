#ifndef TFF_NODE_GRAPH_VISUALIZATION_H_
#define TFF_NODE_GRAPH_VISUALIZATION_H_

#include <iosfwd>
#include <string>
#include "../../utility/uid_pool.h"

namespace mf { namespace flow {

class node_graph;
class processing_node;

class node_graph_visualization {
private:
	const node_graph& graph_;
	std::string graph_id_ = "G";
	uid_pool uids_;

	bool thread_index_colors_ = false;
	bool with_state_ = true;
	
	void generate_node_(std::ostream&, const node&);
	void generate_processing_node_body_(std::ostream&, const processing_node&);
	
	void generate_node_input_connections_(std::ostream&, const node&) const;
	void generate_node_request_receiver_connections_(std::ostream&, const node&) const;
	void generate_ranks_(std::ostream&) const;
	
	static std::string filter_name_(const std::string&);
	static std::string time_window_string_(const time_window&);
	static std::string thread_index_color_(thread_index_type);
	
public:
	explicit node_graph_visualization(const node_graph&);
	
	void generate(std::ostream&);
};


void export_node_graph_visualization(const node_graph&, const std::string& filename);

}}

#endif
