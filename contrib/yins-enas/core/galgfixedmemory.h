//#//////////////////////////////////////////////
/// Graph traversal algorithms

#ifndef core_galgfixedmemory
#define core_galgfixedmemory

// Headers (?)

#include "enums.h"

//#//////////////////////////////////////////////
/// Fixed memory BFS
/// For efficient iterative egonet traversal of graphs that are very large or have a high density.

namespace TSnap { // should this be TSnapDetail (?)

/// Fixed memory BFS, where queues and hash tables are initialized (but the memory reserved) between different calls of GetBfsVisitor().
template <class PGraph> class TFixedMemoryBFS;

template <class PGraph>
class TFixedMemoryBFS {
protected:
  PGraph Graph;
  TSnapQueue<TIntTr> Queue;
  TIntH Color;
public:
  TFixedMemoryBFS(const PGraph& Graph) : Graph(Graph), Queue(Graph->GetNodes()), Color(Graph->GetNodes()) { }
  void SetGraph(const PGraph& Graph);
  template <class TVisitor> void GetBfsVisitor(const PGraph& SubGraph, TVisitor& Visitor, const TEdgeDir& dir, const TIntSet& SkipNIdS, const int& k = -1);
  template <class TVisitor> void GetBfsVisitor(const PGraph& SubGraph, TVisitor& Visitor, const TEdgeDir& dir, const int& k = -1) {
    TIntSet SkipNIdS;
    GetBfsVisitor(SubGraph, Visitor, dir, SkipNIdS, k);
  }
  void Clr(const bool& DoDel = false);
};

template<class PGraph>
void TFixedMemoryBFS<PGraph>::SetGraph(const PGraph& Graph) {
  Graph = Graph;
  const int N = Graph->GetNodes();
  if (Queue.Reserved() < N) { Queue.Gen(N); }
  if (Color.GetReservedKeyIds() < N) { Color.Gen(N); }
}

template <class PGraph> // class template still needs to be declared
template <class TVisitor> // member template
void TFixedMemoryBFS<PGraph>::GetBfsVisitor(const PGraph& SubGraph, TVisitor& Visitor, const TEdgeDir& dir, const TIntSet& SkipNIdS, const int& k) {
  Clr(false);
  Visitor.Start();
  int depth = 0, peekdepth = 0, edge = 0, Deg = 0, U = 0, V = 0, VDeg = 0;
  typename PGraph::TObj::TNodeI NI, UI;
  for (NI = SubGraph->BegNI(); NI < SubGraph->EndNI(); NI++) {
    U = NI.GetId();
    if (! Color.IsKey(U)) { // is unvisited node
      Color.AddDat(U, 1);
      switch(dir) {
        case edInDirected: Deg = Graph->GetNI(U).GetInDeg(); break;
        case edOutDirected: Deg = Graph->GetNI(U).GetOutDeg(); break;
        case edUnDirected: Deg = Graph->GetNI(U).GetDeg(); break;
      }
      Queue.Push(TIntTr(U, depth, Deg));
      Visitor.DiscoverNode(U, depth); // discover node
      while (!Queue.Empty()) { // while stack is not empty
        const TIntTr& Top = Queue.Top();
        U = Top.Val1; depth = Top.Val2; Deg = Top.Val3;
        UI = Graph->GetNI(U);
        edge = 0;
        Queue.Pop();
        while (edge != Deg) { // for all edges
          switch(dir) {
            case edInDirected: V = UI.GetInNId(edge); break;
            case edOutDirected: V = UI.GetOutNId(edge); break;
            case edUnDirected: V = UI.GetNbrNId(edge); break;
          }
          if (SubGraph->IsNode(V)) { // ensures SubGraph node depth = 0
            peekdepth = 0;
          } else {
            peekdepth = depth + 1;
          }
          Visitor.ExamineEdge(U, depth, edge, V); // examine edge
          if (!SkipNIdS.IsKey(V) && !Color.IsKey(V)) { // V is not excluded and V has not been discovered
            Color.AddDat(V, 1);
            Visitor.DiscoverNode(V, peekdepth); // discover node
            Visitor.TreeEdge(U, depth, edge, V); // tree edge
            if (k == -1 || peekdepth < k) {
              switch(dir) {
                case edInDirected: VDeg = Graph->GetNI(V).GetInDeg(); break;
                case edOutDirected: VDeg = Graph->GetNI(V).GetOutDeg(); break;
                case edUnDirected: VDeg = Graph->GetNI(V).GetDeg(); break;
              }
              Queue.Push(TIntTr(V, peekdepth, VDeg));
            }
          }
          else if (Color.GetDat(V) == 1) { // V has been discovered
            Visitor.BackEdge(U, depth, edge, V);
          }
          else {
            Visitor.ForwardEdge(U, depth, edge, V);
          }
          ++edge;
        }
        Color.AddDat(U, 2); 
        Visitor.FinishNode(U, depth); // finish
      }
    }
  }
  Visitor.Finish();
}

template <class PGraph>
void TFixedMemoryBFS<PGraph>::Clr(const bool& DoDel) {
  Queue.Clr(DoDel);
  Color.Clr(DoDel);
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
  this->GetBfsVisitor(Ego, Visitor, Dir);
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
  Visitor.Clr();
}

// Interpolate NF
double InterpolateNF(const TUInt64V& NF, const double& p);
void InterpolateINFH(const THash<TInt, TUInt64V>& INFH, TIntFltH& QuantileH, const double& p);

// Get extrema (diameter and nodes discovered)
void GetNodesINFH(const THash<TInt, TUInt64V>& INFH, TIntIntH& NodesH);
void GetDiameterINFH(const THash<TInt, TUInt64V>& INFH, TIntIntH& DiameterH);

} // namespace TSnap

#endif