#ifndef core_deg
#define core_deg

// Headers (?)

#include "galg.h"

namespace TSnap {

/// Computes in degree distribution using graph degrees
template <class PGraph> void GetInDegH(const PGraph& Graph, TIntIntH& InDegH);
/// Computes out degree distribution using graph degrees
template <class PGraph> void GetOutDegH(const PGraph& Graph, TIntIntH& OutDegH);
/// Computes degree distribution using graph degrees
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

namespace TSnap {

/// Returns k in degree distribution using fixed memory BFS
template <class PGraph> void GetkInDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int k);
/// Returns k out degree distribution using fixed memory BFS
template <class PGraph> void GetkOutDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int k);
/// Returns k degree distribution using fixed memory BFS
template <class PGraph> void GetkDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int k);

template <class PGraph>
class TFixedMemorykDeg : public TFixedMemoryBFS<PGraph> {
public:
  // Backward / forward visitor (degree only)
  class TkDegVisitor {
  public:
    int Deg;
  public:
    TkDegVisitor() : Deg(-1) { }
    void DiscoverNode(int NId, int depth) { 
      Deg++;
    }
    void FinishNode(const int& NId, int depth) { }
    void ExamineEdge(const int& NId1, const int& NId2) { }
    void TreeEdge(const int& NId1, const int& NId2) { }
    void BackEdge(const int& NId1, const int& NId2) { }
    void FwdEdge(const int& NId1, const int& NId2) { }
    void Clr() {
      Deg = -1;
    }
  };
private:
  TkDegVisitor Visitor;
public:
  TFixedMemorykDeg(const PGraph& GraphArg) : TFixedMemoryBFS<PGraph>(GraphArg), Visitor(TkDegVisitor()) { }
  int GetkInDeg(const int NId, const int k);
  int GetkOutDeg(const int NId, const int k);
  int GetkDeg(const int NId, const int k);
  void GetkInDegSeqH(TIntIntVH& DegVH, const int k);
  void GetkOutDegSeqH(TIntIntVH& DegVH, const int k);
  void GetkDegSeqH(TIntIntVH& DegVH, const int k);
  void Clr(const bool& DoDel = false);
};

template <class PGraph>
int TFixedMemorykDeg<PGraph>::GetkInDeg(const int NId, const int k) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial -1
  this->GetBfsVisitor<TkDegVisitor>(Ego, Visitor, edInDirected, k);
  return(Visitor.Deg);
}
template <class PGraph>
int TFixedMemorykDeg<PGraph>::GetkOutDeg(const int NId, const int k) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial -1
  this->GetBfsVisitor<TkDegVisitor>(Ego, Visitor, edOutDirected, k);
  return(Visitor.Deg);
}
template <class PGraph>
int TFixedMemorykDeg<PGraph>::GetkDeg(const int NId, const int k) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial -1
  this->GetBfsVisitor<TkDegVisitor>(Ego, Visitor, edUnDirected, k);
  return(Visitor.Deg);
}

template <class PGraph>
void TFixedMemorykDeg<PGraph>::GetkInDegSeqH(TIntIntVH& DegVH, const int k) {
  typename PGraph::TObj::TNodeI NI;
  TIntV DegV;
  DegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    DegV.Clr();
    for (int i = 1; i <= k; i++) {
      DegV.Add(GetkInDeg(NI.GetId(), i));
    }
    DegVH.AddDat(NI.GetId(), DegV);
  }
}
template <class PGraph>
void TFixedMemorykDeg<PGraph>::GetkOutDegSeqH(TIntIntVH& DegVH, const int k) {
  typename PGraph::TObj::TNodeI NI;
  TIntV DegV;
  DegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    DegV.Clr();
    for (int i = 1; i <= k; i++) {
      DegV.Add(GetkOutDeg(NI.GetId(), i));
    }
    DegVH.AddDat(NI.GetId(), DegV);
  }
}
template <class PGraph>
void TFixedMemorykDeg<PGraph>::GetkDegSeqH(TIntIntVH& DegVH, const int k) {
  typename PGraph::TObj::TNodeI NI;
  TIntV DegV;
  DegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    DegV.Clr();
    for (int i = 1; i <= k; i++) {
      DegV.Add(GetkDeg(NI.GetId(), i));
    }
    DegVH.AddDat(NI.GetId(), DegV);
  }
}

template <class PGraph>
void TFixedMemorykDeg<PGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<PGraph>::Clr(DoDel);
  Visitor.Clr(); // resets the degree visitor to the initial -1
}

template <class PGraph>
void GetkInDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int k) {
  TFixedMemorykDeg<PGraph> FixedMemorykDeg(Graph);
  FixedMemorykDeg.GetkInDegSeqH(DegVH, k);
}
template <class PGraph>
void GetkOutDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int k) {
  TFixedMemorykDeg<PGraph> FixedMemorykDeg(Graph);
  FixedMemorykDeg.GetkOutDegSeqH(DegVH, k);
}
template <class PGraph>
void GetkDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int k) {
  TFixedMemorykDeg<PGraph> FixedMemorykDeg(Graph);
  FixedMemorykDeg.GetkDegSeqH(DegVH, k);
}

} // namespace TSnap

#endif
