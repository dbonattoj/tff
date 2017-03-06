#ifndef TFF_ENCLOSURE_FILTER_H_
#define TFF_ENCLOSURE_FILTER_H_

#include "../filter.h"

namespace tff {

class enclosure_filter_base : public filter {
public:
	struct parallelization_configuration {
		std::size_t factor = 1;
		std::size_t chunk_duration = 1;
	};

protected:
	static thread_local enclosure_filter_base* current_in_construction_;

public:
	static enclosure_filter_base& current_in_construction() {
		Assert(enclosure_filter_base != nullptr);
		return *enclosure_filter_base;
	}
};


template<typename Box>
class enclosure_filter : public enclosure_filter_base {
public:
	using box_type = Box;
	
private:
	optional<box_type> box_;
	filter_subgraph subgraph_;
	parallelization_configuration parallelization_;
	
protected:
	void setup_() override;
	void install_(filter_installation_guide&) override;
	
public:
	template<typename... Box_args>
	enclosure_filter(Box_args&&...);
	
	const box_type& box() const { return *box_; }
	box_type& box() { return *box_; }

	const parallelization_configuration& parallelization() const { return parallelization_; }
	void set_parallelization(const parallelization_configuration& par) { parallelization_ = par; }
};


template<std::size_t Input_dim, typename Input_elem>
class enclosure_filter_input : public filter_input<Input_dim, Input_elem> {
private:
	filter_input<Input_dim, Input_elem>* internal_input_ = nullptr;
};


template<std::size_t Output_dim, typename Output_elem>
class enclosure_filter_output : public filter_output<Output_dim, Output_elem> {
private:
	filter_output<Output_dim, Output_elem>* internal_output_ = nullptr;
};

}

#include "enclosure_filter.tcc"

#endif