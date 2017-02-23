#ifndef TFF_FILTER_EDGE_H_
#define TFF_FILTER_EDGE_H_

#include "view_types.h"
#include "../lo/types.h"
#include "../nd/nd.h"

namespace tff {

class filter;

class filter_input_base;
class filter_output_base;
template<std::size_t Input_dim, typename Input_elem> class filter_input;
template<std::size_t Output_dim, typename Output_elem> class filter_output;


class filter_edge_base {
public:
	virtual ~filter_edge_base() = 0;
	virtual filter_output_base& origin() const = 0;
	virtual filter& origin_filter() const = 0;
	virtual filter_input_base& destination() const = 0;
	virtual filter& destination_filter() const = 0;
};


template<std::size_t Input_dim, typename Input_elem>
class filter_edge_input_base : public virtual filter_edge_base {
public:
	using input_type = filter_input<Input_dim, Input_elem>;
	using input_frame_shape_type = ndsize<Input_dim>;
	using input_full_view_type = filter_input_full_view<Input_dim, Input_elem>;
	
	virtual input_full_view_type input_view_from_opaque(const input_ndarray_window_view_type&) const = 0;
	virtual input_frame_shape_type input_frame_shape() const = 0;
};


template<std::size_t Output_dim, typename Output_elem>
class filter_edge_output_base : public virtual filter_edge_base {
public:
	using output_type = filter_input<Output_dim, Output_elem>;
	using output_frame_shape_type = ndsize<Output_dim>;
	using output_full_view_type = filter_input_full_view<Output_dim, Output_elem>;
};


template<
	std::size_t Output_dim,
	typename Output_elem,
	std::size_t Input_dim,
	typename Input_elem,
	typename Caster
>
class filter_edge :
	public filter_edge_input_base<Input_dim, Input_elem>,
	public filter_edge_output_base<Output_dim, Output_elem>
{
	using base_in = filter_edge_input_base<Input_dim, Input_elem>;
	using base_out = filter_edge_output_base<Output_dim, Output_elem>;
	
	/*
	origin node
	 [output]
		|
		|  ndarray_view_cast
		v
	 [input]
	destination node
	*/

public:
	using typename base_in::input_type;
	using typename base_in::input_frame_shape_type;
	using typename base_in::input_full_view_type;
	using typename base_out::output_type;
	using typename base_out::output_frame_shape_type;
	using caster_type = Caster;
	
private:
	input_type& input_;
	output_type& output_;
	caster_type caster_;
	
	static input_full_view_type cast_view_(const output_full_view_type&);
	static input_frame_shape_type cast_frame_shape_(const output_frame_shape_type&);
	
protected:
	filter_edge(input_type& in, output_type& out) : input_(in), output_(out) { }
	
public:
	output_type& origin() const override { return output_; }
	filter& origin_filter() const override { return output_.this_filter(); }

	input_type& destination() const override { return input_; }
	filter& destination_filter() const override { return input_.this_filter(); }

	input_full_view_type input_view_from_opaque(const input_ndarray_window_view_type&) const override;
	input_frame_shape_type input_frame_shape() const override;
};


}

#endif
