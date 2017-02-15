#ifndef TFF_FILTER_OUTPUT_H_
#define TFF_FILTER_OUTPUT_H_

#include "../utility/ref_vector.h"
#include <string>

namespace tff {

class filter;
class filter_output_format;
class opaque_ndarray_format;


template<std::size_t Output_dim, typename Output_elem> class filter_edge_output_base;


class filter_output_base {
public:
	virtual opaque_ndarray_format ndarray_format(const filter_output_format&) const = 0;
};


template<std::size_t Output_dim, typename Output_elem>
class filter_output : public filter_output_base {
public:
	constexpr static std::size_t dimension = Output_dim;
	using elem_type = Output_elem;

	using edge_base_type = filter_edge_output_base<Output_dim, Output_elem>;
	
private:
	filter& filter_;
	std::string name_;
	
	ref_vector<edge_base_type> edges_;

public:
	explicit filter_output(filter& filt) :
		filter_(filt) { }
	
	filter& this_filter() { return filter_; }
	const filter& this_filter() const { return filter_; }

	void set_name(const std::string& name) { name_ = name; }
	const std::string& name() const { return name_; }

	opaque_ndarray_format ndarray_format(const filter_output_format&) const override;
};

}

#include "filter_output.tcc"

#endif
