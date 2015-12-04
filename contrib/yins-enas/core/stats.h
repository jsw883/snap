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
  return double(ClosedTriadsX) / double(ClosedTriadsX + OpenTriadsX);
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

// template <class PGraph>
// class TFixedMemorySubsetDiameter : public TFixedMemoryBFS<PGraph> {
// public:
//   // Backward / forward visitor (degree only)
//   class TSubsetDiameterVisitor {
//   public:
//     int nodes;
//     int diameter;
//   public:
//     TSubsetDiameterVisitor() : nodes(0), diameter(0) { }
//     void Start() { }
//     void DiscoverNode(const int& NId, const int& depth) {
//       nodes++;
//       if (depth > diameter) {
//         diameter = depth;
//       }
//     }
//     void FinishNode(const int& NId, const int& depth) { }
//     void ExamineEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
//     void TreeEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
//     void BackEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
//     void ForwardEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
//     void Finish() { }
//     void Clr() {
//       nodes = 0;
//       diameter = 0;
//     }
//   };
// private:
//   TSubsetDiameterVisitor Visitor;
// public:
//   TFixedMemorySubsetDiameter(const PGraph& Graph) : TFixedMemoryBFS<PGraph>(Graph), Visitor(TSubsetDiameterVisitor()) { }

//   // Get diameter and node count for a single node (int / out / undirected)
//   void ComputeInDiameter(const int& NId, int& nodes, int& diameter) {
//     ComputeDiameter(NId, edInDirected, nodes, diameter);
//   }
//   void ComputeOutDiameter(const int& NId, int& nodes, int& diameter) {
//     ComputeDiameter(NId, edOutDirected, nodes, diameter);
//   }
//   void ComputeDiameter(const int& NId, int& nodes, int& diameter) {
//     ComputeDiameter(NId, edUnDirected, nodes, diameter);
//   }
//   // Get diameter and node count for a single node using the direction specified 
//   void ComputeDiameter(const int& NId, const TEdgeDir& Dir, int& nodes, int& diameter);

//   // Get diameters and node counts for the subset of nodes (int / out / undirected)
//   void ComputeInSubsetDiameter(const TIntV& NIdV, TIntIntH& NodesH, TIntIntH& DiameterH) {
//     ComputeSubsetDiameter(NIdV, edInDirected, NodesH, DiameterH);
//   }
//   void ComputeOutSubsetDiameter(const TIntV& NIdV, TIntIntH& NodesH, TIntIntH& DiameterH) {
//     ComputeSubsetDiameter(NIdV, edOutDirected, NodesH, DiameterH);
//   }
//   void ComputeSubsetDiameter(const TIntV& NIdV, TIntIntH& NodesH, TIntIntH& DiameterH) {
//     ComputeSubsetDiameter(NIdV, edUnDirected, NodesH, DiameterH);
//   }
//   // Get diameters and node counts for the subset of nodes using the direction specified
//   void ComputeSubsetDiameter(const TIntV& NIdV, const TEdgeDir& Dir, TIntIntH& NodesH, TIntIntH& DiameterH);
  
//   void Clr(const bool& DoDel = false);
// };

// // Compute diameter using the direction specified
// template <class PGraph>
// void TFixedMemorySubsetDiameter<PGraph>::ComputeDiameter(const int& NId, const TEdgeDir& Dir, int& nodes, int& diameter) {
//   PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId);
//   Visitor.Clr();
//   this->GetBfsVisitor<TSubsetDiameterVisitor>(Ego, Visitor, Dir);
//   nodes = Visitor.nodes;
//   diameter = Visitor.diameter;
// }

// // Compute subset diameters using the direction specified
// template <class PGraph>
// void TFixedMemorySubsetDiameter<PGraph>::ComputeSubsetDiameter(const TIntV& NIdV, const TEdgeDir& Dir, TIntIntH& NodesH, TIntIntH& DiameterH) {
//   TIntV::TIter VI;
//   int nodes, diameter;
//   for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
//     ComputeDiameter(VI->Val, Dir, nodes, diameter);
//     NodesH.AddDat(VI->Val, nodes);
//     DiameterH.AddDat(VI->Val, diameter);
//   }
// }

// template <class PGraph>
// void TFixedMemorySubsetDiameter<PGraph>::Clr(const bool& DoDel) {
//   TFixedMemoryBFS<PGraph>::Clr(DoDel);
//   Visitor.Clr(); // resets the degree visitor to have zero nodes and diameter
// }



template <class PGraph>
class TFixedMemoryExactNF : public TFixedMemoryBFS<PGraph> {
public:
  // Backward / forward visitor (degree only)
  class TINFisitor {
  public:
    TIntV INF;
  public:
    TINFisitor() { }
    void Start() { }
    void DiscoverNode(const int& NId, const int& depth) {
      // printf("%d: %d\n", NId, depth);
      if (depth < INF.Len()) {
        INF[depth]++;
      } else {
        INF.Add(1);
        // if (INF.Empty()) {
        //   INF.Add(1);
        // } else {
        //   const int& temp = INF.Last();
        //   printf("temp: %d\n", temp);
        //   INF.Add(temp + 1);
        // }
      }
    }
    void FinishNode(const int& NId, const int& depth) { }
    void ExamineEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void TreeEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void BackEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void ForwardEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void Finish() {
      // INF[0]--;
    }
    void Clr() {
      INF.Clr();
    }
  };
private:
  TINFisitor Visitor;
public:
  TFixedMemoryExactNF(const PGraph& Graph) : TFixedMemoryBFS<PGraph>(Graph), Visitor(TINFisitor()) { }

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

// Compute INF using the direction specified
template <class PGraph>
void TFixedMemoryExactNF<PGraph>::ComputeINF(const int& NId, const TEdgeDir& Dir, TIntV& INF) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId);
  Visitor.Clr();
  this->GetBfsVisitor<TINFisitor>(Ego, Visitor, Dir);
  INF = Visitor.INF;
}

// Compute subset NF using the direction specified
template <class PGraph>
void TFixedMemoryExactNF<PGraph>::ComputeSubsetExactNF(const TIntV& NIdV, const TEdgeDir& Dir, TIntV& NF) {
  // Variables
  TIntV::TIter VI;
  TIntV INF;
  int depth;
  // Clear NF
  NF.Clr();
  // For each node in NIdV
  for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
    // Compute the INFH
    ComputeINF(VI->Val, Dir, INF);
    // Ensure that all depths exist in the NFH
    if (INF.Len() > NF.Len()) {
      for (int depth = NF.Len(); depth < INF.Len(); depth++) {
        NF.Add(0);
      }
    }
    // Increment depth counts
    for (int depth = 0; depth < INF.Len(); depth++) {
      NF[depth] += INF[depth];
    }
    // printf("Computed INF for NId: %d\n", VI->Val);
  }
  for (depth = 1; depth < NF.Len(); depth++) {
    NF[depth] += NF[depth - 1];
  }
}

// Compute subset INFH using the direction specified
template <class PGraph>
void TFixedMemoryExactNF<PGraph>::ComputeSubsetINFH(const TIntV& NIdV, const TEdgeDir& Dir, TIntIntVH& INFH) {
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
void TFixedMemoryExactNF<PGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<PGraph>::Clr(DoDel);
  Visitor.Clr(); // resets the degree visitor to have zero nodes and diameter
}

// Interpolate quantiles
double InterpolateNF(const TIntV& NF, const double& p);
void InterpolateINFH(const TIntIntVH& INFH, TIntIntH& QuantileH, const double& p);

void GetNodesINFH(const TIntIntVH& INFH, TIntIntH& NodesH);
void GetDiameterINFH(const TIntIntVH& INFH, TIntIntH& NodesH);

} // namespace TSnap


#endif
