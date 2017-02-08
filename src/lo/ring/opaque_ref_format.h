#ifndef TFF_OPAQUE_REF_FORMAT_H_
#define TFF_OPAQUE_REF_FORMAT_H_

#include "../../nd/pod_array_format.h"
#include "../../nd/ndcoord_dyn.h"

namespace tff {

/// Opaque format for \ref ndarray_opaque_view which references another opaque format.
/** Contains only the reference to watched opaque format. Avoids propagating a copy of the opaque format every time
 ** a section of a view is taken. The watched opaque format must remain alive. */
template<typename Opaque_format>
class opaque_ref_format {
public:
	using format_type = Opaque_format;
	using frame_handle_type = typename format_type::frame_handle_type;
	using const_frame_handle_type = typename format_type::const_frame_handle_type;
	using frame_pointer_type = typename format_type::frame_pointer_type;
	using const_frame_pointer_type = typename format_type::const_frame_pointer_type;
	
private:
	const Opaque_format& format_;
	
public:
	explicit opaque_ref_format(const format_type& frm) : format_(frm) { }
	
	std::size_t size() const { return format_.size(); }
	std::size_t alignment_requirement() const { return format_.alignment_requirement(); }

	bool is_pod() const { return format_.is_pod(); }
	pod_array_format pod_format() const { return format_.pod_format(); }
	
	bool is_ndarray() const { return format_.is_ndarray(); }
	std::size_t dimension() const { return format_.dimension(); }
	const ndsize_dyn& shape() const { return format_.shape(); }
	std::size_t elem_size() const { return format_.elem_size(); }
	std::size_t elem_alignment_requirement() const { return format_.elem_alignment_requirement(); }
	std::size_t elem_stride() const { return format_.elem_stride(); }

	friend bool operator==(const opaque_ref_format& a, const opaque_ref_format& b) {
		return (a.format_ == b.format_);
	}
	
	friend bool operator!=(const opaque_ref_format& a, const opaque_ref_format& b) {
		return (a.format_ != b.format_);
	}
};

};

#endif
