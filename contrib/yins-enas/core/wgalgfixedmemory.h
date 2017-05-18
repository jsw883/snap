//#//////////////////////////////////////////////
/// Graph traversal algorithms

#ifndef core_wd
#define core_wd

// Headers (?)

#include "enums.h"

//#//////////////////////////////////////////////
/// Fixed memory weighted distance BFS
/// For efficient iterative egonet traversal of graphs that are very large or have a high density.

namespace TSnap { // should this be TSnapDetail (?)

/// Fixed memory BFS, where queues and hash tables are initialized (but the memory reserved) between different calls of GetBfsVisitor().
template <class PGraph> class TFixedMemoryWeightedDistanceBFS;

template <class PGraph>
class TFixedMemoryWeightedDistanceBFS {
public:
  typedef TQuad<TInt, TInt, TInt, TFlt> TQueueQuad;
private:
  // BFS memory
  PGraph Graph;
  TSnapQueue<TQueueQuad> Queue;
  // Visitor
  THash<TInt, TIntSet> PSetH;
  TIntFltH NIdVWDH;
  int k;
  double tol;
  bool preNormalized;
public:
  TFixedMemoryWeightedDistanceBFS(const PGraph& Graph, const TIntV& DstNIdV, const int& k, const double& tol, const bool& preNormalized) : Graph(Graph), Queue(Graph->GetNodes()), k(k), tol(tol), preNormalized(preNormalized) {
    // Initialize destination nodes with -1 weighted distances in case not discovered
    TIntV::TIter VI;
    for (VI = DstNIdV.BegI(); VI < DstNIdV.EndI(); VI++) {
      NIdVWDH.AddDat(VI->Val, 0.0); // Order is preserved
    }
  }
  void SetGraph(const PGraph& Graph);
  template <class TVisitor> void GetBfsVisitor(const int& NId, TVisitor& Visitor, const TEdgeDir& dir, const TIntSet& SkipNIdS);
  template <class TVisitor> void GetBfsVisitor(const int& NId, TVisitor& Visitor, const TEdgeDir& dir) {
    TIntSet SkipNIdS;
    GetBFS(NId, Visitor, dir, SkipNIdS);
  }
  
  void Clr(const bool& DoDel = false);
};

template<class PGraph>
void TFixedMemoryWeightedDistanceBFS<PGraph>::SetGraph(const PGraph& Graph) {
  Graph = Graph;
  const int N = Graph->GetNodes();
  if (Queue.Reserved() < N) { Queue.Gen(N); }
}

template <class PGraph> // class template still needs to be declared
template <class TVisitor> // member template
void TFixedMemoryWeightedDistanceBFS<PGraph>::GetBfsVisitor(const int& NId, TVisitor& Visitor, const TEdgeDir& dir, const TIntSet& SkipNIdS) { // ONLY DEFINED FOR A SINGLE NID FOR NOW
  // Clear memory
  Clr(false);
  // Variables
  int temp = 0, path = 0, depth = 0, edge = 0, Deg = 0, U = 0, V = 0;
  double WD = 0, VWD;
  typename PGraph::TObj::TNodeI NI, UI, VI;
  TIntSet IS;
  // BFS
  IS.AddKey(NId);
  PSetH.AddDat(path, IS);
  // temp++;
  Queue.Push(TQueueQuad(NId, depth, path, WD)); // NO IN-WEIGHT, SOURCE
  Visitor.DiscoverNode(U, depth, 0); // discover node
  while (!Queue.Empty()) {
    const TQueueQuad& Top = Queue.Top();
    U = Top.Val1; depth = Top.Val2; path = Top.Val3; WD = Top.Val4;
    UI = Graph->GetNI(U);
    Deg = UI.GetDeg(dir);
    edge = 0;
    Queue.Pop(); // deletes memory
    while (edge != Deg) {
      V = UI.GetNbrNId(edge, dir);
      Visitor.ExamineEdge(U, depth, edge, path, V); // examine edge

      if (!SkipNIdS.IsKey(V) && !PSetH.GetDat(path).IsKey(V)) {
        // WEIGHTED DISTANCE
        if (WD == 0) {
          VWD = UI.GetNbrEW(edge, dir);
          if (!preNormalized) { VWD /= UI.GetWDeg(dir); }
        } else {
          VWD = WD * UI.GetNbrEW(edge, dir);
          if (!preNormalized) { VWD /= UI.GetWDeg(dir); }
        }
        // STORE
        if (NIdVWDH.IsKey(V) && VWD > tol) {
          NIdVWDH.GetDat(V) += VWD;
          Visitor.DiscoverNode(V, depth + 1, VWD); // discover node
        }
        
        Visitor.TreeEdge(U, depth, edge, path, V); // tree edge
        // CONTINUE
        if (depth + 1 < k) { // || VWD > tol) {  // Changed AND to OR to allow depth past tolerance (counting)
          temp++;
          const TIntSet PSCopy = PSetH.GetDat(path); // Must copy
          TIntSet& PS = PSetH.AddDat(temp, PSCopy);
          PS.AddKey(V);
          Queue.Push(TQueueQuad(V, depth + 1, temp, VWD));
        }
      } else {
        Visitor.ExcludedEdge(U, depth, edge, path, V); // excluded edge
      }
      ++edge;
    }
    PSetH.DelKey(path);
    Visitor.FinishNode(U, depth, NIdVWDH.GetDat(U)); // finish
  }
  TFltV WDV;
  NIdVWDH.GetDatV(WDV);
  Visitor.Finish(WDV);
}

template <class PGraph>
void TFixedMemoryWeightedDistanceBFS<PGraph>::Clr(const bool& DoDel) {
  Queue.Clr(DoDel);
  // Visitor.Clr();
  TIntFltH::TIter HI;
  for (HI = NIdVWDH.BegI(); HI < NIdVWDH.EndI(); HI++) {
    HI.GetDat() = 0; // Reset path length within memory
  }
  PSetH.Clr(DoDel); // May need to force true to reduce memory
}

} // namespace TSnap

//#//////////////////////////////////////////////
/// Weighted distance neighborhood using BFS

namespace TSnap {




typedef THash<TInt, THash<TInt, TFlt> > TIntFltHH;

/// Fixed memory 
template <class PGraph>
class TFixedMemoryWeightedNeighborhood : public TFixedMemoryWeightedDistanceBFS<PGraph> {
public:
  // Backward / forward visitor (degree only)
  class TWeightedNeighborhoodVisitor {
  public:
    TIntIntHH PathsHH;
    TIntFltHH WDHH;
    int k;
  public:
    TWeightedNeighborhoodVisitor(const PGraph& Graph, const int& k) : PathsHH(Graph->GetNodes()), k(k) { }
    void Start() { }
    void DiscoverNode(const int& NId, const int& depth, const double& WD) {
      // Only store path counts for paths of length <= k (weighted distance will keep going)
      if (depth > 0 && depth <= k) {
        // Paths
        if (PathsHH.IsKey(NId)) {
          TIntIntH& PathH = PathsHH.GetDat(NId);
          if (PathH.IsKey(depth)) {
            PathH.GetDat(depth) ++;
          } else {
            PathH.AddDat(depth, 1);
          }
        } else {
          TIntIntH PathH;
          PathH.AddDat(depth, 1);
          PathsHH.AddDat(NId, PathH);
        }
        // WD
        if (WDHH.IsKey(NId)) {
          TIntFltH& WDH = WDHH.GetDat(NId);
          if (WDH.IsKey(depth)) {
            WDH.GetDat(depth) += WD;
          } else {
            WDH.AddDat(depth, WD);
          }
        } else {
          TIntFltH WDH;
          WDH.AddDat(depth, WD);
          WDHH.AddDat(NId, WDH);
        }
      }
    }
    void FinishNode(const int& NId, const int& depth, const double& WD) { }
    void ExamineEdge(const int& SrcNId, const int&  depth, const int& edge, const int& path, const int& DstNId) { }
    void TreeEdge(const int& SrcNId, const int&  depth, const int& edge, const int& path, const int& DstNId) { }
    void ExcludedEdge(const int& SrcNId, const int&  depth, const int& edge, const int& path, const int& DstNId) { }
    void Finish(const TFltV& WDV) { }
    void Clr() {
      PathsHH.Clr(false);
      WDHH.Clr(false);
    }
  };
private:
  TWeightedNeighborhoodVisitor Visitor;
public:
  TFixedMemoryWeightedNeighborhood(const PGraph& Graph, const TIntV& DstNIdV, const int& k, const double& tol, const bool& preNormalized) : TFixedMemoryWeightedDistanceBFS<PGraph>(Graph, DstNIdV, k, tol, preNormalized), Visitor(TWeightedNeighborhoodVisitor(Graph, k)) { }

  // // Compute neighborhood connectivity using the direction specified
  // void ComputeConnectivity(const int& NId, const TEdgeDir& Dir, const TIntSet& SkipNIdS, TIntIntHH& PathsHH);
  // // Compute subset neighborhood connectivity using the direction specified
  // void ComputeSubsetConnectivity(const TIntV& NIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, THash<TIntPr, TIntH>& PathsHH);

  // Compute neighborhood connectivity using the direction specified
  void ComputeConnectivity(const int& NId, const TEdgeDir& Dir, const TIntSet& SkipNIdS, TIntIntHH& PathsHH, TIntFltHH& WDHH);
  // Compute subset neighborhood connectivity using the direction specified
  void ComputeSubsetConnectivity(const TIntV& NIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, THash<TIntPr, TIntIntH>& PathsHH, THash<TIntPr, TIntFltH>& WDHH);

  void Clr(const bool& DoDel = false);
};

// // Compute neighborhood connectivity using the direction specified
// template <class PGraph>
// void TFixedMemoryWeightedNeighborhood<PGraph>::ComputeConnectivity(const int& NId, const TEdgeDir& Dir, const TIntSet& SkipNIdS, TIntIntHH& PathsHH) {
//   Visitor.Clr();
//   this->GetBfsVisitor(NId, Visitor, Dir, SkipNIdS);
//   PathsHH = Visitor.PathsHH;
// }


// // Compute subset neighborhood connectivity using the direction specified
// template <class PGraph>
// void TFixedMemoryWeightedNeighborhood<PGraph>::ComputeSubsetConnectivity(const TIntV& NIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, THash<TIntPr, TIntH>& PathsHH) {
//   // Variables
//   TIntV::TIter VI;
//   int NId;
//   TIntIntHH NIdPathsHH;
//   TIntIntHH::TIter HI;
//   // For each node in NIdV
//   for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
//     NIdPathsHH.Clr(false);
//     NId = VI->Val;
//     ComputeConnectivity(NId, dir, SkipNIdS, NIdPathsHH);
//     for (HI = NIdPathsHH.BegI(); HI < NIdPathsHH.EndI(); HI++) {
//       PathsHH.AddDat(TIntPr(NId, HI.GetKey()), HI.GetDat());
//     }
//   }
// }

// Compute neighborhood connectivity using the direction specified
template <class PGraph>
void TFixedMemoryWeightedNeighborhood<PGraph>::ComputeConnectivity(const int& NId, const TEdgeDir& Dir, const TIntSet& SkipNIdS, TIntIntHH& PathsHH, TIntFltHH& WDHH) {
  Visitor.Clr();
  this->GetBfsVisitor(NId, Visitor, Dir, SkipNIdS);
  PathsHH = Visitor.PathsHH;
  WDHH = Visitor.WDHH;
}

// Compute subset neighborhood connectivity using the direction specified
template <class PGraph>
void TFixedMemoryWeightedNeighborhood<PGraph>::ComputeSubsetConnectivity(const TIntV& NIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, THash<TIntPr, TIntH>& PathsHH, THash<TIntPr, TIntFltH>& WDHH) {
  // Variables
  TIntV::TIter VI;
  int NId;
  TIntIntHH NIdPathsHH;
  TIntFltHH NIdWDHH;
  TIntIntHH::TIter NIdPathsHI;
  TIntFltHH::TIter WDHI;
  // For each node in NIdV
  for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
    NIdPathsHH.Clr(false);
    NIdWDHH.Clr(false);
    NId = VI->Val;
    ComputeConnectivity(NId, dir, SkipNIdS, NIdPathsHH, NIdWDHH);
    for (NIdPathsHI = NIdPathsHH.BegI(); NIdPathsHI < NIdPathsHH.EndI(); NIdPathsHI++) {
      PathsHH.AddDat(TIntPr(NId, NIdPathsHI.GetKey()), NIdPathsHI.GetDat());
    }
    for (WDHI = NIdWDHH.BegI(); WDHI < NIdWDHH.EndI(); WDHI++) {
      WDHH.AddDat(TIntPr(NId, WDHI.GetKey()), WDHI.GetDat());
    }
  }
}

} // namespace TSnap

#endif  