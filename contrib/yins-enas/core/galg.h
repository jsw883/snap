//#//////////////////////////////////////////////
/// Graph traversal algorithms

#ifndef core_galg
#define core_galg

// Headers (?)

#include "enums.h"

//#//////////////////////////////////////////////
/// DFS and BFS algorithms (white / grey / black)

namespace TSnap {

/// Returns the backward connected component (BCC) of a graph from a subgraph using a directed DFS starting from subgraph nodes.
template <class PGraph> void GetBcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom);
/// Returns the forward connected component (BCC) of a graph from a subgraph using a directed DFS starting from subgraph nodes.
template <class PGraph> void GetFcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom);
/// Returns the undirected connected component (BCC) of a graph from a subgraph using a directed DFS starting from subgraph nodes.
template <class PGraph> void GetUcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom);

/// Returns the backward connected component (BCC) of a graph from a subgraph using a directed BFS starting from subgraph nodes and depth limited to k edges.
template <class PGraph> void GetBcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom, const int& k);
/// Returns the forward connected component (BCC) of a graph from a subgraph using a directed BFS starting from subgraph nodes and depth limited to k edges.
template <class PGraph> void GetFcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom, const int& k);
/// Returns the undirected connected component (BCC) of a graph from a subgraph using a directed BFS starting from subgraph nodes and depth limited to k edges.
template <class PGraph> void GetUcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom, const int& k);

} // namespace TSnap

/// Depth first search from a subgraph of a graph with direction specified and depth limited
template <class PGraph, class TVisitor> void GetDfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor, const TEdgeDir& dir);
/// Breadth first search from a subgraph of a graph with direction specified and depth limited
template <class PGraph, class TVisitor> void GetBfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor, const TEdgeDir& dir, const int& k = -1);

// Depth first search from a subgraph of a graph with direction specified and depth limited
template <class PGraph, class TVisitor>
void GetDfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor, const TEdgeDir& dir) {
  const int Nodes = Graph->GetNodes();
  TSStack<TIntTr> Stack(Nodes);
  int edge = 0, Deg = 0, U = 0, V = 0;
  TIntH Color(Nodes);
  typename PGraph::TObj::TNodeI NI, UI;
  for (NI = SubGraph->BegNI(); NI < SubGraph->EndNI(); NI++) {
    U = NI.GetId();
    if (! Color.IsKey(U)) {         // is unvisited node
      Color.AddDat(U, 1); 
      Visitor.DiscoverNode(U);       // discover
      switch(dir) {
        case edInDirected: Deg = Graph->GetNI(U).GetInDeg(); break;
        case edOutDirected: Deg = Graph->GetNI(U).GetOutDeg(); break;
        case edUnDirected: Deg = Graph->GetNI(U).GetDeg(); break;
      }
      Stack.Push(TIntTr(U, 0, Deg));
      while (! Stack.Empty()) {
        const TIntTr& Top = Stack.Top();
        U=Top.Val1; edge=Top.Val2; Deg=Top.Val3;
        typename PGraph::TObj::TNodeI UI = Graph->GetNI(U);
        Stack.Pop();
        while (edge != Deg) {
          switch(dir) {
            case edInDirected: V = UI.GetInNId(edge); break;
            case edOutDirected: V = UI.GetOutNId(edge); break;
            case edUnDirected: V = UI.GetNbrNId(edge); break;
          }
          Visitor.ExamineEdge(U, V); // examine edge
          if (! Color.IsKey(V)) {
            Visitor.TreeEdge(U, V);  // tree edge
            Stack.Push(TIntTr(U, ++edge, Deg));
            U = V;
            Color.AddDat(U, 1); 
            Visitor.DiscoverNode(U); // discover
            UI = Graph->GetNI(U);
            edge = 0;
            switch(dir) {
              case edInDirected: Deg = UI.GetInDeg(); break;
              case edOutDirected: Deg = UI.GetOutDeg(); break;
              case edUnDirected: Deg = UI.GetDeg(); break;
            }
          }
          else if (Color.GetDat(V) == 1) {
            Visitor.BackEdge(U, V);  // edge upward
            ++edge; }
          else {
            Visitor.FwdEdge(U, V);   // edge downward
            ++edge; }
        }
        Color.AddDat(U, 2); 
        Visitor.FinishNode(U);       // finish
      }
    }
  }
}

// Breadth first search from a subgraph of a graph with direction specified and depth limited
template <class PGraph, class TVisitor>
void GetBfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor, const TEdgeDir& dir, const int& k) {
  const int Nodes = Graph->GetNodes();
  TQQueue<TIntTr> Queue(Nodes); // it might be inefficient reinitializing the queue every BFS
  int depth = 0, peekdepth = 0, edge = 0, Deg = 0, U = 0, V = 0, VDeg = 0;
  TIntH Color(Nodes);
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
      while (! Queue.Empty()) { // while stack is not empty
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
          if (! Color.IsKey(V)) { // V has not been discovered
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
}

// Backward / forward visitor (edge methods overloaded to work with either BFS)
class TDfsCnComVisitor {
public:
  TCnCom CnCom;
public:
  TDfsCnComVisitor() { }
  void DiscoverNode(int NId) {
    CnCom.Add(NId);
  }
  void FinishNode(const int& NId) { }
  void ExamineEdge(const int& NId1, const int& NId2) { }
  void TreeEdge(const int& NId1, const int& NId2) { }
  void BackEdge(const int& NId1, const int& NId2) { }
  void FwdEdge(const int& NId1, const int& NId2) { }
};

class TBfsCnComVisitor {
public:
  TCnCom CnCom;
public:
  TBfsCnComVisitor() { }
  void DiscoverNode(int NId, int depth) {
    CnCom.Add(NId);
  }
  void FinishNode(const int& NId, int depth) { }
  void ExamineEdge(const int& NId1, const int& NId2) { }
  void TreeEdge(const int& NId1, const int& NId2) { }
  void BackEdge(const int& NId1, const int& NId2) { }
  void FwdEdge(const int& NId1, const int& NId2) { }
};

namespace TSnap {

// Returns the backward, forward, and undirected connected components (BCC, FCC,
// UCC) of a graph from a subgraph by traversing the graph using a directed DFS
// starting from subgraph nodes.
template <class PGraph>
void GetBcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom) {
  TDfsCnComVisitor Visitor;
  GetDfsVisitor(Graph, SubGraph, Visitor, edInDirected);
  CnCom = Visitor.CnCom;
}
template <class PGraph>
void GetFcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom) {
  TDfsCnComVisitor Visitor;
  GetDfsVisitor(Graph, SubGraph, Visitor, edOutDirected);
  CnCom = Visitor.CnCom;
}
template <class PGraph>
void GetUcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom) {
  TDfsCnComVisitor Visitor;
  GetDfsVisitor(Graph, SubGraph, Visitor, edUnDirected);
  CnCom = Visitor.CnCom;
}

// Returns the backward, forward, and undirected connected components (BCC, FCC,
// UCC) of a graph from a subgraph by traversing the graph using a directed BFS
// starting from subgraph nodes and depth limited to k edges.
template <class PGraph>
void GetBcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom, const int& k) {
  TBfsCnComVisitor Visitor;
  GetBfsVisitor(Graph, SubGraph, Visitor, edInDirected, k);
  CnCom = Visitor.CnCom;
}
template <class PGraph>
void GetFcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom, const int& k) {
  TBfsCnComVisitor Visitor;
  GetBfsVisitor(Graph, SubGraph, Visitor, edOutDirected, k);
  CnCom = Visitor.CnCom;
}
template <class PGraph>
void GetUcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom, const int& k) {
  TBfsCnComVisitor Visitor;
  GetBfsVisitor(Graph, SubGraph, Visitor, edUnDirected, k);
  CnCom = Visitor.CnCom;
}

} // namespace TSnap

//#//////////////////////////////////////////////
/// Fixed memory DFS and BFS base class
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
  template <class TVisitor> void GetBfsVisitor(const PGraph& SubGraph, TVisitor& Visitor, const TEdgeDir& dir, const int& k = -1);
  void Clr(const bool& DoDel = false);
};

template<class PGraph>
void TFixedMemoryBFS<PGraph>::SetGraph(const PGraph& Graph) {
  Graph = Graph;
  const int N = Graph->GetNodes();
  if (Queue.Reserved() < N) { Queue.Gen(N); }
  if (Color.GetReservedKeyIds() < N) { Color.Gen(N); }
}

template <class PGraph> // still need to specify (?)
template <class TVisitor>
void TFixedMemoryBFS<PGraph>::GetBfsVisitor(const PGraph& SubGraph, TVisitor& Visitor, const TEdgeDir& dir, const int& k) {
  Queue.Clr(false);
  Color.Clr(false);
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
      while (! Queue.Empty()) { // while stack is not empty
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
          if (! Color.IsKey(V)) { // V has not been discovered
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

template <class PGraph>
class TFixedMemorySubsetDiameter : public TFixedMemoryBFS<PGraph> {
public:
  // Backward / forward visitor (degree only)
  class TSubsetDiameterVisitor {
  public:
    int nodes;
    int diameter;
  public:
    TSubsetDiameterVisitor() : nodes(0), diameter(0) { }
    void Start() { }
    void DiscoverNode(const int& NId, const int& depth) {
      nodes++;
      if (depth > diameter) {
        diameter = depth;
      }
    }
    void FinishNode(const int& NId, const int& depth) { }
    void ExamineEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void TreeEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void BackEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void ForwardEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void Finish() { }
    void Clr() {
      nodes = 0;
      diameter = 0;
    }
  };
private:
  TSubsetDiameterVisitor Visitor;
public:
  TFixedMemorySubsetDiameter(const PGraph& Graph) : TFixedMemoryBFS<PGraph>(Graph), Visitor(TSubsetDiameterVisitor()) { }

  // Get diameter and node count for a single node (int / out / undirected)
  void ComputeInDiameter(const int& NId, int& nodes, int& diameter) {
    ComputeDiameter(NId, edInDirected);
  }
  void ComputeOutDiameter(const int& NId, int& nodes, int& diameter) {
    ComputeDiameter(NId, edOutDirected);
  }
  void ComputeDiameter(const int& NId, int& nodes, int& diameter) {
    ComputeDiameter(NId, edUnDirected);
  }
  // Get diameter and node count for a single node using the direction specified 
  void ComputeDiameter(const int& NId, const TEdgeDir& Dir, int& nodes, int& diameter);

  // Get diameters and node counts for the subset of nodes (int / out / undirected)
  void ComputeInSubsetDiameter(const TIntV& NIdV, TIntIntH& NodesH, TIntIntH& DiameterH) {
    ComputeSubsetDiameter(NIdV, edInDirected, NodesH, DiameterH);
  }
  void ComputeOutSubsetDiameter(const TIntV& NIdV, TIntIntH& NodesH, TIntIntH& DiameterH) {
    ComputeSubsetDiameter(NIdV, edOutDirected, NodesH, DiameterH);
  }
  void ComputeSubsetDiameter(const TIntV& NIdV, TIntIntH& NodesH, TIntIntH& DiameterH) {
    ComputeSubsetDiameter(NIdV, edUnDirected, NodesH, DiameterH);
  }
  // Get diameters and node counts for the subset of nodes using the direction specified
  void ComputeSubsetDiameter(const TIntV& NIdV, const TEdgeDir& Dir, TIntIntH& NodesH, TIntIntH& DiameterH);
  
  void Clr(const bool& DoDel = false);
};

// Compute egonet statistics using the direction specified
template <class PGraph>
void TFixedMemorySubsetDiameter<PGraph>::ComputeDiameter(const int& NId, const TEdgeDir& Dir, int& nodes, int& diameter) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId);
  Visitor.Clr();
  this->GetBfsVisitor<TSubsetDiameterVisitor>(Ego, Visitor, Dir);
  nodes = Visitor.nodes;
  diameter = Visitor.diameter;
}

// Compute subset using the direction specified
template <class PGraph>
void TFixedMemorySubsetDiameter<PGraph>::ComputeSubsetDiameter(const TIntV& NIdV, const TEdgeDir& Dir, TIntIntH& NodesH, TIntIntH& DiameterH) {
  TIntV::TIter VI;
  int nodes, diameter;
  for (VI = NIdV.BegI(); VI < NIdV.EndI();  VI++) {
    ComputeDiameter(VI->Val, Dir, nodes, diameter);
    NodesH.AddDat(VI->Val, nodes);
    DiameterH.AddDat(VI->Val, diameter);
  }
}

template <class PGraph>
void TFixedMemorySubsetDiameter<PGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<PGraph>::Clr(DoDel);
  Visitor.Clr(); // resets the degree visitor to have zero nodes and diameter
}

} // namespace TSnap

#endif