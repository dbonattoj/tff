#include "node_rtti.h"
#include "../node.h"
#include "../processing/processing_node.h"
#include "../processing/sync_node.h"
#include "../processing/async_node.h"
#include "../processing/sink_node.h"

namespace tff {

namespace {

template<typename T>
bool is_a_or_derived_(const node& nd) {
	return (dynamic_cast<const T*>(&nd) != nullptr);
}

}

bool is_processing_node(const node& nd) {
	return is_a_or_derived_<processing_node>(nd);
}

bool is_sync_node(const node& nd) {
	return is_a_or_derived_<sync_node>(nd);
}

bool is_async_node(const node& nd) {
	return is_a_or_derived_<async_node>(nd);
}

bool is_sink_node(const node& nd) {
	return is_a_or_derived_<sink_node>(nd);
}


}