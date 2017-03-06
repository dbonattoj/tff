#ifndef TFF_ENCLOSURE_BOX_H_
#define TFF_ENCLOSURE_BOX_H_

namespace tff {

class enclosure_box {
protected:
	template<std::size_t Dim, typename Elem> class input;
	template<std::size_t Dim, typename Elem> class output;

	template<std::size_t Dim, typename Elem>
	void expose_input(filter_input<Dim, Elem>& internal, input<Dim, Elem>& external);

	template<std::size_t Dim, typename Elem>
	void expose_output(filter_output<Dim, Elem>& internal, output<Dim, Elem>& external);
	
public:
	//enclosure_box(filter_subgraph&);
};

}

#endif
