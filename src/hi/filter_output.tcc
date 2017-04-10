#include "filter_output.h"
#include "edge/filter_edge_base.h"
#include "../nd/nd.h"
#include <algorithm>

namespace tff {

template<std::size_t Output_dim, typename Output_elem>
void filter_output<Output_dim, Output_elem>::edge_has_connected(edge_base_type& edge) {
	edges_.push_back(edge);
}


template<std::size_t Output_dim, typename Output_elem>
void filter_output<Output_dim, Output_elem>::edge_has_disconnected(const filter_edge_base& edge) {
	auto cmp = [&edge](const edge_base_type& q_edge) { return (&edge == &q_edge); };
	auto it = std::find_if(edges_.begin(), edges_.end(), cmp);
	Assert(it != edges_.end());
	edges_.erase(it);
}


template<std::size_t Output_dim, typename Output_elem>
std::size_t filter_output<Output_dim, Output_elem>::edges_count() const {
	return edges_.size();
}


template<std::size_t Output_dim, typename Output_elem>
const filter_edge_base& filter_output<Output_dim, Output_elem>::edge_at(std::ptrdiff_t i) const {
	return edges_.at(i);
}
template<std::size_t Output_dim, typename Output_elem>
filter_edge_base& filter_output<Output_dim, Output_elem>::edge_at(std::ptrdiff_t i) {
	return edges_.at(i);
}

}
