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
    InDegH.AddDat(NI.GetId(), NI.GetInDeg());
  }
}
template<class PGraph>
void GetOutDegSeqH(const PGraph& Graph, TIntIntH& OutDegH) {
  typename PGraph::TObj::TNodeI NI;
  OutDegH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    OutDegH.AddDat(NI.GetId(), NI.GetOutDeg());
  }
}
template<class PGraph>
void GetDegSeqH(const PGraph& Graph, TIntIntH& DegH) {
  typename PGraph::TObj::TNodeI NI;
  DegH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    DegH.AddDat(NI.GetId(), NI.GetDeg());
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
template <class PGraph> void GetkInDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int& k);
/// Returns k out degree distribution using fixed memory BFS
template <class PGraph> void GetkOutDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int& k);
/// Returns k degree distribution using fixed memory BFS
template <class PGraph> void GetkDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int& k);

template <class PGraph>
class TFixedMemorykDeg : public TFixedMemoryBFS<PGraph> {
public:
  // Backward / forward visitor (degree only)
  class TkDegVisitor {
  public:
    TIntV DegV;
  public:
    TkDegVisitor(const int& k) : DegV(k) { }
    void Start() {
      DegV[0] = -1;
    }
    void DiscoverNode(const int& NId, const int& depth) { 
      DegV[depth]++;
    }
    void FinishNode(const int& NId, const int& depth) { }
    void ExamineEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) { }
    void TreeEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) { }
    void BackEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) { }
    void ForwardEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) { }
    void Finish() {
      for (int i = 1; i < DegV.Len(); i++) { DegV[i] += DegV[i - 1]; };
    }
    void Clr() {
      for (int i = 0; i < DegV.Len(); i++) { DegV[i] = 0; };
    }
  };
private:
  TkDegVisitor Visitor;
public:
  TFixedMemorykDeg(const PGraph& GraphArg, const int& k) : TFixedMemoryBFS<PGraph>(GraphArg), Visitor(TkDegVisitor(k)) { }
  void GetkInDegV(const int& NId, TIntV& DegV, const int& k);
  void GetkOutDegV(const int& NId, TIntV& DegV, const int& k);
  void GetkDegV(const int& NId, TIntV& DegV, const int& k);
  void GetkInDegSeqH(TIntIntVH& DegVH, const int& k);
  void GetkOutDegSeqH(TIntIntVH& DegVH, const int& k);
  void GetkDegSeqH(TIntIntVH& DegVH, const int& k);
  void Clr(const bool& DoDel = false);
};

template <class PGraph>
void TFixedMemorykDeg<PGraph>::GetkInDegV(const int& NId, TIntV& DegV, const int& k) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial -1
  this->GetBfsVisitor<TkDegVisitor>(Ego, Visitor, edInDirected, k);
  DegV = Visitor.DegV;
}
template <class PGraph>
void TFixedMemorykDeg<PGraph>::GetkOutDegV(const int& NId, TIntV& DegV, const int& k) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial -1
  this->GetBfsVisitor<TkDegVisitor>(Ego, Visitor, edOutDirected, k);
  DegV = Visitor.DegV;
}
template <class PGraph>
void TFixedMemorykDeg<PGraph>::GetkDegV(const int& NId, TIntV& DegV, const int& k) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial -1
  this->GetBfsVisitor<TkDegVisitor>(Ego, Visitor, edUnDirected, k);
  DegV = Visitor.DegV;
}

template <class PGraph>
void TFixedMemorykDeg<PGraph>::GetkInDegSeqH(TIntIntVH& DegVH, const int& k) {
  typename PGraph::TObj::TNodeI NI;
  TIntV DegV;
  DegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    DegV.Clr();
    GetkInDegV(NI.GetId(), DegV, k);
    DegVH.AddDat(NI.GetId(), DegV);
  }
}
template <class PGraph>
void TFixedMemorykDeg<PGraph>::GetkOutDegSeqH(TIntIntVH& DegVH, const int& k) {
  typename PGraph::TObj::TNodeI NI;
  TIntV DegV;
  DegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    DegV.Clr();
    GetkOutDegV(NI.GetId(), DegV, k);
    DegVH.AddDat(NI.GetId(), DegV);
  }
}
template <class PGraph>
void TFixedMemorykDeg<PGraph>::GetkDegSeqH(TIntIntVH& DegVH, const int& k) {
  typename PGraph::TObj::TNodeI NI;
  TIntV DegV;
  DegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    DegV.Clr();
    GetkDegV(NI.GetId(), DegV, k);
    DegVH.AddDat(NI.GetId(), DegV);
  }
}

template <class PGraph>
void TFixedMemorykDeg<PGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<PGraph>::Clr(DoDel);
  Visitor.Clr(); // resets the degree visitor to the initial -1
}

template <class PGraph>
void GetkInDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int& k) {
  TFixedMemorykDeg<PGraph> FixedMemorykDeg(Graph, k);
  FixedMemorykDeg.GetkInDegSeqH(DegVH, k);
}
template <class PGraph>
void GetkOutDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int& k) {
  TFixedMemorykDeg<PGraph> FixedMemorykDeg(Graph, k);
  FixedMemorykDeg.GetkOutDegSeqH(DegVH, k);
}
template <class PGraph>
void GetkDegSeqH(const PGraph& Graph, TIntIntVH& DegVH, const int& k) {
  TFixedMemorykDeg<PGraph> FixedMemorykDeg(Graph, k);
  FixedMemorykDeg.GetkDegSeqH(DegVH, k);
}

} // namespace TSnap

#endif
