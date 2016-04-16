//#//////////////////////////////////////////////
/// Graph traversal algorithms

#ifndef core_galgcustom
#define core_galgcustom

// Headers (?)

#include "enums.h"
#include "galgfixedmemory.h"

//#//////////////////////////////////////////////
/// Exact neighborhood function using BFS

namespace TSnap {

/// Fixed memory 
template <class PGraph>
class TFixedMemoryExhaustiveNeighborhood : public TFixedMemoryBFS<PGraph> {
public:
  // Backward / forward visitor (degree only)
  class TExhaustiveNeighborhoodVisitor {
  public:
    TIntIntH NIdVShortestPathLengthH;
    TUInt64V Neighborhood; // Neighborhood function (we get this for free)
  public:
    TExhaustiveNeighborhoodVisitor(const TIntV& DstNIdV) {
      // Initialize destination nodes with infinite path length in case not discovered
      TIntV::TIter VI;
      for (VI = DstNIdV.BegI(); VI < DstNIdV.EndI(); VI++) {
        NIdVShortestPathLengthH.AddDat(VI->Val, -1); // Order is preserved
      }
    }
    void Start() { }
    void DiscoverNode(const int& NId, const int& depth) {
      // Shortest path lengths
      if (NIdVShortestPathLengthH.IsKey(NId)) {
        NIdVShortestPathLengthH.GetDat(NId) = depth;
      }
      // NF
      if (depth < Neighborhood.Len()) {
        Neighborhood[depth]++;
      } else {
        Neighborhood.Add(1);
      }
    }
    void FinishNode(const int& NId, const int& depth) { }
    void ExamineEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void TreeEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void BackEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void ForwardEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void Finish() { }
    void Clr() {
      TIntIntH::TIter HI;
      for (HI = NIdVShortestPathLengthH.BegI(); HI < NIdVShortestPathLengthH.EndI(); HI++) {
        HI.GetDat() = -1; // Reset path length within memory
      }
      Neighborhood.Clr();
    }
  };
private:
  TExhaustiveNeighborhoodVisitor Visitor;
public:
  TFixedMemoryExhaustiveNeighborhood(const PGraph& Graph, const TIntV& DstNIdV) : TFixedMemoryBFS<PGraph>(Graph), Visitor(TExhaustiveNeighborhoodVisitor(DstNIdV)) { }

  // Compute neighborhood depth counts using the direction specified
  void ComputeNeighborhood(const int& NId, const TEdgeDir& Dir, TUInt64V& Neigborhood, TIntV& ShortestPathV);
  // Get INF for a single node using the direction specified 
  void ComputeINF(const int& NId, const TEdgeDir& Dir, TUInt64V& INF, TIntV& ShortestPathV);

  // Get neighborhood for the subset of nodes using the direction specified
  void ComputeSubsetNeighborhoodH(const TIntV& NIdV, const TEdgeDir& Dir, THash<TInt, TUInt64V>& NeighborhoodH, TIntIntVH& DstNIdVShortestPathVH);
  // Get INF for the subset of nodes using the direction specified
  void ComputeSubsetINFH(const TIntV& NIdV, const TEdgeDir& Dir, THash<TInt, TUInt64V>& INFH, TIntIntVH& DstNIdVShortestPathVH);
  
  // Get exact NF for the subset of nodes (int / out / undirected)
  void ComputeInSubsetNF(const TIntV& NIdV, TUInt64V& NF, TIntIntVH& DstNIdVShortestPathVH) {
    ComputeSubsetNF(NIdV, edInDirected, NF, DstNIdVShortestPathVH);
  }
  void ComputeOutSubsetNF(const TIntV& NIdV, TUInt64V& NF, TIntIntVH& DstNIdVShortestPathVH) {
    ComputeSubsetNF(NIdV, edOutDirected, NF, DstNIdVShortestPathVH);
  }
  void ComputeSubsetNF(const TIntV& NIdV, TUInt64V& NF, TIntIntVH& DstNIdVShortestPathVH) {
    ComputeSubsetNF(NIdV, edUnDirected, NF, DstNIdVShortestPathVH);
  }
  // Get exact NF for the subset of nodes using the direction specified
  void ComputeSubsetNF(const TIntV& NIdV, const TEdgeDir& Dir, TUInt64V& NF, TIntIntVH& DstNIdVShortestPathVH);
  
  // Get exact NF for the entire graph (int / out / undirected)
  void ComputeInNF(TUInt64V& NF, TIntIntVH& DstNIdVShortestPathVH) {
    TIntV NIdV; this->Graph->GetNIdV(NIdV);
    ComputeSubsetNF(NIdV, edInDirected, NF, DstNIdVShortestPathVH);
  }
  void ComputeOutNF(TUInt64V& NF, TIntIntVH& DstNIdVShortestPathVH) {
    TIntV NIdV; this->Graph->GetNIdV(NIdV);
    ComputeSubsetNF(NIdV, edOutDirected, NF, DstNIdVShortestPathVH);
  }
  void ComputeNF(TUInt64V& NF, TIntIntVH& DstNIdVShortestPathVH) {
    TIntV NIdV; this->Graph->GetNIdV(NIdV);
    ComputeSubsetNF(NIdV, edUnDirected, NF, DstNIdVShortestPathVH);
  }
  

  void Clr(const bool& DoDel = false);
};

// Compute neighborhood depth counts using the direction specified
template <class PGraph>
void TFixedMemoryExhaustiveNeighborhood<PGraph>::ComputeNeighborhood(const int& NId, const TEdgeDir& Dir, TUInt64V& Neighborhood, TIntV& ShortestPathV) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId);
  Visitor.Clr();
  this->GetBfsVisitor(Ego, Visitor, Dir);
  Neighborhood = Visitor.Neighborhood;
  Visitor.NIdVShortestPathLengthH.GetDatV(ShortestPathV);
}

// Compute INF using the direction specified
template <class PGraph>
void TFixedMemoryExhaustiveNeighborhood<PGraph>::ComputeINF(const int& NId, const TEdgeDir& Dir, TUInt64V& INF, TIntV& ShortestPathV) {
  ComputeNeighborhood(NId, Dir, INF, ShortestPathV);
  ConvertNeighborhoodNF(INF);
}

// Compute subset neighborhood depth counts using the direction specified
template <class PGraph>
void TFixedMemoryExhaustiveNeighborhood<PGraph>::ComputeSubsetNeighborhoodH(const TIntV& NIdV, const TEdgeDir& Dir, THash<TInt, TUInt64V>& NeighborhoodH, TIntIntVH& DstNIdVShortestPathVH) {
  // Variables
  TIntV::TIter VI;
  TUInt64V Neighborhood;
  TIntV ShortestPathV;
  int NId;
  // Clear outputs
  NeighborhoodH.Clr();
  // For each node in NIdV
  for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
    NId = VI->Val;
    // Compute the INFH
    ComputeNeighborhood(NId, Dir, Neighborhood, ShortestPathV);
    // Add INFH
    NeighborhoodH.AddDat(NId, Neighborhood);
    DstNIdVShortestPathVH.AddDat(NId, ShortestPathV);
  }
}

// Compute subset INFH using the direction specified
template <class PGraph>
void TFixedMemoryExhaustiveNeighborhood<PGraph>::ComputeSubsetINFH(const TIntV& NIdV, const TEdgeDir& Dir, THash<TInt, TUInt64V>& INFH, TIntIntVH& DstNIdVShortestPathVH) {
  // Variables
  TIntV::TIter VI;
  TUInt64V INF;
  TIntV ShortestPathV;
  int NId;
  // Clear outputs
  INFH.Clr();
  // For each node in NIdV
  for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
    NId = VI->Val;
    // Compute the INFH
    ComputeINF(NId, Dir, INF, ShortestPathV);
    // Add INFH
    INFH.AddDat(NId, INF);
    DstNIdVShortestPathVH.AddDat(NId, ShortestPathV);
  }
}

// Compute subset NF using the direction specified
template <class PGraph>
void TFixedMemoryExhaustiveNeighborhood<PGraph>::ComputeSubsetNF(const TIntV& NIdV, const TEdgeDir& Dir, TUInt64V& NF, TIntIntVH& DstNIdVShortestPathVH) {
  // Variables
  THash<TInt, TUInt64V>::TIter HI;
  THash<TInt, TUInt64V> NeighborhoodH;
  // Compute neighborhoods
  ComputeSubsetNeighborhoodH(NIdV, Dir, NeighborhoodH, DstNIdVShortestPathVH);
  ConvertSubsetNeighborhoodHSubsetNF(NeighborhoodH, NF);
}

template <class PGraph>
void TFixedMemoryExhaustiveNeighborhood<PGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<PGraph>::Clr(DoDel);
  Visitor.Clr();
}

} // namespace TSnap

#endif