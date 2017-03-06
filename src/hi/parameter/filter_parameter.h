#ifndef TFF_FILTER_PARAMETER_H_
#define TFF_FILTER_PARAMETER_H_

namespace tff {

class filter_parameter_base {
public:
	enum kind { deterministic, dynamic, reference };
};

template<typename Value>
class filter_parameter : public filter_parameter_base {
	
};

}

#endif
