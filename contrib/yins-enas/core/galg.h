#ifndef core_galg
#define core_galg

// Headers (?)

// #include "Snap.h"
// #include "wgraph.h"

namespace TSnap {

/// Returns the backward connected component (BCC) of a graph from a subgraph using a directed DFS starting from subgraph nodes.
template <class PGraph> void GetBcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom);
/// Returns the forward connected component (BCC) of a graph from a subgraph using a directed DFS starting from subgraph nodes.
template <class PGraph> void GetFcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom);
/// Returns the undirected connected component (BCC) of a graph from a subgraph using a directed DFS starting from subgraph nodes.
template <class PGraph> void GetUcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom);

} // namespace TSnap

/// Depth first search from a subgraph of a graph with direction specified
template <class PGraph, class TVisitor> void GetDfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor, const int dir = 1);
/// Breadth first search from a subgraph of a graph with direction specified (depth limited)
template <class PGraph, class TVisitor> void GetBfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor, const int dir = 1, const int k = 1);

// Depth first search from a subgraph of a graph with direction specified
template <class PGraph, class TVisitor>
void GetDfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor, const int dir) {
  const int Nodes = Graph->GetNodes();
  TSStack<TIntTr> Stack(Nodes);
  int edge = 0, Deg = 0, U = 0, V = 0;
  TIntH ColorH(Nodes);
  typename PGraph::TObj::TNodeI NI, UI;
  for (NI = SubGraph->BegNI(); NI < SubGraph->EndNI(); NI++) {
    U = NI.GetId();
    if (! ColorH.IsKey(U)) {         // is unvisited node
      ColorH.AddDat(U, 1); 
      Visitor.DiscoverNode(U);       // discover
      switch(dir) {
        case 0: Deg = Graph->GetNI(U).GetInDeg();  break;
        case 1: Deg = Graph->GetNI(U).GetOutDeg(); break;
        case 2: Deg = Graph->GetNI(U).GetDeg();    break;
      }
      Stack.Push(TIntTr(U, 0, Deg));
      while (! Stack.Empty()) {
        const TIntTr& Top = Stack.Top();
        U=Top.Val1; edge=Top.Val2; Deg=Top.Val3;
        typename PGraph::TObj::TNodeI UI = Graph->GetNI(U);
        Stack.Pop();
        while (edge != Deg) {
          switch(dir) {
            case 0: V = UI.GetInNId(edge);  break;
            case 1: V = UI.GetOutNId(edge); break;
            case 2: V = UI.GetNbrNId(edge); break;
          }
          Visitor.ExamineEdge(U, V); // examine edge
          if (! ColorH.IsKey(V)) {
            Visitor.TreeEdge(U, V);  // tree edge
            Stack.Push(TIntTr(U, ++edge, Deg));
            U = V;
            ColorH.AddDat(U, 1); 
            Visitor.DiscoverNode(U); // discover
            UI = Graph->GetNI(U);
            edge = 0;
            switch(dir) {
              case 0: Deg = UI.GetInDeg();  break;
              case 1: Deg = UI.GetOutDeg(); break;
              case 2: Deg = UI.GetDeg();    break;
            }
          }
          else if (ColorH.GetDat(V) == 1) {
            Visitor.BackEdge(U, V);  // edge upward
            ++edge; }
          else {
            Visitor.FwdEdge(U, V);   // edge downward
            ++edge; }
        }
        ColorH.AddDat(U, 2); 
        Visitor.FinishNode(U);       // finish
      }
    }
  }
}
template <class PGraph, class TVisitor>
void GetBDfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor) {
  GetDfsVisitor(Graph, SubGraph, Visitor, 0);
}
template <class PGraph, class TVisitor>
void GetFDfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor) {
  GetDfsVisitor(Graph, SubGraph, Visitor, 1);
}
template <class PGraph, class TVisitor>
void GetUDfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor) {
  GetDfsVisitor(Graph, SubGraph, Visitor, 2);
}

// Breadth first search from a subgraph of a graph with direction specified (depth limited)
template <class PGraph, class TVisitor>
void GetBfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor, const int dir, const int k) {
  const int Nodes = Graph->GetNodes();
  TQQueue<TIntTr> Queue(Nodes); // it might be inefficient reinitializing the queue every BFS
  int depth = 0, edge = 0, Deg = 0, U = 0, V = 0, VDeg = 0;
  TIntH ColorH(Nodes);
  typename PGraph::TObj::TNodeI NI, UI;
  for (NI = SubGraph->BegNI(); NI < SubGraph->EndNI(); NI++) {
    U = NI.GetId();
    if (! ColorH.IsKey(U)) { // is unvisited node
      ColorH.AddDat(U, 1);
      switch(dir) {
        case 0: Deg = Graph->GetNI(U).GetInDeg();  break;
        case 1: Deg = Graph->GetNI(U).GetOutDeg(); break;
        case 2: Deg = Graph->GetNI(U).GetDeg();    break;
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
            case 0: V = UI.GetInNId(edge);  break;
            case 1: V = UI.GetOutNId(edge); break;
            case 2: V = UI.GetNbrNId(edge); break;
          }
          Visitor.ExamineEdge(U, V); // examine edge
          if (! ColorH.IsKey(V)) { // V has not been discovered
            ColorH.AddDat(V, 1);
            Visitor.DiscoverNode(V, depth + 1); // discover node
            Visitor.TreeEdge(U, V); // tree edge
            if (depth + 1 < k) {
              switch(dir) {
                case 0: VDeg = Graph->GetNI(V).GetInDeg();  break;
                case 1: VDeg = Graph->GetNI(V).GetOutDeg(); break;
                case 2: VDeg = Graph->GetNI(V).GetDeg();    break;
              }
              Queue.Push(TIntTr(V, depth + 1, VDeg));
            }
          }
          else if (ColorH.GetDat(V) == 1) { // V has been discovered
            Visitor.BackEdge(U,V);
          }
          else {
            Visitor.FwdEdge(U,V);
          }
          ++edge;
        }
        ColorH.AddDat(U, 2); 
        Visitor.FinishNode(U, depth); // finish
      }
    }
  }
}
template <class PGraph, class TVisitor>
void GetBBfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor, const int k) {
  GetBfsVisitor(Graph, SubGraph, Visitor, 0, k);
}
template <class PGraph, class TVisitor>
void GetFBfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor, const int k) {
  GetBfsVisitor(Graph, SubGraph, Visitor, 1, k);
}
template <class PGraph, class TVisitor>
void GetUBfsVisitor(const PGraph& Graph, const PGraph& SubGraph, TVisitor& Visitor, const int k) {
  GetBfsVisitor(Graph, SubGraph, Visitor, 2, k);
}

// Backward / forward visitor (edge methods overloaded to work with either BFS)
class TCnComVisitor {
public:
  TCnCom CnCom;
public:
  TCnComVisitor() { }
  void DiscoverNode(int NId) {
    CnCom.Add(NId);
  }
  void DiscoverNode(int NId, int depth) {
    CnCom.Add(NId);
  }
  void FinishNode(const int& NId) { }
  void FinishNode(const int& NId, int depth) { }
  void ExamineEdge(const int& NId1, const int& NId2) { }
  void ExamineEdge(const int& EId) { }
  void TreeEdge(const int& NId1, const int& NId2) { }
  void TreeEdge(const int& EId) { }
  void BackEdge(const int& NId1, const int& NId2) { }
  void BackEdge(const int& EId) { }
  void FwdEdge(const int& NId1, const int& NId2) { }
  void FwdEdge(const int& EId) { }
};

namespace TSnap {

// Returns the backward, forward, and undirected connected components (BCC, FCC,
// UCC) of a graph from a subgraph by traversing the graph using a directed DFS
// starting from subgraph nodes.
template <class PGraph>
void GetBcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom) {
  TCnComVisitor Visitor;
  GetBDfsVisitor(Graph, SubGraph, Visitor);
  CnCom = Visitor.CnCom;
}
template <class PGraph>
void GetFcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom) {
  TCnComVisitor Visitor;
  GetFDfsVisitor(Graph, SubGraph, Visitor);
  CnCom = Visitor.CnCom;
}
template <class PGraph>
void GetUcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom) {
  TCnComVisitor Visitor;
  GetUDfsVisitor(Graph, SubGraph, Visitor);
  CnCom = Visitor.CnCom;
}

// Returns the backward, forward, and undirected connected components (BCC, FCC,
// UCC) of a graph from a subgraph by traversing the graph using a directed BFS
// starting from subgraph nodes and depth limited to k edges.
template <class PGraph>
void GetBcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom, const int k) {
  TCnComVisitor Visitor;
  GetBBfsVisitor(Graph, SubGraph, Visitor, k);
  CnCom = Visitor.CnCom;
}
template <class PGraph>
void GetFcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom, const int k) {
  TCnComVisitor Visitor;
  GetFBfsVisitor(Graph, SubGraph, Visitor, k);
  CnCom = Visitor.CnCom;
}
template <class PGraph>
void GetUcc(const PGraph& Graph, const PGraph& SubGraph, TCnCom& CnCom, const int k) {
  TCnComVisitor Visitor;
  GetUBfsVisitor(Graph, SubGraph, Visitor, k);
  CnCom = Visitor.CnCom;
}

} // namespace TSnap

// Backward / forward visitor (edge methods overloaded to work with either BFS)
// class TEgonetVisitor {
// public:
//   TCnCom CnCom;
//   TIntIntVH TDepthVH;
// public:
//   TEgonetVisitor() { }
//   void DiscoverNode(int NId, int depth) {
//     CnCom.Add(NId);
//     if (depth > 0) { TDepthVH.AddDat(depth).Add(NId); }
//   }
//   void FinishNode(const int& NId, int depth) { }
//   void ExamineEdge(const int& NId1, const int& NId2) { }
//   void ExamineEdge(const int& EId) { }
//   void TreeEdge(const int& NId1, const int& NId2) { }
//   void TreeEdge(const int& EId) { }
//   void BackEdge(const int& NId1, const int& NId2) { }
//   void BackEdge(const int& EId) { }
//   void FwdEdge(const int& NId1, const int& NId2) { }
//   void FwdEdge(const int& EId) { }
// };

namespace TSnap {

// // Returns k deep egonets for node CtrNId (in / out / undirected)
// template<class PGraph>
// PGraph GetInEgonet(const PGraph& Graph, const int CtrNId, const int k, TIntIntVH& TDepthVH) {
//   PGraph Ego = PGraph::TObj::New(); Ego->AddNode(CtrNId);
//   TEgonetVisitor Visitor;
//   GetFBfsVisitor(Graph, Ego, Visitor, k);
//   TDepthVH = Visitor.TDepthVH;
//   return(TSnap::GetSubGraph(Graph, Visitor.CnCom.NIdV));
// }
// template<class PGraph>
// PGraph GetOutEgonet(const PGraph& Graph, const int CtrNId, const int k, TIntIntVH& TDepthVH) {
//   PGraph Ego = PGraph::TObj::New(); Ego->AddNode(CtrNId);
//   TEgonetVisitor Visitor;
//   GetBBfsVisitor(Graph, Ego, Visitor, k);
//   TDepthVH = Visitor.TDepthVH;
//   return(TSnap::GetSubGraph(Graph, Visitor.CnCom.NIdV));
// }
// template<class PGraph>
// PGraph GetEgonet(const PGraph& Graph, const int CtrNId, const int k, TIntIntVH& TDepthVH) {
//   PGraph Ego = PGraph::TObj::New(); Ego->AddNode(CtrNId);
//   TEgonetVisitor Visitor;
//   GetUBfsVisitor(Graph, Ego, Visitor, k);
//   TDepthVH = Visitor.TDepthVH;
//   return(TSnap::GetSubGraph(Graph, Visitor.CnCom.NIdV));
// }

// // Returns k deep egonets for node CtrNId (in / out / undirected)
// PNGraph GetInEgonet(const PNGraph& Graph, const int CtrNId, const int k);
// PNGraph GetOutEgonet(const PNGraph& Graph, const int CtrNId, const int k);
// PNGraph GetEgonet(const PNGraph& Graph, const int CtrNId, const int k);

} // namespace TSnap

#endif