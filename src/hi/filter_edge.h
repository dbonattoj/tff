#ifndef TFF_FILTER_EDGE_H_
#define TFF_FILTER_EDGE_H_

#include "types.h"
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
	using input_full_view_type = input_frame_window_view_type<Input_dim, Input_elem>;
	
	virtual const input_frame_shape_type& input_frame_shape() const = 0;
};


template<std::size_t Output_dim, typename Output_elem>
class filter_edge_output_base : public virtual filter_edge_base {
public:
	using output_type = filter_input<Output_dim, Output_elem>;
	using output_frame_shape_type = ndsize<Output_dim>;
};


template<std::size_t Dim, typename Output_elem, typename Casted_elem, typename Input_elem>
class filter_edge :
	public filter_edge_input_base<Dim, Input_elem>,
	public filter_edge_output_base<Dim, Output_elem>
{
	using base_in = filter_edge_input_base<Dim, Input_elem>;
	using base_out = filter_edge_output_base<Dim, Output_elem>;
	
public:
	using typename base_in::input_type;
	using typename base_in::input_frame_shape_type;
	using typename base_in::input_full_view_type;
	using typename base_out::output_type;
	using typename base_out::output_frame_shape_type;

private:
	input_type& input_;
	output_type& output_;
	
protected:
	filter_edge(input_type& in, output_type& out) : input_(in), output_(out) { }
	
public:
	~filter_edge() override = default;
	
	output_type& origin() const override { return output_; }
	filter& origin_filter() const override { return output_.this_filter(); }
	const input_frame_shape_type& input_frame_shape() const override { return output_.frame_shape(); }

	input_type& destination() const override { return input_; }
	filter& destination_filter() const override { return input_.this_filter(); }
};


template<std::size_t Dim, typename Output_elem, typename Input_elem>
class filter_direct_edge final :
	public filter_edge<Dim, Output_elem, Input_elem, Input_elem>
{
	using base = filter_edge<Dim, Output_elem, Input_elem, Input_elem>;

	/*
	origin node
	 [output_]   (Output_elem)
		|
		|  ndarray_view_cast: Output_elem -> Casted_elem
		v
	 [input_]    (Input_elem = Casted_elem)
	destination node
	*/

public:
	using typename base::input_type;
	using typename base::input_frame_shape_type;
	using typename base::input_full_view_type;
	using typename base::output_type;
	using typename base::output_frame_shape_type;

	filter_direct_edge(input_type& in, output_type& out) : base(in, out) { }
};


template<std::size_t Dim, typename Output_elem, typename Casted_elem, typename Input_elem, typename Convert_function>
class filter_converting_edge final :
	public filter_edge<Dim, Output_elem, Casted_elem, Input_elem>
{
	using base = filter_edge<Dim, Output_elem, Casted_elem, Input_elem>;

	/*
	origin node
	 [output_]   (Output_type)
		|
		|  ndarray_view_cast: Output_type -> Casted_type
		v
	 [in]        (Casted_type)
	converter node: Input_type = Convert_function(Casted_type)
	 [out]       (Input_type)
		|
		|  direct
		v
	 [input_]    (Input_type)
	destination node
	*/

public:
	using typename base::input_type;
	using typename base::input_frame_shape_type;
	using typename base::input_full_view_type;
	using typename base::output_type;
	using typename base::output_frame_shape_type;
	
private:
	Convert_function convert_function_;

public:
	filter_converting_edge(input_type& in, output_type& out, Convert_function&& func) :
		base(in, out), convert_function_(std::forward<Convert_function>(func)) { }

};

}

#endif
