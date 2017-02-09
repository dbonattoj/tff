#ifndef TFF_PROCESSING_NODE_H_
#define TFF_PROCESSING_NODE_H_

#include "../node.h"
#include "../ring/ring.h"

#include <memory>
#include <map>

namespace tff {

class processing_job;


class processing_handler {
public:
	virtual void handler_pre_process_(processing_job&) = 0;
	virtual void handler_process_(processing_job&) = 0;
};


class processing_node : public node {
private:
	ring_format format_;
	std::unique_ptr<rqueue_type> queue_;
	std::map<output_index_type, channel_index_type> output_channels_;
	
	processing_handler* handler_ = nullptr;
	
	time_unit current_time_ = undefined_time;
	
	const ring& ring_() const;
	
	bool call_handler_pre_process(processing_job&) const;
	bool call_handler_process(processing_job&) const;
	
protected:
	bool write_next_(rqueue_type::write_handle&);
	
	const rqueue_type& rqueue_() const { return *queue_; }
	rqueue_type& rqueue_() { return *queue_; }
	
	void setup_ring_(rqueue_variant, std::size_t required_capacity);
	
public:
	processing_node(node_graph&, const std::string& name);
	
	channel_index_type add_channel(const opaque_ndarray_format&);
	std::size_t channels_count() const;
	
	node_input& add_input();
	node_output& add_output(channel_index_type);
	
	void setup() override;
	void request(time_span) override;
	void launch() override;
	void stop() override;
	
	time_unit current_time() const { return current_time_; }

	node_read_handle read_output(time_span, output_index_type) final override;
};


};

#endif
