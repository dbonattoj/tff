#ifndef TFF_FILTER_GRAPH_RELATION_H_
#define TFF_FILTER_GRAPH_RELATION_H_

namespace tff {

class filter;

bool precedes(const filter&, const filter&, bool strict = true);
bool succeedes(const filter&, const filter&, bool strict = true);

}

#endif