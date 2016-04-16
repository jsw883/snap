#ifndef core_deg
#define core_deg

// Headers (?)

#include "galg.h"
#include "galgfixedmemory.h"

//#//////////////////////////////////////////////
/// Wrappers for first degrees

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
void GetInDegH(const PGraph& Graph, TIntIntH& InDegH) {
  typename PGraph::TObj::TNodeI NI;
  InDegH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    InDegH.AddDat(NI.GetId(), NI.GetInDeg());
  }
}
template<class PGraph>
void GetOutDegH(const PGraph& Graph, TIntIntH& OutDegH) {
  typename PGraph::TObj::TNodeI NI;
  OutDegH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    OutDegH.AddDat(NI.GetId(), NI.GetOutDeg());
  }
}
template<class PGraph>
void GetDegH(const PGraph& Graph, TIntIntH& DegH) {
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

//#//////////////////////////////////////////////
/// First k degrees using fixed memory BFS

namespace TSnap {

// /// Returns k in degree distribution using fixed memory BFS
// template <class PGraph> void GetkInDegH(const PGraph& Graph, TIntIntVH& DegVH, const int& k);
// /// Returns k out degree distribution using fixed memory BFS
// template <class PGraph> void GetkOutDegH(const PGraph& Graph, TIntIntVH& DegVH, const int& k);
// /// Returns k degree distribution using fixed memory BFS
// template <class PGraph> void GetkDegH(const PGraph& Graph, TIntIntVH& DegVH, const int& k);

template <class PGraph>
class TFixedMemorykDeg : public TFixedMemoryBFS<PGraph> {
public:
  // Backward / forward visitor (degree only)
  class TkDegVisitor {
  public:
    TIntV DegV;
  public:
    TkDegVisitor(const int& k) : DegV(k) { }
    void Start() { }
    void DiscoverNode(const int& NId, const int& depth) { }
    void FinishNode(const int& NId, const int& depth) { }
    void ExamineEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) { }
    void TreeEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) {
      DegV[depth]++;
    }
    void BackEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) {
      DegV[depth]++;
    }
    void ForwardEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) {
      DegV[depth]++;
    }
    void Finish() {
      for (int i = 1; i < DegV.Len(); i++) {
        DegV[i] += DegV[i - 1];
      };
    }
    void Clr() {
      for (int i = 0; i < DegV.Len(); i++) {
        DegV[i] = 0;
      };
    }
  };
private:
  TkDegVisitor Visitor;
  int k;
public:
  TFixedMemorykDeg(const PGraph& Graph, const int& k) : TFixedMemoryBFS<PGraph>(Graph), Visitor(TkDegVisitor(k)), k(k) { }
  // Get k degree for a single node (int / out / undirected)
  void GetkInDegV(const int& NId, TIntV& DegV) {
    GetkDegV(NId, DegV, edInDirected);
  }
  void GetkOutDegV(const int& NId, TIntV& DegV) {
    GetkDegV(NId, DegV, edOutDirected);
  }
  void GetkDegV(const int& NId, TIntV& DegV) {
    GetkDegV(NId, DegV, edUnDirected);
  }
  // Get k degree for a single node according to direction specified
  void GetkDegV(const int& NId, TIntV& DegV, const TEdgeDir& Dir);
  // Get k degree for all nodes (int / out / undirected)
  void GetkInDegH(TIntIntVH& DegVH) {
    GetkDegH(DegVH, edInDirected);
  }
  void GetkOutDegH(TIntIntVH& DegVH) {
    GetkDegH(DegVH, edOutDirected);
  }
  void GetkDegH(TIntIntVH& DegVH) {
    GetkDegH(DegVH, edUnDirected);
  }
  // Get k degree for all nodes according to direction specified
  void GetkDegH(TIntIntVH& DegVH, const TEdgeDir& Dir);
  void Clr(const bool& DoDel = false);
};

// Get k degree for a single node according to direction specified
template <class PGraph>
void TFixedMemorykDeg<PGraph>::GetkDegV(const int& NId, TIntV& DegV, const TEdgeDir& Dir) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial -1
  this->GetBfsVisitor<TkDegVisitor>(Ego, Visitor, Dir, k);
  DegV = Visitor.DegV;
}

// Get k degree for all nodes according to direction specified
template <class PGraph>
void TFixedMemorykDeg<PGraph>::GetkDegH(TIntIntVH& DegVH, const TEdgeDir& Dir) {
  typename PGraph::TObj::TNodeI NI;
  TIntV DegV;
  DegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    DegV.Clr();
    GetkDegV(NI.GetId(), DegV, Dir);
    DegVH.AddDat(NI.GetId(), DegV);
  }
}

template <class PGraph>
void TFixedMemorykDeg<PGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<PGraph>::Clr(DoDel);
  Visitor.Clr(); // resets the degree visitor
}

// template <class PGraph>
// void GetkInDegH(const PGraph& Graph, TIntIntVH& DegVH, const int& k) {
//   TFixedMemorykDeg<PGraph> FixedMemorykDeg(Graph, k);
//   FixedMemorykDeg.GetkInDegH(DegVH, k);
// }
// template <class PGraph>
// void GetkOutDegH(const PGraph& Graph, TIntIntVH& DegVH, const int& k) {
//   TFixedMemorykDeg<PGraph> FixedMemorykDeg(Graph, k);
//   FixedMemorykDeg.GetkOutDegH(DegVH, k);
// }
// template <class PGraph>
// void GetkDegH(const PGraph& Graph, TIntIntVH& DegVH, const int& k) {
//   TFixedMemorykDeg<PGraph> FixedMemorykDeg(Graph, k);
//   FixedMemorykDeg.GetkDegH(DegVH, k);
// }

} // namespace TSnap

#endif
