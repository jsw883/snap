//#//////////////////////////////////////////////
/// Graph traversal algorithms

#ifndef core_wd
#define core_wd

// Headers (?)

#include "enums.h"
#include "galgfixedmemory.h"

//#//////////////////////////////////////////////
/// Fixed memory BFS
/// For efficient iterative egonet traversal of graphs that are very large or have a high density.

namespace TSnap { // should this be TSnapDetail (?)

/// Fixed memory BFS, where queues and hash tables are initialized (but the memory reserved) between different calls of GetBfsVisitor().
template <class PGraph> class TFixedMemoryWD;

template <class PGraph>
class TFixedMemoryWD {
public:
  typedef TTriple<TInt, TInt, TFlt> TQueueTr;
private:
  PGraph Graph;
  TSnapQueue<TQueueTr > Queue;
  TIntH Color;
  // Visitor
  TIntFltH NIdVWDH;
  int k;
  double tol;
public:
  TFixedMemoryWD(const PGraph& Graph, const TIntV& DstNIdV, const int& k, const double& tol) : Graph(Graph), Queue(Graph->GetNodes()), Color(Graph->GetNodes()), k(k), tol(tol)  {
    // Initialize destination nodes with -1 weighted distances in case not discovered
    TIntV::TIter VI;
    for (VI = DstNIdV.BegI(); VI < DstNIdV.EndI(); VI++) {
      NIdVWDH.AddDat(VI->Val, 0.0); // Order is preserved
    }
  }
  void SetGraph(const PGraph& Graph);
  void GetBFS(const int& NId, const TEdgeDir& dir);
  
  // Get weighted distances using the direction specified
  void ComputeWDV(const int& NId, const TEdgeDir& Dir, TFltV& WDV);
  // Get weighted distances for the subset of nodes using the direction specified
  void ComputeSubsetWDVH(const TIntV& NIdV, const TEdgeDir& Dir, TIntFltVH& WDVH);
  
  void Clr(const bool& DoDel = false);
};

template<class PGraph>
void TFixedMemoryWD<PGraph>::SetGraph(const PGraph& Graph) {
  Graph = Graph;
  const int N = Graph->GetNodes();
  if (Queue.Reserved() < N) { Queue.Gen(N); }
  if (Color.GetReservedKeyIds() < N) { Color.Gen(N); }
}

template <class PGraph> // class template still needs to be declared
void TFixedMemoryWD<PGraph>::GetBFS(const int& NId, const TEdgeDir& dir) { // ONLY DEFINED FOR A SINGLE NID FOR NOW
  Clr(false);
  // Visitor.Start();
  int depth = 0, edge = 0, Deg = 0, U = 0, V = 0;
  double WD = 0, VWD;
  typename PGraph::TObj::TNodeI NI, UI, VI;
  // BFS
  // printf("NId: %d\n", NId);
  Color.AddDat(NId, 0);
  Queue.Push(TQueueTr(NId, depth, WD)); // NO IN-WEIGHT, SOURCE
  
  // Visitor.DiscoverNode(U, depth, W);
  // Weighted distances
  // if (NIdVWDH.IsKey(NId)) {
  //   NIdVWDH.GetDat(NId) = depth;
  // }
        
  while (!Queue.Empty()) {
    const TQueueTr& Top = Queue.Top();
    U = Top.Val1; depth = Top.Val2; WD = Top.Val3; UI = Graph->GetNI(U);
    // printf("U: %d\n", U);
    // printf("depth: %d\n", depth);
    // printf("WD: %f\n", WD);
    
    Deg = UI.GetDeg(dir);
    
    edge = 0;
    Queue.Pop();
    while (edge != Deg) {
      V = UI.GetNbrNId(edge, dir);
      // Visitor.ExamineEdge(U, depth, edge, V);
      
      if (Color.GetDat(V) < depth + 1) {
        // Color.AddDat(V, 1);
        Color.AddDat(V, depth + 1);
        
        // WEIGHTED DISTANCE
        if (WD == 0) {
          VWD = UI.GetNbrEW(edge, dir);
        } else {
          VWD = WD * UI.GetNbrEW(edge, dir) / UI.GetWDeg(dir);
        }
        // TODO: STORE CUMULATIVE WD IN NIdVWDH
        
        // Visitor.DiscoverNode(V, depth + 1, W);
        // Weighted distances
        if (NIdVWDH.IsKey(V)) {
          // NIdVWDH.GetDat(V) = depth + 1;
          NIdVWDH.GetDat(V) += VWD;
        } else if (depth + 1 < k && VWD > tol) { // CONTINUE
          Queue.Push(TQueueTr(V, depth + 1, VWD));
        }
        
        // Visitor.TreeEdge(U, depth, edge, V);
        
      }
      // else if (Color.GetDat(V) == 1) {
      //   Visitor.BackEdge(U, depth, edge, V);
      // }
      // else {
      //   Visitor.ForwardEdge(U, depth, edge, V);
      // }
      
      ++edge;
    }
    // Color.AddDat(U, 2);
    // Visitor.FinishNode(U, depth); // finish
  }
  // Visitor.Finish();
}

// Compute neighborhood depth counts using the direction specified
template <class PGraph>
void TFixedMemoryWD<PGraph>::ComputeWDV(const int& NId, const TEdgeDir& Dir, TFltV& WDV) {
  GetBFS(NId, Dir);
  NIdVWDH.GetDatV(WDV);
}

// Compute subset INFH using the direction specified
template <class PGraph>
void TFixedMemoryWD<PGraph>::ComputeSubsetWDVH(const TIntV& NIdV, const TEdgeDir& Dir, TIntFltVH& WDVH) {
  // Variables
  TIntV::TIter VI;
  TFltV WDV;
  int NId;
  // Clear outputs
  WDVH.Clr();
  // For each node in NIdV
  for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
    NId = VI->Val;
    // Compute the INFH
    // printf("ComputeWDV\n----------\n");
    ComputeWDV(NId, Dir, WDV);
    // Add INFH
    WDVH.AddDat(NId, WDV);
  }
}

template <class PGraph>
void TFixedMemoryWD<PGraph>::Clr(const bool& DoDel) {
  Queue.Clr(DoDel);
  Color.Clr(DoDel);
  // Visitor.Clr();
  TIntFltH::TIter HI;
  for (HI = NIdVWDH.BegI(); HI < NIdVWDH.EndI(); HI++) {
    HI.GetDat() = 0; // Reset path length within memory
  }
}

} // namespace TSnap

#endif