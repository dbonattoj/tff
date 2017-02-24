#ifndef TFF_FILTER_INPUT_H_
#define TFF_FILTER_INPUT_H_

#include "../common.h"
#include "../nd/ndcoord.h"
#include <string>
#include <memory>

namespace tff {

class filter;
class filter_edge_base;
template<std::size_t Input_dim, typename Input_elem> class filter_edge_input_base;

class filter_output_base;
template<std::size_t Output_dim, typename Output_elem> class filter_output;

class filter_input_base {
public:
	virtual const std::string& name() const = 0;
	virtual time_window window() const = 0;

	virtual bool is_connected() const = 0;
	virtual filter_edge_base& edge() const = 0;
};


template<std::size_t Input_dim, typename Input_elem>
class filter_input : public filter_input_base {
public:
	constexpr static std::size_t dimension = Input_dim;
	using elem_type = Input_elem;
	
	using edge_base_type = filter_edge_input_base<Input_dim, Input_elem>;
	using frame_shape_type = ndsize<Input_dim>;
	
private:
	filter& filter_;
	std::string name_;
	std::unique_ptr<edge_base_type> edge_;
	time_window window_;
	
public:
	explicit filter_input(filter& filt) :
		filter_(filt) { }
	
	filter& this_filter() { return filter_; }
	const filter& this_filter() const { return filter_; }

	void set_name(const std::string& name) { name_ = name; }
	const std::string& name() const override { return name_; }
	
	void set_window(time_window win) { window_ = win; }
	time_window window() const { return window_; }
	
	bool is_connected() const override;
	const filter_edge_base& edge() const override;
	
	const frame_shape_type& frame_shape() const;
	
	void connect(filter_output<Input_dim, Input_elem>&);
	
	//template<std::size_t Output_dim, typename Output_elem>
	//void connect(filter_output<Output_dim, Output_elem>&);
};

}

#include "filter_input.tcc"

#endif