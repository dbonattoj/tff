#ifndef TFF_WORKER_H_
#define TFF_WORKER_H_

#include "../common.h"
#include <thread>
#include <string>

namespace tff {

class worker {
public:
	using thread_function = std::function<void(const worker&)>;
	
private:
	std::thread thread_;
	std::string name_;
	thread_index_type thread_index_;
	
	void thread_main_();
	
public:
	worker(thread_index_type index, thread_function function, const std::string& name);
	~worker();
};

};

#endif
