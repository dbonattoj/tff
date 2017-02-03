#ifndef TFF_RING_FORMAT_H_
#define TFF_RING_FORMAT_H_

namespace tff {


class ring_format {
public:
	class frame_channel {
	private:
		opaque_ndarray_format ndarray_format_;
	};
	
	class parameter_channel {
		
	};
	
private:
	std::vector<frame_channel> frame_channels_;
	std::vector<parameter_channel> parameter_channels_;
};

}

#endif