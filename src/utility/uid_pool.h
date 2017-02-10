#ifndef TFF_UTILITY_UID_POOL_H_
#define TFF_UTILITY_UID_POOD_H_

#include <map>
#include <string>
#include <memory>
#include <utility>

namespace tff {

/// Assigns unique IDs to objects based on their address.
class uid_pool {
private:
	std::map<const void*, std::string> uids_;
	
public:
	void clear() { uids_.clear(); }
	
	template<typename T>
	const std::string& uid(const T& obj, const std::string& prefix = "") {
		const void* address = std::addressof(obj);
		auto it = uids_.find(address);
		if(it != uids_.end()) {
			return it->second;
		} else {
			std::size_t index = uids_.size();
			std::string uid = prefix + std::to_string(index);
			auto res = uids_.emplace(address, std::move(uid));
			return res.first->second;
		}
	}
};

};

#endif