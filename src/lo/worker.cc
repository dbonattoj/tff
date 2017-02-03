#include "worker.h"

namespace tff {

void worker::thread_main_() {
	set_this_thread_name(name_);
	
	std::unique_lock<std::mutex> lock(mutex_);
	for(;;) {
		cv_.wait(lock);

		if(state_ == launch) {
			this->thread_main_();
			state_ = idle;
		} else if(state == idle) {
			continue;
		} else if(state == kill) {
			break;
		}
	}
}


void worker::worker_launch_() {
	{
		std::lock_guard<std::mutex> lock(mutex_);
		state_ = launch;
	}
	cv_.notify_one();
}


worker::worker(thread_index_type index, const std::string& name) :
	thread_index_(index),
	name_(name),
	state_(idle),
	thread_(std::bind(&worker::thread_main_, this)) { }


worker::~worker() {
	{
		std::lock_guard<std::mutex> lock(mutex_);
		state_ = kill;
	}
	cv_.notify_one();
	thread_.join();
}


}