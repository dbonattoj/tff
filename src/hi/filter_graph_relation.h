#ifndef TFF_FILTER_GRAPH_RELATION_H_
#define TFF_FILTER_GRAPH_RELATION_H_

namespace tff {

class filter;

bool precedes(const filter&, const filter&);
bool precedes_strict(const filter&, const filter&);

}

#endif