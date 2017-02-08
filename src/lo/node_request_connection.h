#ifndef TFF_NODE_REQUEST_CONNECTION_H_
#define TFF_NODE_REQUEST_CONNECTION_H_

#include "../common.h"
#include "node_graph_relation.h"

namespace tff {

class node;

/// Request connection between two nodes.
/** _Sender_ node recursively forwards requests to connected _receivers_, with span expanded by the connection's
 ** time window. Each node can be connected to multiple receivers. Each node is connected to one or zero senders. */
class node_request_connection {
private:
	node& sender_;
	node& receiver_;
	time_window window_;
	
public:
	node_request_connection(node& sender, node& receiver, const time_window& win) :
		sender_(sender), receiver_(receiver), window_(win) { Assert(precedes_strict(receiver, sender)); }
	
	time_window window() const { return window_; }
	void set_window(time_window win) { window_ = win; }
	
	const node& sender() const { return sender_; }
	node& sender() { return sender_; }
	const node& receiver() const { return receiver_; }
	node& receiver() { return receiver_; }
};

}

#endif
