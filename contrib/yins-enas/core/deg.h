#ifndef core_deg
#define core_deg

// Headers (?)

// #include "Snap.h"

namespace TSnap {

/// Computes in degree distribution
template <class PGraph> void GetInDegH(const PGraph& Graph, TIntIntH& InDegH);
/// Computes out degree distribution
template <class PGraph> void GetOutDegH(const PGraph& Graph, TIntIntH& OutDegH);
/// Computes degree distribution
template <class PGraph> void GetDegH(const PGraph& Graph, TIntIntH& DegH);

/// Computes degree distributions (in / out / undirected)
template <class PGraph> void GetDegVH(const PGraph& Graph, TIntIntVH& DegVH);

template<class PGraph>
void GetInDegSeqH(const PGraph& Graph, TIntFltH& InDegH) {
  typename PGraph::TObj::TNodeI NI;
  InDegH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    InDegH.AddDat(NI.GetId(), NI.GetWInDeg());
  }
}
template<class PGraph>
void GetOutDegSeqH(const PGraph& Graph, TIntIntH& OutDegH) {
  typename PGraph::TObj::TNodeI NI;
  OutDegH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    OutDegH.AddDat(NI.GetId(), NI.GetWOutDeg());
  }
}
template<class PGraph>
void GetDegSeqH(const PGraph& Graph, TIntIntH& DegH) {
  typename PGraph::TObj::TNodeI NI;
  DegH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    DegH.AddDat(NI.GetId(), NI.GetWDeg());
  }
}

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
