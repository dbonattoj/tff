#ifndef TFF_FILTER_BOX_H_
#define TFF_FILTER_BOX_H_

namespace tff {

/// Base class for application filter implementation.
class filter_box {
public:
	struct shared_data { };
	struct local_data { };

private:
	filter& filter_;

protected:
	explicit filter_box(filter&);
	virtual ~filter_box() = default;
	
	template<std::size_t Dim, typename Elem> using input = filter_input<Dim, Elem>;
	template<std::size_t Dim, typename Elem> using output = filter_output<Dim, Elem>;
	
	void pre_process(filter_processing_job&) const;
	void process(filter_processsing_job&) const;
};

}

#endif