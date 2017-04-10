#ifndef TFF_FILTER_OUTPUT_H_
#define TFF_FILTER_OUTPUT_H_

#include "../utility/ref_vector.h"
#include "../lo/types.h"
#include <string>

namespace tff {

class filter;
class filter_input_base;
class filter_output_format;
class opaque_ndarray_format;


class filter_edge_base;
template<std::size_t Output_dim, typename Output_elem> class filter_edge_output_base;


/// Output of filter, type-erased base class.
class filter_output_base {
private:
	filter& filter_;
	std::string name_ = "out";
	bool buffer_required_ = false;
	
	filter_output_base(const filter_output_base&) = delete;
	filter_output_base& operator=(const filter_output_base&) = delete;
	
protected:
	explicit filter_output_base(filter&);

public:
	virtual ~filter_output_base() = default;
	
	filter& this_filter() { return filter_; }
	const filter& this_filter() const { return filter_; }

	void set_name(const std::string& name) { name_ = name; }
	const std::string& name() const { return name_; }
	
	virtual std::size_t edges_count() const = 0;
	virtual const filter_edge_base& edge_at(std::ptrdiff_t) const = 0;
	virtual filter_edge_base& edge_at(std::ptrdiff_t) = 0;
	virtual void edge_has_disconnected(const filter_edge_base&) = 0;
	bool is_connected() const { return (this->edges_count() > 0); }
	
	virtual bool frame_shape_is_defined() const = 0;
	virtual opaque_ndarray_format data_format() const { throw not_implemented(); }

	virtual data_channel_index_type data_channel_index() const { throw not_implemented(); }
	virtual void set_data_channel_index(data_channel_index_type) { throw not_implemented(); }

	virtual bool has_internal_edge() const { throw not_implemented(); }
	virtual filter_edge_base& internal_edge() { throw not_implemented(); }
	virtual const filter_edge_base& internal_edge() const { throw not_implemented(); }
};


/// Output of filter, base class with concrete frame data type.
template<std::size_t Output_dim, typename Output_elem>
class filter_output : public filter_output_base {
public:
	constexpr static std::size_t dimension = Output_dim;
	using elem_type = Output_elem;

	using edge_base_type = filter_edge_output_base<Output_dim, Output_elem>;
	using frame_shape_type = ndsize<Output_dim>;
	
private:
	ref_vector<edge_base_type> edges_;
	
public:
	explicit filter_output(filter& filt) :
		filter_output_base(filt) { }
	
	auto& edges() { return edges_; }
	const auto& edges() const { return edges_; }
	std::size_t edges_count() const override;
	const filter_edge_base& edge_at(std::ptrdiff_t) const override;
	filter_edge_base& edge_at(std::ptrdiff_t) override;
	
	void edge_has_connected(edge_base_type&);
	void edge_has_disconnected(const filter_edge_base&) override;
	
	virtual const frame_shape_type& frame_shape() const = 0;
};

}

#include "filter_output.tcc"

#endif
