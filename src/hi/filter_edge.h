#ifndef TFF_FILTER_EDGE_H_
#define TFF_FILTER_EDGE_H_

#include "view_types.h"
#include "../lo/types.h"
#include "../nd/nd.h"
#include "filter_graph_relation.h"

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


/// Edge connecting a filter output (_origin_) to a filter input (_destination_).
template<
	std::size_t Output_dim,
	typename Output_elem,
	std::size_t Input_dim,
	typename Input_elem,
	typename Caster
>
class filter_edge final :
	public filter_edge_output_base<Output_dim, Output_elem>,
	public filter_edge_input_base<Input_dim, Input_elem>
{
	using base_in = filter_edge_input_base<Input_dim, Input_elem>;
	using base_out = filter_edge_output_base<Output_dim, Output_elem>;
	
	/*
	origin filter
	 [output]
		|
		|  cast
		v
	 [input]
	destination filter
	*/

public:
	using typename base_out::output_type;
	using typename base_out::output_frame_shape_type;
	using typename base_out::output_full_view_type;
	using typename base_in::input_type;
	using typename base_in::input_frame_shape_type;
	using typename base_in::input_full_view_type;
	using caster_type = Caster;
	
private:
	caster_type caster_;
	
public:
	filter_edge(output_type& out, input_type& in, const caster_type& caster = caster_type()) :
		base_out(out), base_in(in), caster_(caster)
	{
		if(&out.this_filter().graph() != &in.this_filter().graph())
			throw invalid_filter_graph("filter edge must be between filters in same subgraph");
		
		if(succeedes(out.this_filter(), in.this_filter()))
			throw invalid_filter_graph("filter edge origin filter must not succeede destination filter");
	}
	
	input_full_view_type input_view_from_opaque(const const_data_window_view_type& out_opaque_vw) const override {
		output_full_view_type out_vw = from_opaque<Output_dim + 1, const Output_elem>(out_opaque_vw);
		return caster_.cast_view(out_vw);
	}
	
	input_frame_shape_type input_frame_shape() const override {
		output_frame_shape_type output_shp = base_out::origin().frame_shape();
		return caster_.cast_frame_shape(output_shp);
	}
};


}

#endif
