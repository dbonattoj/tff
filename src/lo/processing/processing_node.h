#ifndef TFF_PROCESSING_NODE_H_
#define TFF_PROCESSING_NODE_H_

#include "../node.h"
#include "../ring/ring.h"

#include <memory>

namespace tff {

class processing_handler;
class processing_job;

class processing_node : public node {
private:
	ring_format format_;
	optional<rqueue_type> queue_;
	
	processing_handler* handler_ = nullptr;
	
	time_unit current_time_ = undefined_time;
	
	const ring& ring_() const;
	
	bool call_handler_pre_process(processing_job&) const;
	bool call_handler_process(processing_job&) const;
	
protected:
	bool write_next_(rqueue_type::write_handle&);
	
	const rqueue_type& rqueue_() const { return *queue_; }
	rqueue_type& rqueue_() { return *queue_; }
	
	void queue_request_(time_span);
	void queue_stop_();
	
	void setup_ring_(rqueue_variant, std::size_t required_capacity);
	
public:
	processing_node(node_graph&, const std::string& name);
	
	data_channel_index_type add_data_channel(const opaque_ndarray_format&);
	std::size_t data_channels_count() const;
	
	node_input& add_input();
	node_output& add_data_output(data_channel_index_type);
	node_output& add_pull_only_output();
	
	bool has_handler() const { return (handler_ != nullptr); }
	void set_handler(processing_handler& hd) { handler_ = &hd; }
	processing_handler& handler() const { Assert(handler_ != nullptr); return *handler_; }
	
	virtual thread_index_type processing_thread() const = 0;
	
	void verify() const override;
	void setup() override;
	
	time_unit current_time() const { return current_time_; }

	thread_index_type input_reader_thread(input_index_type) const final override;
	thread_index_type request_sender_thread() const final override;
	
	node_read_handle read(time_span, const node_read_guide&) override;
};


};

#endif
