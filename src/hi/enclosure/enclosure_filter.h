#ifndef TFF_ENCLOSURE_FILTER_H_
#define TFF_ENCLOSURE_FILTER_H_

#include "../filter.h"
#include "../filter_input.h"
#include "../filter_output.h"
#include "../filter_subgraph.h"
#include "../../utility/ref_vector.h"

namespace tff {

template<std::size_t Input_dim, typename Input_elem> class enclosure_input_filter_edge;
template<std::size_t Output_dim, typename Output_elem> class enclosure_output_filter_edge;


/// Filter which contains a nested filter graph, base class.
class enclosure_filter_base : public filter {
public:
	struct parallelization_configuration {
		std::size_t factor = 1;
		std::size_t chunk_duration = 1;
	};
	
private:
	ref_vector<filter> root_internal_filters_();
	
protected:
	static thread_local enclosure_filter_base* current_in_construction_;

	filter_subgraph subgraph_;
	parallelization_configuration parallelization_;
	
	explicit enclosure_filter_base(filter_subgraph& gr) :
		filter(gr) { }

	void setup_() override;
	void install_(filter_installation_guide&) override;
	
public:
	filter_subgraph& subgraph() { return subgraph_; }
	const filter_subgraph& subgraph() const { return subgraph_; }
	
	const parallelization_configuration& parallelization() const { return parallelization_; }
	void set_parallelization(const parallelization_configuration& par) { parallelization_ = par; }

	static enclosure_filter_base& current_in_construction() {
		Assert(current_in_construction_ != nullptr);
		return *current_in_construction_;
	}
};


/// Filter which contains a nested filter graph, constructed by its _box_.
template<typename Box>
class enclosure_filter : public enclosure_filter_base {
public:
	using box_type = Box;
	
private:
	optional<box_type> box_;
	
public:
	template<typename... Box_args>
	explicit enclosure_filter(filter_subgraph&, Box_args&&...);
	
	const box_type& box() const { return *box_; }
	box_type& box() { return *box_; }
	
	const box_type& operator*() const { return box(); }
	box_type& operator*() { return box(); }
	const box_type* operator->() const { return &box(); }
	box_type* operator->() { return &box(); }
};


/// Input of enclosure filter.
/** Attached to zero or more inputs of internal filters within the enclosure.
 ** This connection is represented by a \ref enclosure_input_filter_edge. */
template<std::size_t Input_dim, typename Input_elem>
class enclosure_filter_input : public filter_input<Input_dim, Input_elem> {
	using base = filter_input<Input_dim, Input_elem>;
	
public:
	using internal_input_type = filter_input<Input_dim, Input_elem>;
	using internal_edge_type = enclosure_input_filter_edge<Input_dim, Input_elem>;
	
private:
	ref_vector<internal_edge_type> internal_edges_;
	
public:
	enclosure_filter_input() : base(enclosure_filter_base::current_in_construction()) { }
	~enclosure_filter_input();

	enclosure_filter_base& this_filter()
		{ return static_cast<enclosure_filter_base&>(base::this_filter()); }
	const enclosure_filter_base& this_filter() const
		{ return static_cast<enclosure_filter_base&>(base::this_filter()); }
	
	std::size_t internal_edges_count() const override { return internal_edges_.size(); }
	const internal_edge_type& internal_edge_at(std::ptrdiff_t index) const override { return internal_edges_.at(index); }
	internal_edge_type& internal_edge_at(std::ptrdiff_t index) override { return internal_edges_.at(index); }
	
	void internal_input_has_connected(internal_input_type&);
	void internal_input_has_disconnected(internal_input_type&);
};


/// Output of enclosure filter.
/** Attached to one output of an internal filter within the enclosure.
 ** This connection is represented by a \ref enclosure_output_filter_edge. */
template<std::size_t Output_dim, typename Output_elem>
class enclosure_filter_output : public filter_output<Output_dim, Output_elem> {
	using base = filter_output<Output_dim, Output_elem>;
	
public:
	using internal_output_type = filter_output<Output_dim, Output_elem>;
	using internal_edge_type = enclosure_output_filter_edge<Output_dim, Output_elem>;
	using typename base::frame_shape_type;
	
private:
	std::unique_ptr<internal_edge_type> internal_edge_;
	
public:
	enclosure_filter_output() : base(enclosure_filter_base::current_in_construction()) { }
	~enclosure_filter_output();

	enclosure_filter_base& this_filter()
		{ return static_cast<enclosure_filter_base&>(base::this_filter()); }
	const enclosure_filter_base& this_filter() const
		{ return static_cast<enclosure_filter_base&>(base::this_filter()); }


	void set_internal_edge(std::unique_ptr<internal_edge_type>);
	void delete_internal_edge();


	internal_output_type& internal_output();
	const internal_output_type& internal_output() const;


/*
	bool has_internal_edge() const override { return (internal_edge_ != nullptr); }
	internal_edge_type& internal_edge() override { return *internal_edge_; }
	const internal_edge_type& internal_edge() const override { return *internal_edge_; }
	*/
	bool frame_shape_is_defined() const override;
	const frame_shape_type& frame_shape() const override;
};

}

#include "enclosure_filter.tcc"

#endif