#ifndef core_deg
#define core_deg

// Headers (?)

// #include "Snap.h"

namespace TSnap {

/// Computes first degree distributions (in / out / undirected)
template <class PGraph> void GetDegVH(const PGraph& Graph, TIntIntVH& DegVH);

template <class PGraph>
void GetDegVH(const PGraph& Graph, TIntIntVH& DegVH) {
  typename PGraph::TObj::TNodeI NI;
  TIntV DegV;
  DegVH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    DegV.Clr();
    DegV.Add(NI.GetInDeg());
    DegV.Add(NI.GetOutDeg());
    DegV.Add(NI.GetDeg());
    DegVH.AddDat(NI.GetId(), DegV);
  }
}

} // namespace TSnap

#endif
