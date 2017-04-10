#ifndef TFF_FILTER_EDGE_BASE_H_
#define TFF_FILTER_EDGE_BASE_H_

#include "hi/view_types.h"
#include "lo/types.h"
#include "nd/nd.h"
#include "hi/filter_graph_relation.h"

namespace tff {

class filter;

class filter_input_base;
class filter_output_base;
template<std::size_t Input_dim, typename Input_elem> class filter_input;
template<std::size_t Output_dim, typename Output_elem> class filter_output;


/// Edge connecting filters, base class.
class filter_edge_base {
public:
	virtual ~filter_edge_base() = default;
	
	virtual bool has_origin() const = 0;
	virtual const filter_output_base& origin() const;
	virtual filter_output_base& origin();
	const filter& origin_filter() const;
	filter& origin_filter();
	
	virtual bool has_destination() const = 0;
	virtual const filter_input_base& destination() const;
	virtual filter_input_base& destination();
	const filter& destination_filter() const;
	filter& destination_filter();
};


/// Edge connected to a filter's input, base class.
template<std::size_t Input_dim, typename Input_elem>
class filter_edge_input_base : public virtual filter_edge_base {
public:
	using input_type = filter_input<Input_dim, Input_elem>;
	using input_frame_shape_type = ndsize<Input_dim>;
	using input_full_view_type = filter_input_full_view<Input_dim, Input_elem>;
	
private:
	input_type& input_;
	
protected:
	explicit filter_edge_input_base(input_type& input) : input_(input) { }
	
public:
	bool has_destination() const override { return true; }
	const input_type& destination() const override { return input_; }
	input_type& destination() override { return input_; }
	
	virtual input_full_view_type input_view_from_opaque(const const_data_window_view_type&) const = 0;
	virtual input_frame_shape_type input_frame_shape() const = 0;
};


/// Edge connected to a filter's output, base class.
template<std::size_t Output_dim, typename Output_elem>
class filter_edge_output_base : public virtual filter_edge_base {
public:
	using output_type = filter_output<Output_dim, Output_elem>;
	using output_frame_shape_type = ndsize<Output_dim>;
	using output_full_view_type = filter_input_full_view<Output_dim, Output_elem>;
	
private:
	output_type& output_;
	
protected:
	explicit filter_edge_output_base(output_type& output) : output_(output) { }
	
public:
	bool has_origin() const override { return true; }
	const output_type& origin() const override { return output_; }
	output_type& origin() override { return output_; }
};


}

#endif
