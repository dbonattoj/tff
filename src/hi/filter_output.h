#ifndef TFF_FILTER_OUTPUT_H_
#define TFF_FILTER_OUTPUT_H_

#include "../utility/ref_vector.h"
#include <string>

namespace tff {

class filter;
class filter_input_base;
class filter_output_format;
class opaque_ndarray_format;


class filter_edge_base;
template<std::size_t Output_dim, typename Output_elem> class filter_edge_output_base;


class filter_output_base {
private:
	filter_output_base(const filter_output_base&) = delete;
	filter_output_base& operator=(const filter_output_base&) = delete;

public:
	virtual const std::string& name() const = 0;
	
	virtual std::size_t edges_count() const = 0;
	virtual const filter_edge_base& edge_at(std::ptrdiff_t) const = 0;
	
	virtual opaque_ndarray_format ndarray_format() const = 0;
};


template<std::size_t Output_dim, typename Output_elem>
class filter_output : public filter_output_base {
public:
	constexpr static std::size_t dimension = Output_dim;
	using elem_type = Output_elem;

	using edge_base_type = filter_edge_output_base<Output_dim, Output_elem>;
	using frame_shape_type = ndsize<Output_dim>;
	
private:
	filter& filter_;
	std::string name_;
	
	ref_vector<edge_base_type> edges_;
	
	optional<frame_shape_type> frame_shape_;
	
public:
	explicit filter_output(filter& filt) :
		filter_(filt) { }
	
	filter& this_filter() { return filter_; }
	const filter& this_filter() const { return filter_; }

	void set_name(const std::string& name) { name_ = name; }
	const std::string& name() const override { return name_; }
	
	auto& edges() { return edges_; }
	const auto& edges() const { return edges_; }
	std::size_t edges_count() const override;
	const filter& connected_filter_at_edge(std::ptrdiff_t i) const override;
	void edge_has_connected(edge_base_type&);
	
	void define_frame_shape(const frame_shape_type&);
	bool frame_shape_is_defined() const;
	const frame_shape_type& frame_shape();

	opaque_ndarray_format data_format() const override;
};

}

#include "filter_output.tcc"

#endif
