#ifndef TFF_SWITCH_NODE_H_
#define TFF_SWITCH_NODE_H_

#include "node.h"

namespace tff {

class switch_node : public node {

};

// TODO node with 1 output, cannot have time window on output.
// forwards reads to one of its inputs, depending on frame index, and parallelization/skip settings
// of predecessors

}

#endif