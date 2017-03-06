#ifndef TFF_FILTER_GRAPH_VISUALIZATION_H_
#define TFF_FILTER_GRAPH_VISUALIZATION_H_

#include <iosfwd>
#include <string>
#include "../../utility/uid_pool.h"
#include "../../common.h"

namespace tff {

class filter_subgraph;
class filter;
class processing_node;

class filter_graph_visualization {
private:
	const filter_subgraph& graph_;
	std::string graph_id_ = "G";
	uid_pool uids_;
	
	void generate_filter_(std::ostream&, const filter&);
	
	void generate_filter_input_connections_(std::ostream&, const filter&);
	void generate_ranks_(std::ostream&);
	void generate_pulled_(std::ostream&);
	
	static std::string name_(const std::string&);
	static std::string time_window_string_(const time_window&);
	
public:
	explicit filter_graph_visualization(const filter_subgraph&);
	
	void generate(std::ostream&);
};


void export_filter_graph_visualization(const filter_subgraph&, const std::string& filename);

}

#endif
