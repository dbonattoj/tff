#ifndef TFF_FILTER_INPUT_H_
#define TFF_FILTER_INPUT_H_

#include "../common.h"
#include "../nd/ndcoord.h"
#include "../lo/types.h"
#include "../utility/exception.h"
#include <string>
#include <memory>

namespace tff {

class filter;
class filter_edge_base;
template<std::size_t Input_dim, typename Input_elem> class filter_edge_input_base;

class filter_output_base;
template<std::size_t Output_dim, typename Output_elem> class filter_output;

class filter_input_base {
private:
	filter& filter_;
	std::string name_;
	time_window window_;

	filter_input_base(const filter_input_base&) = delete;
	filter_input_base& operator=(const filter_input_base&) = delete;
	
protected:
	explicit filter_input_base(filter&);
	
public:
	virtual ~filter_input_base() = default;
	
	filter& this_filter() { return filter_; }
	const filter& this_filter() const { return filter_; }

	void set_name(const std::string& name) { name_ = name; }
	const std::string& name() const { return name_; }
	
	void set_window(time_window win) { window_ = win; }
	time_window window() const { return window_; }
	
	virtual bool is_connected() const = 0;
	virtual filter_edge_base& edge() const = 0;
	
	virtual input_index_type input_index() const { throw not_implemented(); }
	virtual void set_input_index(input_index_type) { throw not_implemented(); }
};


template<std::size_t Input_dim, typename Input_elem>
class filter_input : public filter_input_base {
public:
	constexpr static std::size_t dimension = Input_dim;
	using elem_type = Input_elem;
	
	using edge_base_type = filter_edge_input_base<Input_dim, Input_elem>;
	using frame_shape_type = ndsize<Input_dim>;
	
private:
	std::unique_ptr<edge_base_type> edge_;
	
public:
	explicit filter_input(filter& filt) :
		filter_input_base(filt) { }
	
	bool is_connected() const override;
	const edge_base_type& edge() const override;
	
	const frame_shape_type& frame_shape() const;
	
	void connect(filter_output<Input_dim, Input_elem>&);
	
	//template<std::size_t Output_dim, typename Output_elem>
	//void connect(filter_output<Output_dim, Output_elem>&);
};

}

#include "filter_input.tcc"

#endif