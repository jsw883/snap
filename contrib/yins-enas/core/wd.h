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
  TFixedMemoryWD(const PGraph& Graph, const TIntV& DstNIdV, const int& k, const double& tol, const bool& preNormalized) : Graph(Graph), Queue(Graph->GetNodes()), k(k), tol(tol), preNormalized(preNormalized) {
    // Initialize destination nodes with -1 weighted distances in case not discovered
    TIntV::TIter VI;
    for (VI = DstNIdV.BegI(); VI < DstNIdV.EndI(); VI++) {
      NIdVWDH.AddDat(VI->Val, 0.0); // Order is preserved
    }
  }
  void SetGraph(const PGraph& Graph);
  void GetBFS(const int& NId, const TEdgeDir& dir, const TIntSet& SkipNIdS);
  void GetBFS(const int& NId, const TEdgeDir& dir) { TIntSet SkipNIdS; GetBFS(NId, dir, SkipNIdS); }
  
  // Get weighted distances using the direction specified
  void ComputeWDV(const int& NId, const TEdgeDir& dir, const TIntSet& SkipNIdS, TFltV& WDV);
  void ComputeWDV(const int& NId, const TEdgeDir& dir, TFltV& WDV) { TIntSet SkipNIdS; ComputeWDV(NId, dir, SkipNIdS, WDV); }
  // Get weighted distances for the subset of nodes using the direction specified
  void ComputeSubsetWDVH(const TIntV& NIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, TIntFltVH& WDVH);
  void ComputeSubsetWDVH(const TIntV& NIdV, const TEdgeDir& dir, TIntFltVH& WDVH) { TIntSet SkipNIdS; ComputeSubsetWDVH(NIdV, dir, SkipNIdS, WDVH); }
  
  void Clr(const bool& DoDel = false);
};

template<class PGraph>
void TFixedMemoryWD<PGraph>::SetGraph(const PGraph& Graph) {
  Graph = Graph;
  const int N = Graph->GetNodes();
  if (Queue.Reserved() < N) { Queue.Gen(N); }
}

template <class PGraph> // class template still needs to be declared
void TFixedMemoryWD<PGraph>::GetBFS(const int& NId, const TEdgeDir& dir, const TIntSet& SkipNIdS) { // ONLY DEFINED FOR A SINGLE NID FOR NOW
  
  printf("BFS START\n");

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
  temp++;
  Queue.Push(TQueueQuad(NId, depth, path, WD)); // NO IN-WEIGHT, SOURCE

  printf("BFS WHILE\n\n");

  // int i = 0;

  while (!Queue.Empty()) {
    
    // printf("%d: ", i);

    const TQueueQuad& Top = Queue.Top();

    printf("(");

    U = Top.Val1; depth = Top.Val2; path = Top.Val3; WD = Top.Val4;

    printf("U: %d, depth: %d, path: %d, WD: %f", U, depth, path, WD);
    printf("IsNode: %d", Graph->IsNode(U));

    UI = Graph->GetNI(U);
    Deg = UI.GetDeg(dir);

    printf("Deg: %d", Deg);

    edge = 0;
    Queue.Pop(); // deletes memory

    printf(")\n");

    while (edge != Deg) {
      V = UI.GetNbrNId(edge, dir);

      printf(" -> %d: (V: %d", edge, V);

      if (!SkipNIdS.IsKey(V) && !PSetH.GetDat(path).IsKey(V)) {
        
        // WEIGHTED DISTANCE
        if (WD == 0) {

          printf("WD: ");
          
          VWD = UI.GetNbrEW(edge, dir);

          printf("%f, ", VWD);
          
          if (!preNormalized) { VWD /= UI.GetWDeg(dir); }
        } else {
          
          printf("WD: ");
          
          VWD = WD * UI.GetNbrEW(edge, dir);
          if (!preNormalized) { VWD /= UI.GetWDeg(dir); }

          printf("%f, ", VWD);
          
        }
        // STORE

        printf("store: ");
        
        if (NIdVWDH.IsKey(V) && VWD > tol) {
          NIdVWDH.GetDat(V) += VWD;
          
          printf("stored, ");
          
        } else {

          printf("ignored, ");
          
        }
        
        // CONTINUE

        printf("continue: ");
          
        if (depth + 1 < k && VWD > tol) {
          temp++;

          printf("yes, path: ");

          const TIntSet PSCopy = PSetH.GetDat(path); // Must copy
          TIntSet& PS = PSetH.AddDat(temp, PSCopy);
          PS.AddKey(V);

          printf("updated, queue: ");

          Queue.Push(TQueueQuad(V, depth + 1, temp, VWD));

          printf("updated,  ");

        } else {

          printf("no");

        }
      
        printf(")\n");

      }

      printf("\n");

      ++edge;
    }
    PSetH.DelKey(path);
  }

  printf("BFS END\n");

}


// Compute neighborhood depth counts using the direction specified
template <class PGraph>
void TFixedMemoryWD<PGraph>::ComputeWDV(const int& NId, const TEdgeDir& dir, const TIntSet& SkipNIdS, TFltV& WDV) {
  GetBFS(NId, dir, SkipNIdS);
  NIdVWDH.GetDatV(WDV);
}

// Compute subset INFH using the direction specified
template <class PGraph>
void TFixedMemoryWD<PGraph>::ComputeSubsetWDVH(const TIntV& NIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, TIntFltVH& WDVH) {
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
    printf("ComputeWDV\n----------\n");
    ComputeWDV(NId, dir, WDV);
    // Add INFH
    WDVH.AddDat(NId, WDV);
  }
}

template <class PGraph>
void TFixedMemoryWD<PGraph>::Clr(const bool& DoDel) {
  Queue.Clr(DoDel);
  // Visitor.Clr();
  TIntFltH::TIter HI;
  for (HI = NIdVWDH.BegI(); HI < NIdVWDH.EndI(); HI++) {
    HI.GetDat() = 0; // Reset path length within memory
  }
  PSetH.Clr(DoDel); // May need to force true to reduce memory
}

} // namespace TSnap

#endif  