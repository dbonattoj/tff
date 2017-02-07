#ifndef TFF_WORKER_H_
#define TFF_WORKER_H_

#include "../common.h"
#include <thread>
#include <string>
#include <atomic>

namespace tff {

/// Object which owns a separate thread, base class.
/** The thread is created and paused when \ref worker is constructed. */
class worker {
private:
	thread_index_type thread_index_;
	std::string name_;
	
	enum { idle, launch, kill } state_;
	std::mutex mutex_;
	std::condition_variable cv_;

	std::thread thread_;
	
	void thread_main_();
	
protected:
	virtual void worker_main_() = 0;
	
	/// Schedules call to worker_main_() on the separate thread.
	/** If worker_main_() is still executing, blocks until it is finished. */
	void worker_launch_();
	
public:
	worker(thread_index_type index, const std::string& name);
	~worker();
};

};

#endif
