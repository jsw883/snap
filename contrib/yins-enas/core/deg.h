#ifndef core_deg
#define core_deg

// Headers (?)

#include "galg.h"

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

// Backward / forward visitor (degree only)
class TDegVisitor {
public:
  int Deg;
public:
  TDegVisitor() : Deg(-1) { }
  void DiscoverNode(int NId, int depth) { 
    Deg++;
  }
  void FinishNode(const int& NId, int depth) { }
  void ExamineEdge(const int& NId1, const int& NId2) { }
  void TreeEdge(const int& NId1, const int& NId2) { }
  void BackEdge(const int& NId1, const int& NId2) { }
  void FwdEdge(const int& NId1, const int& NId2) { }
};

template <class PGraph>
int GetBDeg(const PGraph& Graph, const PGraph& SubGraph, const int k) {
  TDegVisitor Visitor;
  GetBBfsVisitor(Graph, SubGraph, Visitor, k);
  return(Visitor.Deg);
}

template <class PGraph>
int GetFDeg(const PGraph& Graph, const PGraph& SubGraph, const int k) {
  TDegVisitor Visitor;
  GetFBfsVisitor(Graph, SubGraph, Visitor, k);
  return(Visitor.Deg);
}

template <class PGraph>
int GetUDeg(const PGraph& Graph, const PGraph& SubGraph, const int k) {
  TDegVisitor Visitor;
  GetUBfsVisitor(Graph, SubGraph, Visitor, k);
  return(Visitor.Deg);
}

namespace TSnap {

// Returns k deep degrees for node CtrNId (in / out / undirected)
int GetkInDeg(const PNGraph& Graph, const int CtrNId, const int k);
int GetkOutDeg(const PNGraph& Graph, const int CtrNId, const int k);
int GetkDeg(const PNGraph& Graph, const int CtrNId, const int k);

// Returns k deep degrees for the graph as DegVH (in / out / undirected)
void GetkInDegSeqH(const PNGraph& Graph, TIntIntVH& DegVH, const int k);
void GetkOutDegSeqH(const PNGraph& Graph, TIntIntVH& DegVH, const int k);
void GetkDegSeqH(const PNGraph& Graph, TIntIntVH& DegVH, const int k);

}

#endif
