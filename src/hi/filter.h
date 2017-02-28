#ifndef TFF_FILTER_H_
#define TFF_FILTER_H_

#include "../utility/ref_vector.h"
#include <string>

namespace tff {

class filter_input_base;
class filter_output_base;
class filter_installation_guide;

class filter {
private:
	ref_vector<filter_input_base> inputs_;
	ref_vector<filter_output_base> outputs_;
	
	bool was_installed_ = false;
	std::string name_;

	filter(const filter&) = delete;
	filter& operator=(const filter&) = delete;
	
protected:
	filter() = default;
	virtual void install_() = 0;
	
public:
	virtual ~filter() = default;
	
	void set_name(const std::string& nm) { name_ = nm; }
	const std::string& name() const { return name_; }
	
	auto& inputs() { return inputs_; }
	auto& outputs() { return outputs_; }
	const auto& inputs() const { return inputs_; }
	const auto& outputs() const { return outputs_; }
	bool is_source() const { return inputs_.empty(); }
	bool is_sink() const { return outputs_.empty(); }
	
	void register_input(filter_input_base&);
	void register_output(filter_output_base&);
	
	void propagate_install(filter_installation_guide&);
};

};

#endif