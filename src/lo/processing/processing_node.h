#ifndef TFF_PROCESSING_NODE_H_
#define TFF_PROCESSING_NODE_H_

#include "../node.h"
#include "../ring/ring.h"

#include <memory>
#include <map>

namespace tff {

class processing_handler {
public:
	virtual void handler_pre_process_(processing_job&) = 0;
	virtual void handler_process_(processing_job&) = 0;
};


class processing_node : public node {
private:
	processing_handler* handler_ = nullptr;
	std::unique_ptr<rqueue_type> queue_;
	std::map<output_index_type, channel_index_type> output_channels_;
	
	time_unit current_time_ = undefined_time;
	
	const ring& ring_() const { return queue_.ring(); }
	
	bool call_handler_pre_process(processing_job&) const;
	bool call_handler_process(processing_job&) const;
	
protected:
	bool write_next_(rqueue_write_handle&);
	
	const rqueue_type& rqueue_() const { return *queue_; }
	rqueue_type& rqueue_() { return *queue_; }
	
public:
	processing_node(node_graph);
	
	std::size_t channels_count() const { return ring_().ndarray_channels_count(); }
	
	void setup() override;
	void request(time_span) override;
	void launch() override;
	void stop() override;
	
	read_handle read_output(time_span, output_index_type) override;

};


};

#endif
