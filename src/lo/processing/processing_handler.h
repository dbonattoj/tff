#ifndef TFF_PROCESSING_HANDLER_H_
#define TFF_PROCESSING_HANDLER_H_

namespace tff {

class processing_job;

class processing_handler {
public:
	virtual void handler_pre_process_(processing_job&) = 0;
	virtual void handler_process_(processing_job&) = 0;
};

}

#endif