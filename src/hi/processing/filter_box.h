#ifndef TFF_FILTER_BOX_H_
#define TFF_FILTER_BOX_H_

namespace tff {

/// Base class for application filter implementation.
class filter_box {
private:
	filter& filter_;
	
protected:
	template<std::size_t Dim, typename Elem> class input;
	template<std::size_t Dim, typename Elem> class output;

	filter_box();
	virtual ~filter_box() = default;
	
	void setup() { }
	void pre_process(filter_processing_job&) { }
	void process(filter_processsing_job&) { }
};


template<std::size_t Dim, typename Elem>
class filter_box::input : public filter_input<Dim, Elem> {
public:
	input();
};


template<std::size_t Dim, typename Elem>
class filter_box::output : public filter_output<Dim, Elem> {
public:
	output();
};


}

#endif