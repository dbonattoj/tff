#ifndef TFF_NODE_RTTI_H_
#define TFF_NODE_RTTI_H_

#include "../processing/processing_node.h"
#include "../processing/sync_node.h"
#include "../processing/async_node.h"
#include "../sink_node.h"


namespace tff {

bool is_processing_node(const node&);
bool is_sync_node(const node&);
bool is_async_node(const node&);
bool is_sink_node(const node&);

template<typename Function>
void node_downcast_dispatch(const node& nd, Function&& func) {
	if(is_sync_node(nd)) func(static_cast<const sync_node&>(nd));
	else if(is_async_node(nd)) func(static_cast<const async_node&>(nd));
	else if(is_processing_node(nd)) func(static_cast<const processing_node&>(nd));
	else if(is_sink_node(nd)) func(static_cast<const sink_node&>(nd));
	else func(nd);
};

}

#endif