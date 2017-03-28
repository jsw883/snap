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
  
  // // Get weighted distances using the direction specified
  // void ComputeWDV(const int& NId, const TEdgeDir& dir, const TIntSet& SkipNIdS, TFltV& WDV);
  // void ComputeWDV(const int& NId, const TEdgeDir& dir, TFltV& WDV) { TIntSet SkipNIdS; ComputeWDV(NId, dir, SkipNIdS, WDV); }
  // // Get weighted distances for the subset of nodes using the direction specified
  // void ComputeSubsetWDVH(const TIntV& NIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, TIntFltVH& WDVH);
  // void ComputeSubsetWDVH(const TIntV& NIdV, const TEdgeDir& dir, TIntFltVH& WDVH) { TIntSet SkipNIdS; ComputeSubsetWDVH(NIdV, dir, SkipNIdS, WDVH); }
  
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

    // printf("U: %d, depth: %d, path: %d, WD: %f, IsNode: %d, Deg: %d)\n", U, depth, path, WD, Graph->IsNode(U), Deg);
  
    while (edge != Deg) {
      V = UI.GetNbrNId(edge, dir);
      Visitor.ExamineEdge(U, depth, edge, path, V); // examine edge

      // printf(" -> %d: (V: %d", edge, V);

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
        }
        Visitor.DiscoverNode(V, depth + 1, NIdVWDH.GetDat(V)); // discover node
        Visitor.TreeEdge(U, depth, edge, path, V); // tree edge

        // printf(", VWD: %f, VWDTotal: %f", VWD, (double) NIdVWDH.GetDat(V));

        // CONTINUE
        if (depth + 1 < k) { // || VWD > tol) {  // Changed AND to OR to allow depth past tolerance (counting)
          temp++;
          const TIntSet PSCopy = PSetH.GetDat(path); // Must copy
          TIntSet& PS = PSetH.AddDat(temp, PSCopy);
          PS.AddKey(V);
          Queue.Push(TQueueQuad(V, depth + 1, temp, VWD));

          // printf(", continue: yes");

        } else {

          // printf(", continue: no");

        }

        // printf(")\n");

      } else {
        Visitor.ExcludedEdge(U, depth, edge, path, V); // excluded edge

        // printf(", excluded)\n");

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


// // Compute neighborhood depth counts using the direction specified
// template <class PGraph>
// void TFixedMemoryWeightedDistanceBFS<PGraph>::ComputeWDV(const int& NId, const TEdgeDir& dir, const TIntSet& SkipNIdS, TFltV& WDV) {
//   GetBFS(NId, dir, SkipNIdS);
//   NIdVWDH.GetDatV(WDV);
// }

// // Compute subset INFH using the direction specified
// template <class PGraph>
// void TFixedMemoryWeightedDistanceBFS<PGraph>::ComputeSubsetWDVH(const TIntV& NIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, TIntFltVH& WDVH) {
//   // Variables
//   TIntV::TIter VI;
//   TFltV WDV;
//   int NId;
//   // Clear outputs
//   WDVH.Clr();
//   // For each node in NIdV
//   for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
//     NId = VI->Val;
//     // Compute the INFH
//     // // printf("ComputeWDV\n----------\n");
//     ComputeWDV(NId, dir, WDV);
//     // Add INFH
//     WDVH.AddDat(NId, WDV);
//   }
// }

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






/// Fixed memory 
template <class PGraph>
class TFixedMemoryWeightedNeighborhood : public TFixedMemoryWeightedDistanceBFS<PGraph> {
public:
  // Backward / forward visitor (degree only)
  class TWeightedNeighborhoodVisitor {
  public:
    typedef THash<TInt, THash<TInt, TFlt> > TIntFltHH;
  public:
    TIntIntHH PathsHH;
    int k;
  public:
    TWeightedNeighborhoodVisitor(const PGraph& Graph, const int& k) : PathsHH(Graph->GetNodes()), k(k) { }
    void Start() { }
    void DiscoverNode(const int& NId, const int& depth, const double& WD) {
      // Only store path counts for paths of length <= k (weighted distance will keep going)
      if (depth > 0 && depth <= k) {
        if (PathsHH.IsKey(NId)) {
          TIntH& PathVH = PathsHH.GetDat(NId);
          if (PathVH.IsKey(depth)) {
            PathVH.GetDat(depth) ++;
          } else {
            PathVH.AddDat(depth, 1);
          }
        } else {
          TIntH PathVH;
          PathVH.AddDat(depth, 1);
          PathsHH.AddDat(NId, PathVH);
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
    }
  };
private:
  TWeightedNeighborhoodVisitor Visitor;
public:
  TFixedMemoryWeightedNeighborhood(const PGraph& Graph, const TIntV& DstNIdV, const int& k, const double& tol, const bool& preNormalized) : TFixedMemoryWeightedDistanceBFS<PGraph>(Graph, DstNIdV, k, tol, preNormalized), Visitor(TWeightedNeighborhoodVisitor(Graph, k)) { }

  // Compute neighborhood connectivity using the direction specified
  void ComputeConnectivity(const int& NId, const TEdgeDir& Dir, const TIntSet& SkipNIdS, TIntIntHH& PathsHH);
  // Compute subset neighborhood connectivity using the direction specified
  void ComputeSubsetConnectivity(const TIntV& NIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, THash<TIntPr, TIntH>& PathsHH);

  void Clr(const bool& DoDel = false);
};

// Compute neighborhood connectivity using the direction specified
template <class PGraph>
void TFixedMemoryWeightedNeighborhood<PGraph>::ComputeConnectivity(const int& NId, const TEdgeDir& Dir, const TIntSet& SkipNIdS, TIntIntHH& PathsHH) {
  Visitor.Clr();
  this->GetBfsVisitor(NId, Visitor, Dir, SkipNIdS);
  PathsHH = Visitor.PathsHH;
}


// Compute subset neighborhood connectivity using the direction specified
template <class PGraph>
void TFixedMemoryWeightedNeighborhood<PGraph>::ComputeSubsetConnectivity(const TIntV& NIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, THash<TIntPr, TIntH>& PathsHH) {
  // Variables
  TIntV::TIter VI;
  int NId;
  TIntIntHH NIdPathsVH;
  TIntIntHH::TIter HI;
  // For each node in NIdV
  for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
    NIdPathsVH.Clr(false);
    NId = VI->Val;
    ComputeConnectivity(NId, dir, SkipNIdS, NIdPathsVH);
    for (HI = NIdPathsVH.BegI(); HI < NIdPathsVH.EndI(); HI++) {
      PathsHH.AddDat(TIntPr(NId, HI.GetKey()), HI.GetDat());
    }
  }
}

} // namespace TSnap

#endif  