#ifndef core_stats
#define core_stats

// Headers (?)

#include "galg.h"

//#//////////////////////////////////////////////
/// Clustering coefficients

namespace TSnap {

// Computes global clustering coefficient (need to check this for method)
template<class PGraph> double GetGlobClustCf(const PGraph& Graph, int SampleNodes = -1);
// Computes average clustering coefficient (need to check this for method)
template<class PGraph> double GetAvClustCf(const PGraph& Graph, int SampleNodes = -1);

// Computes local clustering coefficient for each node in NIdV (returns average)
template<class PGraph> double GetAvLocalClustCoeff(const PGraph& Graph, const TIntV& NIdV, TIntFltH& NIdClustCoeffH);
// Computes average local clustering coefficient (entire graph)
template<class PGraph> double GetAvLocalClustCoeff(const PGraph& Graph);
  
// Compute directed binary local clustering coefficients as defined in Clustering in Complex Directed Networks [Fagiolo, 2007]
template<class PGraph> double GetDirLocalClustCoeff(const PGraph& Graph, const int& NId, double& CycleCoeff, double& MidCoeff, double& InCoeff, double& OutCoeff);
// Should work for undirected graphs by overcounting 
template<class PGraph> double GetDirLocalClustCoeff(const PGraph& Graph, const int& NId);

// Computes global clustering coefficient (need to check this for method)
template<class PGraph>
double GetGlobClustCf(const PGraph& Graph, int SampleNodes) {
  int64 ClosedTriadsX, OpenTriadsX;
  GetTriads(Graph, ClosedTriadsX, OpenTriadsX, SampleNodes);
  if (OpenTriadsX == 0) {
    return 0;
  } else {
    return double(ClosedTriadsX) / double(ClosedTriadsX + OpenTriadsX);
  }
}

// Computes average clustering coefficient (need to check this for method)
template<class PGraph> double GetAvClustCf(const PGraph& Graph, int SampleNodes) {
  return GetClustCf(Graph, SampleNodes = -1);
}

// Computes local clustering coefficient for each node in NIdV (returns average)
template<class PGraph>
double GetAvLocalClustCoeff(const PGraph& Graph, const TIntV& NIdV, TIntFltH& NIdClustCoeffH) {
  // Variables
  // PGraph::TObj::TNodeI NI;
  TIntV::TIter NI;
  double Coeff, AvCoeff = 0;
  // Check empty
  NIdClustCoeffH.Clr();
  // For each node in NIdV
  for (NI = NIdV.BegI(); NI < NIdV.EndI(); NI++) {
    const int& NId = NI->Val;
    Coeff = GetDirLocalClustCoeff(Graph, NId);
    NIdClustCoeffH.AddDat(NId, Coeff);
    AvCoeff += Coeff;
  }
  return AvCoeff / ((double) NIdV.Len());
}

// Computes average local clustering coefficient (entire graph)
template<class PGraph>
double GetAvLocalClustCoeff(const PGraph& Graph) {
  // Variables
  TIntV NIdV; Graph->GetNIdV(NIdV);
  TIntFltH NIdClustCoeffH;
  return GetAvLocalClustCoeff(Graph, NIdV, NIdClustCoeffH);
}

// Compute directed binary local clustering coefficients as defined in Clustering in Complex Directed Networks [Fagiolo, 2007]
template<class PGraph>
double GetDirLocalClustCoeff(const PGraph& Graph, const int& NId, double& CycleCoeff, double& MidCoeff, double& InCoeff, double& OutCoeff) {
  // Check multigraph
  IAssertR(!Graph->HasFlag(gfMultiGraph), "Clustering coefficients are not defined for multigraphs of any kind.");
  
  typename PGraph::TObj::TNodeI NI = Graph->GetNI(NId);
  // Variables
  TIntSet OutS, InS;  // Hash backed
  int CycleFreq = 0, MidFreq = 0, InFreq = 0, OutFreq = 0;
  int OutNId, InNId;
  int OutDeg = NI.GetOutDeg(), InDeg = NI.GetInDeg(), Deg = OutDeg + InDeg, BiDeg = 0;
  // Collect nodes in OUT
  for (int e = 0; e < OutDeg; e++) {
    OutS.AddKey(NI.GetOutNId(e));
  }
  // Collect nodes in IN
  for (int e = 0; e < InDeg; e++) {
    InNId = NI.GetInNId(e);
    InS.AddKey(InNId);
    if (OutS.IsKey(InNId)) {
      BiDeg++;
    }
  }
  // Edge cases
  if (Deg < 2 || InDeg*OutDeg*BiDeg == 1) {
    CycleCoeff = MidCoeff = InCoeff = OutCoeff = 0;
    return 0;
  }
  // OUT
  for (int j = 0; j < OutS.Len(); j++) {
    NI = Graph->GetNI(OutS.GetKey(j));
    // OUT x OUT (unique)
    for (int k = j + 1; k < OutS.Len(); k++) {
      OutNId = OutS.GetKey(k);
      if (NI.IsOutNId(OutNId)) {
        OutFreq++;
      }
      if (NI.IsInNId(OutNId)) {
        OutFreq++;
      }
    }
    // OUT x IN (complete)
    for (int k = 0; k < InS.Len(); k++) {
      InNId = InS.GetKey(k);
      if (NI.IsOutNId(InNId)) {
        CycleFreq++;
      }
      if (NI.IsInNId(InNId)) {
        MidFreq++;
      }
    }
  }
  // IN
  for (int j = 0; j < InS.Len(); j++) {
    NI = Graph->GetNI(InS.GetKey(j));
    // IN x IN (unique)
    for (int k = j + 1; k < InS.Len(); k++) {
      InNId = InS.GetKey(k);
      if (NI.IsOutNId(InNId)) {
        InFreq++;
      }
      if (NI.IsInNId(InNId)) {
        InFreq++;
      }
    }
  }
  
  // Compute directed coefficients
  CycleCoeff = InDeg*OutDeg == 0 ? 0 : ((double) CycleFreq) / ((double) InDeg*OutDeg - BiDeg);
  MidCoeff = InDeg*OutDeg == 0 ? 0 : ((double) MidFreq) / ((double) InDeg*OutDeg - BiDeg);
  InCoeff = InDeg < 2 ? 0 : ((double) InFreq) / ((double) InDeg*(InDeg - 1));
  OutCoeff = OutDeg < 2 ? 0 : ((double) OutFreq) / ((double) OutDeg*(OutDeg - 1));
  // Return undirected coefficient for overloading
  return ((double) CycleFreq + MidFreq + InFreq + OutFreq) /
    ((double) Deg*(Deg - 1) - 2*BiDeg);
}

// Should work for undirected graphs by overcounting 
template<class PGraph>
double GetDirLocalClustCoeff(const PGraph& Graph, const int& NId) {
  double CycleCoeff, MidCoeff, InCoeff, OutCoeff;
  return GetDirLocalClustCoeff(Graph, NId, CycleCoeff, MidCoeff, InCoeff, OutCoeff);
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
/// Approximate neighborhood function using ANF

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

} // namespace TSnap

//#//////////////////////////////////////////////
/// Exact neighborhood function using BFS

namespace TSnap {

// Convert neighborhood to NF (and NF to neighborhood)
void ConvertNeighborhoodNF(TUInt64V& Neighborhood);
void ConvertNFNeighborhood(TUInt64V& NF);

// Convert neighborhood to INF for subset of nodes (and INF to neighborhood)
void ConvertSubsetNeighborhoodHSubsetINFH(THash<TInt, TUInt64V>& NeighborhoodH);
void ConvertSubsetINFHSubsetNeighborhoodH(THash<TInt, TUInt64V>& INFH);

// Compute subset NF from subset neighborhood (and from subset INF)
void ConvertSubsetNeighborhoodHSubsetNF(const THash<TInt, TUInt64V>& NeighborhoodH, TUInt64V& NF);
void ConvertSubsetINFHSubsetNF(THash<TInt, TUInt64V>& INFH, TUInt64V& NF);

/// Fixed memory 
template <class PGraph>
class TFixedMemoryNeighborhood : public TFixedMemoryBFS<PGraph> {
public:
  // Backward / forward visitor (degree only)
  class TNeighborhoodVisitor {
  public:
    TUInt64V Neighborhood;
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
  void ComputeNeighborhood(const int& NId, const TEdgeDir& Dir, TUInt64V& Neigborhood);
  // Get INF for a single node using the direction specified 
  void ComputeINF(const int& NId, const TEdgeDir& Dir, TUInt64V& INF);

  // Get neighborhood for the subset of nodes using the direction specified
  void ComputeSubsetNeighborhoodH(const TIntV& NIdV, const TEdgeDir& Dir, THash<TInt, TUInt64V>& NeighborhoodH);
  // Get INF for the subset of nodes using the direction specified
  void ComputeSubsetINFH(const TIntV& NIdV, const TEdgeDir& Dir, THash<TInt, TUInt64V>& INFH);
  
  // Get exact NF for the subset of nodes (int / out / undirected)
  void ComputeInSubsetNF(const TIntV& NIdV, TUInt64V& NF) {
    ComputeSubsetNF(NIdV, edInDirected, NF);
  }
  void ComputeOutSubsetNF(const TIntV& NIdV, TUInt64V& NF) {
    ComputeSubsetNF(NIdV, edOutDirected, NF);
  }
  void ComputeSubsetNF(const TIntV& NIdV, TUInt64V& NF) {
    ComputeSubsetNF(NIdV, edUnDirected, NF);
  }
  // Get exact NF for the subset of nodes using the direction specified
  void ComputeSubsetNF(const TIntV& NIdV, const TEdgeDir& Dir, TUInt64V& NF);
  
  // Get exact NF for the entire graph (int / out / undirected)
  void ComputeInNF(TUInt64V& NF) {
    TIntV NIdV; this->Graph->GetNIdV(NIdV);
    ComputeSubsetNF(NIdV, edInDirected, NF);
  }
  void ComputeOutNF(TUInt64V& NF) {
    TIntV NIdV; this->Graph->GetNIdV(NIdV);
    ComputeSubsetNF(NIdV, edOutDirected, NF);
  }
  void ComputeNF(TUInt64V& NF) {
    TIntV NIdV; this->Graph->GetNIdV(NIdV);
    ComputeSubsetNF(NIdV, edUnDirected, NF);
  }
  
  void Clr(const bool& DoDel = false);
};

// Compute neighborhood depth counts using the direction specified
template <class PGraph>
void TFixedMemoryNeighborhood<PGraph>::ComputeNeighborhood(const int& NId, const TEdgeDir& Dir, TUInt64V& Neighborhood) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId);
  Visitor.Clr();
  this->GetBfsVisitor<TNeighborhoodVisitor>(Ego, Visitor, Dir);
  Neighborhood = Visitor.Neighborhood;
}

// Compute INF using the direction specified
template <class PGraph>
void TFixedMemoryNeighborhood<PGraph>::ComputeINF(const int& NId, const TEdgeDir& Dir, TUInt64V& INF) {
  ComputeNeighborhood(NId, Dir, INF);
  ConvertNeighborhoodNF(INF);
}

// Compute subset neighborhood depth counts using the direction specified
template <class PGraph>
void TFixedMemoryNeighborhood<PGraph>::ComputeSubsetNeighborhoodH(const TIntV& NIdV, const TEdgeDir& Dir, THash<TInt, TUInt64V>& NeighborhoodH) {
  // Variables
  TIntV::TIter VI;
  TUInt64V Neighborhood;
  int NId;
  // Clear outputs
  NeighborhoodH.Clr();
  // For each node in NIdV
  for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
    NId = VI->Val;
    // Compute the INFH
    ComputeNeighborhood(NId, Dir, Neighborhood);
    // Add INFH
    NeighborhoodH.AddDat(NId, Neighborhood);
  }
}

// Compute subset INFH using the direction specified
template <class PGraph>
void TFixedMemoryNeighborhood<PGraph>::ComputeSubsetINFH(const TIntV& NIdV, const TEdgeDir& Dir, THash<TInt, TUInt64V>& INFH) {
  // Variables
  TIntV::TIter VI;
  TUInt64V INF;
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

// Compute subset NF using the direction specified
template <class PGraph>
void TFixedMemoryNeighborhood<PGraph>::ComputeSubsetNF(const TIntV& NIdV, const TEdgeDir& Dir, TUInt64V& NF) {
  // Variables
  THash<TInt, TUInt64V>::TIter HI;
  THash<TInt, TUInt64V> NeighborhoodH;
  // Compute neighborhoods
  ComputeSubsetNeighborhoodH(NIdV, Dir, NeighborhoodH);
  ConvertSubsetNeighborhoodHSubsetNF(NeighborhoodH, NF);
}

template <class PGraph>
void TFixedMemoryNeighborhood<PGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<PGraph>::Clr(DoDel);
  Visitor.Clr(); // resets the degree visitor to have zero nodes and diameter
}

// Interpolate NF
double InterpolateNF(const TUInt64V& NF, const double& p);
void InterpolateINFH(const THash<TInt, TUInt64V>& INFH, TIntFltH& QuantileH, const double& p);

// Get extrema (diameter and nodes discovered)
void GetNodesINFH(const THash<TInt, TUInt64V>& INFH, TIntIntH& NodesH);
void GetDiameterINFH(const THash<TInt, TUInt64V>& INFH, TIntIntH& DiameterH);

} // namespace TSnap


#endif
