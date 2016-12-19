#ifndef core_subgraphs
#define core_subgraphs

// Headers (?)

#include "galg.h"
#include "galgfixedmemory.h"

namespace TSnap {

template <class PGraph>
class TFixedMemorykEgo : public TFixedMemoryBFS<PGraph> {
public:
// Backward / forward visitor (degree only)
  class TkEgoVisitor {
  public:
    // Parameters for BFS
    PGraph Graph;
    int k;
    TEdgeDir Dir;
    // Recorded during BFS
    int Nodes;
    TIntV NIdV;
  public:
    TkEgoVisitor(const PGraph& Graph, const int& k) : Graph(Graph), k(k), Dir(edUnDirected), NIdV(Graph->GetNodes()) { }
    // Supplementary methods
    void SetEdgeDir(const TEdgeDir& DirArg) { Dir = DirArg; }
    // BFS methods (standard signatures)
    void Start() { }
    void DiscoverNode(int NId, int depth) {
      NIdV.Add(NId);
    }
    void FinishNode(const int& NId, int depth) { }
    void ExamineEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) { }
    void TreeEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) { }
    void BackEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) { }
    void ForwardEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) { }
    void Finish() { }
    // Clear method (somewhat standard)
    void Clr() {
      Dir = edUnDirected;
      NIdV.Clr(false);
    }
  };
  
private:
  TkEgoVisitor Visitor;
  int k;
public:
  TFixedMemorykEgo(const PGraph& Graph, const int& k) : TFixedMemoryBFS<PGraph>(Graph), Visitor(TkEgoVisitor(Graph, k)), k(k) { }
  // Direction specific compute egonet statistics
  void ComputeInEgonet(const int& NId) {
    ComputeEgonet(NId, edInDirected);
  }
  void ComputeOutEgonet(const int& NId) {
    ComputeEgonet(NId, edOutDirected);
  }
  void ComputeEgonet(const int& NId) {
    ComputeEgonet(NId, edUnDirected);
  }
  // Compute egonet statistics using the direction specified
  void ComputeEgonet(const int& NId, const TEdgeDir& Dir);
  // Get results from the egonet traversal (only valid after compute method called)
  TIntV GetNIdV() { return Visitor.NIdV; }
  void Clr(const bool& DoDel = false);
};

template <class PGraph>
void TFixedMemorykEgo<PGraph>::ComputeEgonet(const int& NId, const TEdgeDir& Dir) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr();
  Visitor.SetEdgeDir(Dir);
  this->GetBfsVisitor<TkEgoVisitor>(Ego, Visitor, Dir, k);
}

}

#endif