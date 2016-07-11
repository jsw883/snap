#ifndef core_gauxiliary
#define core_gauxiliary

// Headers (?)

namespace TSnap {

/// Removes nodes of degree zero from the graph (binary degree).
template <class PGraph> void RemoveIsolated(PGraph& Graph);

/// Removes nodes of degree zero from the graph (binary degree).
template <class PGraph>
void RemoveIsolated(PGraph& Graph) {
  typename PGraph::TObj::TNodeI NI;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    if (NI.GetDeg() == 0) {
      Graph->DelNode(NI);
    }
  }
}

}

#endif