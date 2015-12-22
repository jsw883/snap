#ifndef core_stats
#define core_stats

// Headers (?)

#include "galg.h"

//#//////////////////////////////////////////////
/// Clustering coefficients

namespace TSnap {

// Computes global clustering coefficient (need to check this for method)
template<class PGraph> double GetGlobClustCf(PGraph& Graph, int SampleNodes = -1);
// Computes average clustering coefficient (need to check this for method)
template<class PGraph> double GetAvClustCf(PGraph& Graph, int SampleNodes = -1);

// Computes global clustering coefficient (need to check this for method)
template<class PGraph>
double GetGlobClustCf(PGraph& Graph, int SampleNodes) {
  int64 ClosedTriadsX, OpenTriadsX;
  GetTriads(Graph, ClosedTriadsX, OpenTriadsX, SampleNodes);
  if (OpenTriadsX == 0) {
    return 0;
  } else {
    return double(ClosedTriadsX) / double(ClosedTriadsX + OpenTriadsX);
  }
}

// Computes average clustering coefficient (need to check this for method)
template<class PGraph> double GetAvClustCf(PGraph& Graph, int SampleNodes) {
  return GetClustCf(Graph, SampleNodes = -1);
}

} // namespace TSnap

//#//////////////////////////////////////////////
/// Degrees

namespace TSnap {

// Computes maximum degree (undirected)
template <class PGraph>
int GetMxDeg(PGraph& Graph) {
  typename PGraph::TObj::TNodeI NI;
  int MxDeg = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    if (MxDeg < NI.GetDeg()) {
      MxDeg = NI.GetDeg();
    }
  }
  return MxDeg;
}

// Computes maximum degree (in / out)
int GetMxInDeg(PNGraph& Graph);
int GetMxOutDeg(PNGraph& Graph);

} // namespace TSnap


//#//////////////////////////////////////////////
/// Exact neighborhood function using BFS

namespace TSnap {

// Computes given percentiles of the shortest path cumulative density (multiple runs of ANF)
template<class PGraph>
void GetAnfEffDiam(const PGraph& Graph, const bool& IsDir, const TFltV& Percentiles, TFltV& PValues, const int NRuns = 1, int NApprox = -1) {
  typename TFltV::TIter PI;
  typename TMomV::TIter MomI;
  TMomV MomV;
  TMom::NewV(MomV, Percentiles.Len());
  if (NApprox == -1) {
    if (Graph->GetNodes() < 100000) { NApprox = 64; }
    else if (Graph->GetNodes() < 1000000) { NApprox = 32; }
    else { NApprox = 16; }
  }
  for (int r = 0; r < NRuns; r++) {
    for (MomI = MomV.BegI(), PI = Percentiles.BegI(); MomI < MomV.EndI() || PI < Percentiles.EndI(); MomI++, PI++) {
      (*MomI)->Add(TSnap::GetAnfEffDiam(Graph, IsDir, PI->Val, NApprox));
    }
  }
  TMom::DefV(MomV);
  for (MomI = MomV.BegI(); MomI < MomV.EndI(); MomI++) {
    PValues.Add((*MomI)->GetMean());
  }
}

template <class PGraph>
class TFixedMemoryNeighborhood : public TFixedMemoryBFS<PGraph> {
public:
  // Backward / forward visitor (degree only)
  class TNeighborhoodVisitor {
  public:
    TIntV Neighborhood;
  public:
    TNeighborhoodVisitor() { }
    void Start() { }
    void DiscoverNode(const int& NId, const int& depth) {
      // printf("%d: %d\n", NId, depth);
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
      Neighborhood.Clr();
    }
  };
private:
  TNeighborhoodVisitor Visitor;
public:
  TFixedMemoryNeighborhood(const PGraph& Graph) : TFixedMemoryBFS<PGraph>(Graph), Visitor(TNeighborhoodVisitor()) { }

// Compute neighborhood depth counts using the direction specified
  void ComputeNeighborhood(const int& NId, const TEdgeDir& Dir, TIntV& Neigborhood);

  // Get INF for a single node (int / out / undirected)
  void ComputeInINF(const int& NId, TIntV& INF) {
    ComputeINF(NId, edInDirected, INF);
  }
  void ComputeOutINF(const int& NId, TIntV& INF) {
    ComputeINF(NId, edOutDirected, INF);
  }
  void ComputeINF(const int& NId, TIntV& INF) {
    ComputeINF(NId, edUnDirected, INF);
  }
  // Get INF for a single node using the direction specified 
  void ComputeINF(const int& NId, const TEdgeDir& Dir, TIntV& INF);

  // Get exact NF for the subset of nodes (int / out / undirected)
  void ComputeInSubsetExactNF(const TIntV& NIdV, TIntV& NF) {
    ComputeSubsetExactNF(NIdV, edInDirected, NF);
  }
  void ComputeOutSubsetExactNF(const TIntV& NIdV, TIntV& NF) {
    ComputeSubsetExactNF(NIdV, edOutDirected, NF);
  }
  void ComputeSubsetExactNF(const TIntV& NIdV, TIntV& NF) {
    ComputeSubsetExactNF(NIdV, edUnDirected, NF);
  }
  // Get exact NF for the subset of nodes using the direction specified
  void ComputeSubsetExactNF(const TIntV& NIdV, const TEdgeDir& Dir, TIntV& NF);
  // Get INF and diameters for the subset of nodes using the direction specified
  void ComputeSubsetINFH(const TIntV& NIdV, const TEdgeDir& Dir, TIntIntVH& INFH);
  
  void Clr(const bool& DoDel = false);
};

// Compute neighborhood depth counts using the direction specified
template <class PGraph>
void TFixedMemoryNeighborhood<PGraph>::ComputeNeighborhood(const int& NId, const TEdgeDir& Dir, TIntV& Neighborhood) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId);
  Visitor.Clr();
  this->GetBfsVisitor<TNeighborhoodVisitor>(Ego, Visitor, Dir);
  Neighborhood = Visitor.Neighborhood;
}

// Compute INF using the direction specified
template <class PGraph>
void TFixedMemoryNeighborhood<PGraph>::ComputeINF(const int& NId, const TEdgeDir& Dir, TIntV& INF) {
  ComputeNeighborhood(NId, Dir, INF);
  // Aggregate
  for (int depth = 1; depth < INF.Len(); depth++) {
    INF[depth] += INF[depth - 1];
  }
}

// Compute subset NF using the direction specified
template <class PGraph>
void TFixedMemoryNeighborhood<PGraph>::ComputeSubsetExactNF(const TIntV& NIdV, const TEdgeDir& Dir, TIntV& NF) {
  // Variables
  TIntV::TIter VI;
  TIntV Neighborhood;
  int depth;
  // Clear NF
  NF.Clr();
  // For each node in NIdV
  for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
    // Compute the INFH
    ComputeNeighborhood(VI->Val, Dir, Neighborhood);
    // Ensure that all depths exist in the NFH
    if (Neighborhood.Len() > NF.Len()) {
      for (int depth = NF.Len(); depth < Neighborhood.Len(); depth++) {
        NF.Add(0);
      }
    }
    // Increment depth counts
    for (int depth = 0; depth < Neighborhood.Len(); depth++) {
      NF[depth] += Neighborhood[depth];
    }
  }
  // Aggregate
  for (depth = 1; depth < NF.Len(); depth++) {
    NF[depth] += NF[depth - 1];
  }
}

// Compute subset INFH using the direction specified
template <class PGraph>
void TFixedMemoryNeighborhood<PGraph>::ComputeSubsetINFH(const TIntV& NIdV, const TEdgeDir& Dir, TIntIntVH& INFH) {
  // Variables
  TIntV::TIter VI;
  TIntV INF;
  int NId;
  // Clear outputs
  INFH.Clr();
  // For each node in NIdV
  for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
    NId = VI->Val;
    // Compute the INFH
    ComputeINF(NId, Dir, INF);
    // Add INFH
    INFH.AddDat(NId, INF);
  }
}


template <class PGraph>
void TFixedMemoryNeighborhood<PGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<PGraph>::Clr(DoDel);
  Visitor.Clr(); // resets the degree visitor to have zero nodes and diameter
}

// Interpolate quantiles
double InterpolateNF(const TIntV& NF, const double& p);
void InterpolateINFH(const TIntIntVH& INFH, TIntFltH& QuantileH, const double& p);

void GetNodesINFH(const TIntIntVH& INFH, TIntIntH& NodesH);
void GetDiameterINFH(const TIntIntVH& INFH, TIntIntH& DiameterH);

} // namespace TSnap


#endif
