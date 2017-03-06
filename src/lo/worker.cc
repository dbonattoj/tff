#include "worker.h"
#include "../os/thread.h"
#include "../utility/assert.h"

#include <iostream>

namespace tff {

void worker::thread_main_() {
	set_this_thread_name(name_);
	
	std::unique_lock<std::mutex> lock(mutex_);
	while(state_ != kill) {
		if(state_ == idle) {
			cv_.wait(lock);
		} else if(state_ == launch) {
			this->worker_main_();
			state_ = idle;
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


void worker::worker_wait_idle_(){
	std::lock_guard<std::mutex> lock(mutex_);
	Assert(state_ == idle);
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