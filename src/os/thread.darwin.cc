#include "os.h"
#ifdef TFF_OS_DARWIN

#include "thread.h"
#include <pthread.h>

namespace tff {

void set_this_thread_name(const std::string& name) {
	std::string truncated_name = name.substr(0, 15);
	pthread_setname_np(truncated_name.c_str());
}

}

#endif