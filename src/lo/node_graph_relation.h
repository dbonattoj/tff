#ifndef TFF_NODE_GRAPH_RELATION_H_
#define TFF_NODE_GRAPH_RELATION_H_

#include "node.h"

namespace tff {

class node;

bool precedes(const node&, const node&);
bool precedes_strict(const node&, const node&);

const node& first_successor(const node&);
node& first_successor(node&);

}

#endif