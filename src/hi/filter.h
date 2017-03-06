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
	enum class stage { initial, was_setup, was_installed };
	
	ref_vector<filter_input_base> inputs_;
	ref_vector<filter_output_base> outputs_;
	
	stage stage_ = stage::initial;
	std::string name_;
	bool pulled_ = false;

	filter(const filter&) = delete;
	filter& operator=(const filter&) = delete;

protected:
	filter() = default;
	virtual void setup_() = 0;
	virtual void install_(filter_installation_guide&) = 0;
	
public:
	virtual ~filter() = default;
	
	void set_name(const std::string& nm) { name_ = nm; }
	const std::string& name() const { return name_; }
	
	void set_is_pulled(bool pulled) { pulled_ = pulled; }
	bool is_pulled() const { return pulled_; }
	
	auto& inputs() { return inputs_; }
	auto& outputs() { return outputs_; }
	const auto& inputs() const { return inputs_; }
	const auto& outputs() const { return outputs_; }
	
	void register_input(filter_input_base&);
	void register_output(filter_output_base&);
	
	void propagate_prepare_install(filter_installation_guide&) const;
	void propagate_setup();
	void propagate_install(filter_installation_guide&);
};

};

#endif