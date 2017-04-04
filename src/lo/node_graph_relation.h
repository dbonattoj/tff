#ifndef TFF_NODE_GRAPH_RELATION_H_
#define TFF_NODE_GRAPH_RELATION_H_

namespace tff {

class node;

bool precedes(const node&, const node&);
bool precedes_strict(const node&, const node&);

}

#endif
