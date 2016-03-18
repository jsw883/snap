//#//////////////////////////////////////////////
/// Weighted graphs

#ifndef core_wgraph
#define core_wgraph

// Headers (?)

#include "enums.h"

//#//////////////////////////////////////////////
/// Weighted directed graphs

// NOTE: TWNGraph is based on TNGraph and maintians copies of the edges at the
// source and destination nodes to minimize cache misses when traversing the
// network. Consistency is enforced when edge weights are updated, incremented,
// or when edges are deleted.

// NOTE: In order to define smart pointers, TEdgeW needs to be defined (C++98)
template <class TEdgeW> class TWNGraph;

/// Integer weighted directed graph (TWNGraph<TInt>)
typedef TWNGraph<TInt> TIntWNGraph;
/// Float weighted directed graph (TWNGraph<TFlt>)
typedef TWNGraph<TFlt> TFltWNGraph;

/// Pointer to an integer weighted directed graph (TFltWNGraph)
typedef TPt<TIntWNGraph> PIntWNGraph;
/// Pointer to a float weighted directed graph (TIntWNGraph)
typedef TPt<TFltWNGraph> PFltWNGraph;

//#//////////////////////////////////////////////
/// Weighted directed multigraphs

// NOTE: TWNEGraph is based on TNEGraph but also maintians copies of the edges
// at source and destination nodes to minimize cache misses when traversing the
// network. Consistency is enforced when edge weights are updated, incremented,
// or when edges are deleted.

// NOTE: In order to define smart pointers, TEdgeW needs to be defined (C++98)
template <class TEdgeW> class TWNEGraph;

/// Integer weighted directed graph (TWNEGraph<TInt>)
typedef TWNEGraph<TInt> TIntWNEGraph;
/// Float weighted directed graph (TWNEGraph<TFlt>)
typedef TWNEGraph<TFlt> TFltWNEGraph;

/// Pointer to an integer weighted directed graph (TFltWNEGraph)
typedef TPt<TIntWNEGraph> PIntWNEGraph;
/// Pointer to a float weighted directed graph (TIntWNEGraph)
typedef TPt<TFltWNEGraph> PFltWNEGraph;

//#//////////////////////////////////////////////
/// Weighted directed graph.
template <class TEdgeW> // TEdgeW can be TInt or TFlt (which must be numeric typed, should this be restricted (?))
class TWNGraph {
public:
  typedef TWNGraph<TEdgeW> TNet;
  typedef TPt<TNet> PNet;
  typedef TPair<TInt, TEdgeW> TNIdEdgeW;
public:
  /// Edge class used to store edge information ##TWNGraph::TEdge::Class
  class TNode {
  private:
    TInt Id;
    TVec<TNIdEdgeW> InNIdEdgeWV, OutNIdEdgeWV;
  private:
    // Get in / out / undirected edge methods (allows both const and non const return usage)
    TNIdEdgeW& GetInNIdEdgeW(const int& EdgeN) { return InNIdEdgeWV[EdgeN]; }
    const TNIdEdgeW& GetInNIdEdgeW(const int& EdgeN) const { return InNIdEdgeWV[EdgeN]; }
    TNIdEdgeW& GetOutNIdEdgeW(const int& EdgeN) { return OutNIdEdgeWV[EdgeN]; }
    const TNIdEdgeW& GetOutNIdEdgeW(const int& EdgeN) const { return OutNIdEdgeWV[EdgeN]; }
    TNIdEdgeW& GetNbrNIdEdgeW(const int& EdgeN) { return EdgeN < GetOutDeg() ? GetOutNIdEdgeW(EdgeN) : GetInNIdEdgeW(EdgeN - GetOutDeg()); }
    const TNIdEdgeW& GetNbrNIdEdgeW(const int& EdgeN) const { return EdgeN < GetOutDeg() ? GetOutNIdEdgeW(EdgeN) : GetInNIdEdgeW(EdgeN - GetOutDeg()); }
  public:
    TNode() : Id(-1), InNIdEdgeWV(), OutNIdEdgeWV() { }
    TNode(const int& NId) : Id(NId), InNIdEdgeWV(), OutNIdEdgeWV() { }
    TNode(const TNode& Node) : Id(Node.Id), InNIdEdgeWV(Node.InNIdEdgeWV), OutNIdEdgeWV(Node.OutNIdEdgeWV) { }
    TNode(TSIn& SIn) : Id(SIn), InNIdEdgeWV(SIn), OutNIdEdgeWV(SIn) { }
    void Save(TSOut& SOut) const { Id.Save(SOut); InNIdEdgeWV.Save(SOut); OutNIdEdgeWV.Save(SOut); }
    int GetId() const { return Id; }
    int GetInDeg() const { return InNIdEdgeWV.Len(); }
    int GetOutDeg() const { return OutNIdEdgeWV.Len(); }
    int GetDeg() const { return GetInDeg() + GetOutDeg(); }
    int GetDeg(const TEdgeDir& dir) const {
      int Deg = -1;
      switch(dir) {
        case edInDirected: Deg = GetInDeg(); break;
        case edOutDirected: Deg = GetOutDeg(); break;
        case edUnDirected: Deg = GetDeg(); break;
      }
      return Deg;
    }
    TEdgeW GetWInDeg() const;
    TEdgeW GetWOutDeg() const;
    TEdgeW GetWDeg() const { return GetWInDeg() + GetWOutDeg(); }
    TEdgeW GetWDeg(const TEdgeDir& dir) const {
      TEdgeW WDeg = -1;
      switch(dir) {
        case edInDirected: WDeg = GetWInDeg(); break;
        case edOutDirected: WDeg = GetWOutDeg(); break;
        case edUnDirected: WDeg = GetWDeg(); break;
      }
      return WDeg;
    }
    int GetInNId(const int& NodeN) const { return InNIdEdgeWV[NodeN].GetVal1(); }
    int GetOutNId(const int& NodeN) const { return OutNIdEdgeWV[NodeN].GetVal1(); }
    int GetNbrNId(const int& NodeN) const { return NodeN < GetOutDeg() ? GetOutNId(NodeN) : GetInNId(NodeN - GetOutDeg()); }
    int GetNbrNId(const int& EdgeN, const TEdgeDir& dir) const {
      int NId = -1;
      switch(dir) {
        case edInDirected: NId = GetInNId(EdgeN); break;
        case edOutDirected: NId = GetOutNId(EdgeN); break;
        case edUnDirected: NId = GetNbrNId(EdgeN); break;
      }
      return NId;
    }
    TEdgeW GetInEW(const int& EdgeN) const { return InNIdEdgeWV[EdgeN].GetVal2(); }
    TEdgeW GetOutEW(const int& EdgeN) const { return OutNIdEdgeWV[EdgeN].GetVal2(); }
    TEdgeW GetNbrEW(const int& EdgeN) const { return EdgeN < GetOutDeg() ? GetOutEW(EdgeN) : GetInEW(EdgeN - GetOutDeg()); }
    TEdgeW GetNbrEW(const int& EdgeN, const TEdgeDir& dir) const {
      TEdgeW W = -1;
      switch(dir) {
        case edInDirected: W = GetInEW(EdgeN); break;
        case edOutDirected: W = GetOutEW(EdgeN); break;
        case edUnDirected: W = GetNbrEW(EdgeN); break;
      }
      return W;
    }
    void SetInEW(const int& EdgeN, const TEdgeW& W) { InNIdEdgeWV[EdgeN].Val2 = W; }
    void SetOutEW(const int& EdgeN, const TEdgeW& W) { OutNIdEdgeWV[EdgeN].Val2 = W; }
    void SetNbrEW(const int& EdgeN, const TEdgeW& W) { EdgeN < GetOutDeg() ? SetOutEW(EdgeN, W) : SetInEW(EdgeN - GetOutDeg(), W); }
    void SetNbrEW(const int& EdgeN, const TEdgeW& W, const TEdgeDir& dir) {
      switch(dir) {
        case edInDirected: SetInEW(EdgeN, W); break;
        case edOutDirected: SetOutEW(EdgeN, W); break;
        case edUnDirected: SetNbrEW(EdgeN, W); break;
      }
    }
    
    // Needed for IsEdge method (and for traingle counting algorithms)
    bool IsInNId(const int& NId, int& NodeN) const;
    bool IsInNId(const int& NId) const { int NodeN; return IsInNId(NId, NodeN); }
    bool IsOutNId(const int& NId, int& NodeN) const;
    bool IsOutNId(const int& NId) const { int NodeN; return IsOutNId(NId, NodeN); }
    bool IsNbrNId(const int& NId, int& NodeN) const { if (IsOutNId(NId, NodeN)) { return true; } else if (IsInNId(NId, NodeN)) { NodeN += GetOutDeg(); return true; } else { return false; } }
    bool IsNbrNId(const int& NId) const { return IsOutNId(NId) || IsInNId(NId); }
    
    void PackInNIdEdgeW() { InNIdEdgeWV.Pack(); }
    void PackOutNIdEdgeW() { OutNIdEdgeWV.Pack(); }
    void Pack() { InNIdEdgeWV.Pack(); OutNIdEdgeWV.Pack(); }
    friend class TWNGraph<TEdgeW>;
    // friend class TWNGraphWMtx<TWdgeW>; // required for weighted adjacency matrix SVD
  };
  /// Node iterator. Only forward iteration (operator++) is supported.
  class TNodeI {
  private:
    typedef typename THash<TInt, TNode>::TIter THashIter;
    THashIter NodeHI;
  public:
    TNodeI() : NodeHI() { }
    TNodeI(const THashIter& NodeHIter) : NodeHI(NodeHIter) { }
    TNodeI(const TNodeI& NodeI) : NodeHI(NodeI.NodeHI) { }
    TNodeI& operator = (const TNodeI& NodeI) { NodeHI = NodeI.NodeHI; return *this; }
    /// Increment iterator.
    TNodeI& operator++ (int) { NodeHI++; return *this; }
    /// Decrement iterator.
    TNodeI& operator-- (int) { NodeHI--; return *this; }
    // Methods for ordering.
    bool operator == (const TNodeI& NodeI) const { return NodeHI == NodeI.NodeHI; }
    bool operator != (const TNodeI& NodeI) const { return NodeHI != NodeI.NodeHI; }
    bool operator < (const TNodeI& NodeI) const { return NodeHI < NodeI.NodeHI; }
    bool operator > (const TNodeI& NodeI) const { return NodeHI > NodeI.NodeHI; }
    /// Returns ID of the current node.
    int GetId() const { return NodeHI.GetDat().GetId(); }
    /// Returns in-degree of the current node.
    int GetInDeg() const { return NodeHI.GetDat().GetInDeg(); }
    /// Returns out-degree of the current node.
    int GetOutDeg() const { return NodeHI.GetDat().GetOutDeg(); }
    /// Returns degree of the current node, the sum of in-degree and out-degree.
    int GetDeg() const { return NodeHI.GetDat().GetDeg(); }
    /// Returns degree of the current node, for the direction specified.
    int GetDeg(const TEdgeDir& dir) const { return NodeHI.GetDat().GetDeg(dir); }
    /// Returns weighted in-degree of the current node.
    TEdgeW GetWInDeg() const { return NodeHI.GetDat().GetWInDeg(); }
    /// Returns weighted out-degree of the current node.
    TEdgeW GetWOutDeg() const { return NodeHI.GetDat().GetWOutDeg(); }
    /// Returns weighted degree of the current node, the sum of weighted in-degree and weighted out-degree.
    TEdgeW GetWDeg() const { return NodeHI.GetDat().GetWDeg(); }
    /// Returns weighted degree of the current node, for the direction specified.
    TEdgeW GetWDeg(const TEdgeDir& dir) const { return NodeHI.GetDat().GetWDeg(dir); }
    /// Returns ID of NodeN-th in-node (the node pointing to the current node).
    int GetInNId(const int& EdgeN) const { return NodeHI.GetDat().GetInNId(EdgeN); }
    /// Returns ID of NodeN-th out-node (the node the current node points to).
    int GetOutNId(const int& EdgeN) const { return NodeHI.GetDat().GetOutNId(EdgeN); }
    /// Returns ID of NodeN-th neighboring node.
    int GetNbrNId(const int& EdgeN) const { return NodeHI.GetDat().GetNbrNId(EdgeN); }
    /// Returns ID of NodeN-th neighboring node, for the direction specififed.
    int GetNbrNId(const int& EdgeN, const TEdgeDir& dir) const { return NodeHI.GetDat().GetNbrNId(EdgeN, dir); }
    /// Returns weight of EdgeN-th in-edge.
    TEdgeW GetInEW(const int& EdgeN) const { return NodeHI.GetDat().GetInEW(EdgeN); }
    /// Returns weight of EdgeN-th out-edge.
    TEdgeW GetOutEW(const int& EdgeN) const { return NodeHI.GetDat().GetOutEW(EdgeN); }
    /// Returns weight of EdgeN-th edge.
    TEdgeW GetNbrEW(const int& EdgeN) const { return NodeHI.GetDat().GetNbrEW(EdgeN); }
    /// Returns weight of EdgeN-th edge.
    TEdgeW GetNbrEW(const int& EdgeN, const TEdgeDir& dir) const { return NodeHI.GetDat().GetNbrEW(EdgeN, dir); }
    
    /// Tests whether node with ID NId points to the current node.
    bool IsInNId(const int& NId) const { return NodeHI.GetDat().IsInNId(NId); }
    /// Tests whether the current node points to node with ID NId.
    bool IsOutNId(const int& NId) const { return NodeHI.GetDat().IsOutNId(NId); }
    /// Tests whether node with ID NId is a neighbor of the current node.
    bool IsNbrNId(const int& NId) const { return IsOutNId(NId) || IsInNId(NId); }
    
    /// Tests whether node with ID NId points to the current node.
    bool IsInNId(const int& NId, int& NodeN) const { return NodeHI.GetDat().IsInNId(NId, NodeN); }
    /// Tests whether the current node points to node with ID NId.
    bool IsOutNId(const int& NId, int& NodeN) const { return NodeHI.GetDat().IsOutNId(NId, NodeN); }
    /// Tests whether node with ID NId is a neighbor of the current node.
    bool IsNbrNId(const int& NId, int& NodeN) const { return IsOutNId(NId, NodeN) || IsInNId(NId, NodeN); }
    
    friend class TWNGraph<TEdgeW>;
  };
  /// Edge iterator. Only forward iteration (operator++) is supported.
  class TEdgeI {
  private:
    TNodeI BegNode, CurNode, EndNode;
    int CurEdge;
  public:
    TEdgeI() : CurNode(), EndNode(), CurEdge(0) { }
    TEdgeI(const TNodeI& BegNodeI, const TNodeI& NodeI, const TNodeI& EndNodeI, const int& EdgeN = 0) : BegNode(BegNodeI), CurNode(NodeI), EndNode(EndNodeI), CurEdge(EdgeN) { }
    TEdgeI(const TEdgeI& EdgeI) : BegNode(EdgeI.BegNode), CurNode(EdgeI.CurNode), EndNode(EdgeI.EndNode), CurEdge(EdgeI.CurEdge) { }
    TEdgeI& operator = (const TEdgeI& EdgeI) { if (this!=&EdgeI) { CurNode=EdgeI.CurNode; EndNode=EdgeI.EndNode; CurEdge=EdgeI.CurEdge; }  return *this; }
    /// Increment iterator.
    TEdgeI& operator++ (int) {
      // Make sure the current node has nonzero outdegree
      // before incrementing CurEdge
      // Otherwise, we run into errors.
      // This is possible if a pruning algorithm is being
      // applied, removing edges in place; if no edges are
      // left, this case will occur.
      // if (CurNode.GetOutDeg() != 0) {
        CurEdge++;
        if (CurEdge >= CurNode.GetOutDeg()) { // this will already take into account the case for the code that is commented
          CurEdge = 0;
          CurNode++;
          while (CurNode < EndNode && CurNode.GetOutDeg() == 0) {
            CurNode++;
          }
        }
        return *this;
      // This handles the case detailed above by skipping over
      // empty nodes until it reaches one that has edges to be
      // iterated over.
      // } else {
      //   while (CurNode < EndNode && CurNode.GetOutDeg() == 0) {
      //     CurNode++;
      //   }
      //   return *this;
      // }
    }
    /// Decrement iterator.
    TEdgeI& operator-- (int) {
      CurEdge--;
      if (CurEdge < 0) {
        CurNode--;
        while (CurNode > BegNode && CurNode.GetOutDeg() == 0) {
          CurNode--;
        }
        CurEdge = CurNode.GetOutDeg() - 1;
      }
      return *this;
    }
    
    // TODO: implement random access iterator (?)
    
    // Methods for ordering.
    bool operator == (const TEdgeI& EdgeI) const { return CurNode == EdgeI.CurNode && CurEdge == EdgeI.CurEdge; }
    bool operator != (const TEdgeI& EdgeI) const { return CurNode != EdgeI.CurNode || CurEdge != EdgeI.CurEdge; }
    bool operator < (const TEdgeI& EdgeI) const { return CurNode < EdgeI.CurNode || (CurNode == EdgeI.CurNode && CurEdge < EdgeI.CurEdge); }
    bool operator > (const TEdgeI& EdgeI) const { return CurNode > EdgeI.CurNode || (CurNode == EdgeI.CurNode && CurEdge > EdgeI.CurEdge); }
    /// Returns the source node of the edge.
    int GetSrcNId() const { return CurNode.GetId(); }
    /// Returns the destination node of the edge.
    int GetDstNId() const { return CurNode.GetOutNId(CurEdge); }
    /// Gets the weight of an edge.
    TEdgeW GetW() const { return CurNode.GetOutEW(CurEdge); }
    friend class TWNGraph;
  };
private:
  // Get node methods (allows both const and non const return usage)
  TNode& GetNode(const int& NId) { return NodeH.GetDat(NId); }
  const TNode& GetNode(const int& NId) const { return NodeH.GetDat(NId); }
private:
  TCRef CRef;
  TInt MxNId;
  THash<TInt, TNode> NodeH;
public:
  TWNGraph() : CRef(), MxNId(0), NodeH() { }
  /// Constructor that reserves enough memory for a graph of Nodes nodes and Edges edges.
  explicit TWNGraph(const int& Nodes, const int& Edges) : MxNId(0) { Reserve(Nodes, Edges); }
  TWNGraph(const TWNGraph& Graph) : MxNId(Graph.MxNId), NodeH(Graph.NodeH) { }
  /// Constructor that loads the graph from a (binary) stream SIn.
  TWNGraph(TSIn& SIn) : MxNId(SIn), NodeH(SIn) { }
  /// Saves the graph to a (binary) stream SOut.
  void Save(TSOut& SOut) const { MxNId.Save(SOut); NodeH.Save(SOut); }
  /// Static constructor that returns a pointer to the graph. Call: PNGraph Graph = TWNGraph::New().
  static PNet New() { return PNet(new TNet()); }
  /// Static constructor that returns a pointer to the graph and reserves enough memory for Nodes nodes and Edges edges.
  static PNet New(const int& Nodes, const int& Edges) { return PNet(new TNet(Nodes, Edges)); }
  /// Static constructor that loads the graph from a stream SIn and returns a pointer to it.
  static PNet Load(TSIn& SIn) { return PNet(new TNet(SIn)); }
  /// Allows for run-time checking the type of the graph (see the TGraphFlag for flags).
  bool HasFlag(const TGraphFlag& Flag) const {
    return HasGraphFlag(TWNGraph::TNet, Flag);
  }
  TNet& operator = (const TNet& Graph) { if (this != &Graph) { MxNId = Graph.MxNId; NodeH = Graph.NodeH; } return *this; }
  
  /// Returns the number of nodes in the graph.
  int GetNodes() const { return NodeH.Len(); }
  /// Adds a node of ID NId to the graph.
  int AddNode(int NId = -1);
  /// Adds a node of ID NodeI.GetId() to the graph.
  int AddNode(const TNodeI& NodeId) { return AddNode(NodeId.GetId()); }
  /// Deletes node of ID NId from the graph. 
  void DelNode(const int& NId);
  /// Deletes node of ID NodeI.GetId() from the graph.
  void DelNode(TNode& NodeI) { DelNode(NodeI.GetId()); }
  /// Tests whether ID NId is a node.
  bool IsNode(const int& NId) const { return NodeH.IsKey(NId); }
  /// Returns an iterator referring to the first node in the graph.
  TNodeI BegNI() const { return TNodeI(NodeH.BegI()); }
  /// Returns an iterator referring to the past-the-end node in the graph.
  TNodeI EndNI() const { return TNodeI(NodeH.EndI()); }
  /// Returns an iterator referring to the node of ID NId in the graph.
  TNodeI GetNI(const int& NId) const { return TNodeI(NodeH.GetI(NId)); }
  /// Returns an ID that is larger than any node ID in the graph.
  int GetMxNId() const { return MxNId; }
  
  /// Returns the number of edges in the graph.
  int GetEdges() const;
  /// Adds an edge from node IDs SrcNId to node DstNId to the graph.
  int AddEdge(const int& SrcNId, const int& DstNId, const TEdgeW& W = 1);
  /// Adds an edge from EdgeI.GetSrcNId() to EdgeI.GetDstNId() to the graph.
  int AddEdge(const TEdgeI& EdgeI) { return AddEdge(EdgeI.GetSrcNId(), EdgeI.GetDstNId(), EdgeI.GetW()); }
  /// Deletes an edge from node IDs SrcNId to DstNId from the graph.
  void DelEdge(const int& SrcNId, const int& DstNId, const bool& IsDir = true);
  /// Deletes an edge from the edge iterator EI, checking internal consistency for EI
  void DelEdge(TEdgeI& EdgeI);
  /// Tests whether an edge from node IDs SrcNId to DstNId exists in the graph.
  bool IsEdge(const int& SrcNId, const int& DstNId, const bool& IsDir = true) const;
  /// Returns an iterator referring to the first edge in the graph.
  TEdgeI BegEI() const {
    TNodeI NI = BegNI();
    while(NI < EndNI() && NI.GetOutDeg() == 0) {
      NI++;
    }
    return TEdgeI(BegNI(), NI, EndNI());
  }
  /// Returns an iterator referring to the past-the-end edge in the graph.
  TEdgeI EndEI() const {
    return TEdgeI(BegNI(), EndNI(), EndNI());
  }
  /// Returns an iterator referring to edge (SrcNId, DstNId) in the graph.
  TEdgeI GetEI(const int& SrcNId, const int& DstNId) const {
    const TNode& SrcNode = GetNode(SrcNId);
    int EdgeN;
    SrcNode.IsOutNId(DstNId, EdgeN);
    return(TEdgeI(BegNI(), GetNI(SrcNId), EndNI(), EdgeN));
  }
    
  /// Returns the edge weight corresponding to the edge SrcNId and DstNId.
  TEdgeW GetEW(const int& SrcNId, const int& DstNId);
  /// Sets the edge weight corresponding to the edge SrcNId and DstNId.
  void SetEW(const int& SrcNId, const int& DstNId, const TEdgeW& W = 1);
  /// Returns the total weight in the graph.
  TEdgeW GetTotalW();
  /// Returns the maximum edge weight in the graph.
  TEdgeW GetMxW();
  
  /// Returns an ID of a random node in the graph.
  int GetRndNId(TRnd& Rnd = TInt::Rnd) { return NodeH.GetKey(NodeH.GetRndKeyId(Rnd, 0.8)); }
  /// Returns an interator referring to a random node in the graph.
  TNodeI GetRndNI(TRnd& Rnd = TInt::Rnd) { return GetNI(GetRndNId(Rnd)); }
  /// Gets a vector IDs of all nodes in the graph.
  void GetNIdV(TIntV& NIdV) const;

  /// Tests whether the graph is empty (has zero nodes).
  bool Empty() const { return GetNodes() == 0; }
  /// Deletes all nodes and edges from the graph.
  void Clr() { MxNId = 0; NodeH.Clr(); }
  /// Reserves memory for a graph of Nodes nodes and Edges edges.
  void Reserve(const int& Nodes, const int& Edges) { if (Nodes > 0) { NodeH.Gen(Nodes/2); } }
  /// Reserves memory for node ID NId having InDeg in-edges.
  void ReserveNIdInDeg(const int& NId, const int& InDeg) { GetNode(NId).InNIdEdgeWV.Reserve(InDeg); }
  /// Reserves memory for node ID NId having OutDeg out-edges.
  void ReserveNIdOutDeg(const int& NId, const int& OutDeg) { GetNode(NId).OutNIdEdgeWV.Reserve(OutDeg); }
  /// Defragments the graph. ##TWNGraph::Defrag
  void Defrag(const bool& OnlyNodeLinks = false);
  /// Checks the graph data structure for internal consistency.
  bool IsOk(const bool& ThrowExcept = true) const;
  /// Print the graph in a human readable form to an output stream OutF.
  void Dump(FILE *OutF = stdout) const;
  /// Returns a small graph on 5 nodes and 6 edges. ##TWNGraph::GetSmallGraph
  static PNet GetSmallGraph();
  friend class TPt<TWNGraph<TEdgeW> >;
  // friend class TWNGraphWMtx<TWdgeW>; // required for weighted adjacency matrix SVD
};

// Templated (requires template alias / C++11)
// template <class TEdgeW>
// using PWNGraph = TPt<TWNGraph<TEdgeW> >;

// set flags
namespace TSnap {
template <class TEdgeW> struct IsDirected<TWNGraph<TEdgeW> > { enum { Val = 1 }; };
}

// TNode methods implemented

// Effectively binary search on SrcNId / DstNId (ignoring weights)
template <class TEdgeW>
bool TWNGraph<TEdgeW>::TNode::IsInNId(const int& NId, int& NodeN) const {
  int LValN = 0, RValN = GetInDeg() - 1;
  while (RValN >= LValN) {
    NodeN = (LValN + RValN) / 2;
    if (NId == InNIdEdgeWV[NodeN].GetVal1()) { return true; }
    if (NId < InNIdEdgeWV[NodeN].GetVal1()) {
      RValN = NodeN - 1;
    } else {
      LValN = NodeN + 1;
    }
  }
  NodeN = -1;
  return false;
}

// Effectively binary search on SrcNId / DstNId (ignoring weights) 
template <class TEdgeW>
bool TWNGraph<TEdgeW>::TNode::IsOutNId(const int& NId, int& NodeN) const {
  int LValN = 0, RValN = GetOutDeg() - 1;
  while (RValN >= LValN) {
    NodeN = (LValN + RValN) / 2;
    if (NId == OutNIdEdgeWV[NodeN].GetVal1()) { return true; }
    if (NId < OutNIdEdgeWV[NodeN].GetVal1()) {
      RValN = NodeN - 1;
    } else {
      LValN = NodeN + 1;
    }
  }
  NodeN = -1;
  return false;
}

// TNodeI methods implemented

template <class TEdgeW>
TEdgeW TWNGraph<TEdgeW>::TNode::GetWInDeg() const {
  TEdgeW WDeg = 0;
  for (int edge = 0; edge < GetInDeg(); edge++) {
    WDeg += GetInEW(edge); // Graph->GetEdge(NodeHI.GetDat().GetInEId(edge)).GetW();
  }
  return WDeg;
}

template <class TEdgeW>
TEdgeW TWNGraph<TEdgeW>::TNode::GetWOutDeg() const {
  TEdgeW WDeg = 0;
  for (int edge = 0; edge < GetOutDeg(); edge++) {
    WDeg += GetOutEW(edge); // Graph->GetEdge(NodeHI.GetDat().GetOutEId(edge)).GetW();
  }
  return WDeg;
}

// TWNGraph<TEdgeW> methods implemented

template <class TEdgeW>
int TWNGraph<TEdgeW>::AddNode(int NId) {
  if (NId == -1) {
    NId = MxNId;  MxNId++;
  } else {
    IAssertR(!IsNode(NId), TStr::Fmt("NodeId %d already exists", NId));
    MxNId = TMath::Mx(NId+1, MxNId());
  }
  NodeH.AddDat(NId, TNode(NId));
  return NId;
}

template <class TEdgeW>
void TWNGraph<TEdgeW>::DelNode(const int& NId) {
  const TNode& Node = GetNode(NId);
  int node;
  for (int edge = 0; edge < Node.GetOutDeg(); edge++) {
    const int DstNId = Node.GetOutNId(edge);
    TNode& N = GetNode(DstNId);
    N.IsInNId(NId, node);
    if (node != -1) { N.InNIdEdgeWV.Del(node); }
  }
  for (int edge = 0; edge < Node.GetInDeg(); edge++) {
    const int SrcNId = Node.GetInNId(edge);
    TNode& N = GetNode(SrcNId);
    N.IsOutNId(NId, node);
    if (node != -1) { N.OutNIdEdgeWV.Del(node); }
  }
  NodeH.DelKey(NId);
}

template <class TEdgeW>
int TWNGraph<TEdgeW>::GetEdges() const {
  int edges = 0;
  for (int N = NodeH.FFirstKeyId(); NodeH.FNextKeyId(N); ) {
    edges += NodeH[N].GetOutDeg();
  }
  return edges;
}

template <class TEdgeW>
int TWNGraph<TEdgeW>::AddEdge(const int& SrcNId, const int& DstNId, const TEdgeW& W) {
  typedef TPair<TInt, TEdgeW> TNIdEdgeW;
  IAssertR(IsNode(SrcNId) && IsNode(DstNId), TStr::Fmt("%d or %d not a node.", SrcNId, DstNId).CStr());
  IAssertR(W > 0, "Weight not positive (stricly).");
  if (IsEdge(SrcNId, DstNId)) {
    // increment edge weight if edge exists
    int EdgeN;
    GetNode(SrcNId).IsOutNId(DstNId, EdgeN);
    GetNode(SrcNId).OutNIdEdgeWV[EdgeN].Val2 += W;
    GetNode(DstNId).IsInNId(SrcNId, EdgeN);
    GetNode(DstNId).InNIdEdgeWV[EdgeN].Val2 += W;
    return -1;
  }
  GetNode(SrcNId).OutNIdEdgeWV.AddSorted(TNIdEdgeW(DstNId, W));
  GetNode(DstNId).InNIdEdgeWV.AddSorted(TNIdEdgeW(SrcNId, W));
  return -1;
}

template <class TEdgeW>
void TWNGraph<TEdgeW>::DelEdge(const int& SrcNId, const int& DstNId, const bool& IsDir) {
  IAssertR(IsNode(SrcNId) && IsNode(DstNId), TStr::Fmt("%d or %d not a node.", SrcNId, DstNId).CStr());
  int node;
  { TNode& N = GetNode(SrcNId);
  N.IsOutNId(DstNId, node);
  if (node != -1) { N.OutNIdEdgeWV.Del(node); } }
  { TNode& N = GetNode(DstNId);
  N.IsInNId(SrcNId, node);
  if (node != -1) { N.InNIdEdgeWV.Del(node); } }
  if (!IsDir) {
    { TNode& N = GetNode(SrcNId);
    N.IsInNId(DstNId, node);
    if (node != -1) { N.InNIdEdgeWV.Del(node); } }
    { TNode& N = GetNode(DstNId);
    N.IsOutNId(SrcNId, node);
    if (node != -1) { N.OutNIdEdgeWV.Del(node); } }
  }
}

template <class TEdgeW>
void TWNGraph<TEdgeW>::DelEdge(TEdgeI& EdgeI) {
  const int SrcNId = EdgeI.GetSrcNId(), DstNId = EdgeI.GetDstNId();
  DelEdge(SrcNId, DstNId);
  if (EdgeI.CurEdge >= EdgeI.CurNode.GetOutDeg()) {
    EdgeI++;
  }
}

template <class TEdgeW>
bool TWNGraph<TEdgeW>::IsEdge(const int& SrcNId, const int& DstNId, const bool& IsDir) const {
  if (! IsNode(SrcNId) || ! IsNode(DstNId)) { return false; }
  if (IsDir) { return GetNode(SrcNId).IsOutNId(DstNId); }
  else { return GetNode(SrcNId).IsOutNId(DstNId) || GetNode(DstNId).IsOutNId(SrcNId); }
}

template <class TEdgeW>
TEdgeW TWNGraph<TEdgeW>::GetEW(const int& SrcNId, const int& DstNId) {
  const TNode& SrcNode = GetNode(SrcNId);
  int EdgeN;
  SrcNode.IsOutNId(DstNId, EdgeN);
  return(SrcNode.GetOutEW(EdgeN));
}

template <class TEdgeW>
void TWNGraph<TEdgeW>::SetEW(const int& SrcNId, const int& DstNId, const TEdgeW& W) {
  TNode& SrcNode = GetNode(SrcNId);
  TNode& DstNode = GetNode(DstNId);
  int EdgeN;
  SrcNode.IsOutNId(DstNId, EdgeN); SrcNode.SetOutEW(EdgeN, W);
  DstNode.IsInNId(SrcNId, EdgeN); DstNode.SetInEW(EdgeN, W);
}

template <class TEdgeW>
TEdgeW TWNGraph<TEdgeW>::GetTotalW() {
  TEdgeW TotalW = 0;
  for (TEdgeI EI = BegEI(); EI < EndEI(); EI++) {
    TotalW += EI.GetW();
  }
  return TotalW;
}

template <class TEdgeW>
TEdgeW TWNGraph<TEdgeW>::GetMxW() {
  TEdgeW W, MxW = 0;
  for (TEdgeI EI = BegEI(); EI < EndEI(); EI++) {
    W = EI.GetW();
    if (W > MxW) { MxW = W; }
  }
  return MxW;
}

template <class TEdgeW>
void TWNGraph<TEdgeW>::GetNIdV(TIntV& NIdV) const {
  NIdV.Gen(GetNodes(), 0);
  for (int N=NodeH.FFirstKeyId(); NodeH.FNextKeyId(N); ) {
    NIdV.Add(NodeH.GetKey(N));
  }
}

template <class TEdgeW>
void TWNGraph<TEdgeW>::Defrag(const bool& OnlyNodeLinks) {
  for (int KId = NodeH.FFirstKeyId(); NodeH.FNextKeyId(KId); ) {
    // TNode& Node = NodeH[KId];
    // Node.Pack();
    NodeH[KId].Pack();
  }
  if (! OnlyNodeLinks && ! NodeH.IsKeyIdEqKeyN()) { NodeH.Defrag(); }
}

template <class TEdgeW>
bool TWNGraph<TEdgeW>::IsOk(const bool& ThrowExcept) const {
  bool RetVal = true;
  for (int N = NodeH.FFirstKeyId(); NodeH.FNextKeyId(N); ) {
    const TNode& Node = NodeH[N];
    if (! Node.OutNIdEdgeWV.IsSorted()) {
      const TStr Msg = TStr::Fmt("Out-neighbor list of node %d is not sorted.", Node.GetId());
      if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
    }
    if (! Node.InNIdEdgeWV.IsSorted()) {
      const TStr Msg = TStr::Fmt("In-neighbor list of node %d is not sorted.", Node.GetId());
      if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
    }
    // Check out-edges
    int prevNId = -1;
    for (int e = 0; e < Node.GetOutDeg(); e++) {
      if (! IsNode(Node.GetOutNId(e))) {
        const TStr Msg = TStr::Fmt("Out-edge %d --> %d: node %d does not exist.",
          Node.GetId(), Node.GetOutNId(e), Node.GetOutNId(e));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      if (e > 0 && prevNId == Node.GetOutNId(e)) {
        const TStr Msg = TStr::Fmt("Node %d has duplidate out-edge %d --> %d.",
          Node.GetId(), Node.GetId(), Node.GetOutNId(e));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      prevNId = Node.GetOutNId(e);
    }
    // Check in-edges
    prevNId = -1;
    for (int e = 0; e < Node.GetInDeg(); e++) {
      if (! IsNode(Node.GetInNId(e))) {
        const TStr Msg = TStr::Fmt("In-edge %d <-- %d: node %d does not exist.",
          Node.GetId(), Node.GetInNId(e), Node.GetInNId(e));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      if (e > 0 && prevNId == Node.GetInNId(e)) {
        const TStr Msg = TStr::Fmt("Node %d has duplidate in-edge %d <-- %d.",
          Node.GetId(), Node.GetId(), Node.GetInNId(e));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      prevNId = Node.GetInNId(e);
    }
    
    // Check out edge weight consistency
    for (int e = 0; e < Node.GetOutDeg(); e++) {
      int n;
      const TNode& DstNode = GetNode(Node.GetOutNId(e));
      bool exists = DstNode.IsInNId(Node.GetId(), n);
      if (!exists) {
        const TStr Msg = TStr::Fmt("Out-edge %d --> %d not consistent with corresponding in-edge %d <-- %d.",
          Node.GetId(), Node.GetOutNId(e), DstNode.GetId(), DstNode.GetInNId(n));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      if (Node.GetOutEW(e) != DstNode.GetInEW(n)) {
        const TStr Msg = TStr::Fmt("Out-edge %d --> %d weight %f not consistent with corresponding in-edge %d <-- %d weight %f.",
          Node.GetId(), Node.GetOutNId(e), double(Node.GetOutEW(e)),
          DstNode.GetId(), DstNode.GetInNId(n), double(DstNode.GetInEW(n)));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
    }
    // Check in edge weight consistency
    for (int e = 0; e < Node.GetInDeg(); e++) {
      int n;
      const TNode& SrcNode = GetNode(Node.GetInNId(e));
      bool exists = SrcNode.IsOutNId(Node.GetId(), n);
      if (!exists) {
        const TStr Msg = TStr::Fmt("In-edge %d <-- %d not consistent with corresponding out-edge %d --> %d.",
          Node.GetId(), Node.GetInNId(e), SrcNode.GetId(), SrcNode.GetOutNId(n));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      if (Node.GetInEW(e) != SrcNode.GetOutEW(n)) {
        const TStr Msg = TStr::Fmt("Out-edge %d --> %d weight %f not consistent with corresponding in-edge %d <-- %d weight %f.",
          Node.GetId(), Node.GetInNId(e), double(Node.GetInEW(e)),
          SrcNode.GetId(), SrcNode.GetOutNId(n), double(SrcNode.GetOutEW(n)));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
    }
  }
  return RetVal;
}

template <class TEdgeW>
void TWNGraph<TEdgeW>::Dump(FILE *OutF) const {
  const int NodePlaces = (int) ceil(log10((double) GetNodes()));
  const int EdgePlaces = (int) ceil(log10((double) GetEdges()));
  const int WPlaces = (int) ceil(log10((double) GetMxW()));
  fprintf(OutF, "-------------------------------------------------\nDirected Node-Edge Graph: nodes: %d, edges: %d\n", GetNodes(), GetEdges());
  for (TNodeI NodeI = BegNI(); NodeI < EndNI(); NodeI++) {
    fprintf(OutF, "  %*d]\n", NodePlaces, NodeI.GetId());
    fprintf(OutF, "    in[%d]", NodeI.GetInDeg());
    for (int edge = 0; edge < NodeI.GetInDeg(); edge++) {
      fprintf(OutF, " %*d:%*f", NodePlaces, NodeI.GetInNId(edge), WPlaces, double(NodeI.GetInEW(edge))); }
    fprintf(OutF, "\n    out[%d]", NodeI.GetOutDeg());
    for (int edge = 0; edge < NodeI.GetOutDeg(); edge++) {
      fprintf(OutF, " %*d:%*f", NodePlaces, NodeI.GetOutNId(edge), WPlaces, double(NodeI.GetOutEW(edge))); }
    fprintf(OutF, "\n");
  }
  fprintf(OutF, "\n");
}

//#//////////////////////////////////////////////
/// Weighted directed multigraph.
template <class TEdgeW> // TEdgeW can be TInt or TFlt (which must be numeric typed, should this be restricted (?))
class TWNEGraph {
public:
  typedef TWNEGraph<TEdgeW> TNet;
  typedef TPt<TNet> PNet;
public:
  /// Edge class used to store edge information
  class TEdge {
  private:
    TInt Id, SrcNId, DstNId;
    TEdgeW EdgeW;
  public:
    TEdge() : Id(-1), SrcNId(-1), DstNId(-1), EdgeW(1) { }
    TEdge(const int& EId, const int& SourceNId, const int& DestNId, const TEdgeW& W) : Id(EId), SrcNId(SourceNId), DstNId(DestNId), EdgeW(W) { }
    TEdge(const TEdge& Edge) : Id(Edge.Id), SrcNId(Edge.SrcNId), DstNId(Edge.DstNId), EdgeW(Edge.EdgeW) { }
    TEdge(TSIn& SIn) : Id(SIn), SrcNId(SIn), DstNId(SIn), EdgeW(SIn) { }
    void Save(TSOut& SOut) const { Id.Save(SOut); SrcNId.Save(SOut); DstNId.Save(SOut); EdgeW.Save(SOut); }
    // Required for AddSorted
    bool operator == (const TEdge& E) const { return Id == E.Id; }
    bool operator != (const TEdge& E) const { return Id != E.Id; }
    bool operator < (const TEdge& E) const { return Id < E.Id; }
    bool operator > (const TEdge& E) const { return Id > E.Id; }
    // Edge topology accessor method(s)
    int GetId() const { return Id; }
    int GetSrcNId() const { return SrcNId; }
    int GetDstNId() const { return DstNId; }
    // Edge weight accessor method
    TEdgeW GetW() const { return EdgeW; }
    // Edge weight setter method
    void SetW(const TEdgeW& W) { EdgeW = W; }
    friend class TWNEGraph<TEdgeW>;
  };
  class TNode {
  private:
    TInt Id;
    TVec<TEdge> InEdgeV, OutEdgeV;
  private:
    // Get in / out / undirected edge methods (allows both const and non const return usage)
    TEdge& GetInEdge(const int& EdgeN) { return InEdgeV[EdgeN]; }
    const TEdge& GetInEdge(const int& EdgeN) const { return InEdgeV[EdgeN]; }
    TEdge& GetOutEdge(const int& EdgeN) { return OutEdgeV[EdgeN]; }
    const TEdge& GetOutEdge(const int& EdgeN) const { return OutEdgeV[EdgeN]; }
    TEdge& GetNbrEdge(const int& EdgeN) { return EdgeN<GetOutDeg()?GetOutEdge(EdgeN):GetInEdge(EdgeN-GetOutDeg()); }
    const TEdge& GetNbrEdge(const int& EdgeN) const { return EdgeN<GetOutDeg()?GetOutEdge(EdgeN):GetInEdge(EdgeN-GetOutDeg()); }
  public:
    TNode() : Id(-1), InEdgeV(), OutEdgeV() { }
    TNode(const int& NId) : Id(NId), InEdgeV(), OutEdgeV() { }
    TNode(const TNode& Node) : Id(Node.Id), InEdgeV(Node.InEdgeV), OutEdgeV(Node.OutEdgeV) { }
    TNode(TSIn& SIn) : Id(SIn), InEdgeV(SIn), OutEdgeV(SIn) { }
    void Save(TSOut& SOut) const { Id.Save(SOut); InEdgeV.Save(SOut); OutEdgeV.Save(SOut); }
    int GetId() const { return Id; }
    int GetInDeg() const { return InEdgeV.Len(); }
    int GetOutDeg() const { return OutEdgeV.Len(); }
    int GetDeg() const { return GetInDeg() + GetOutDeg(); }
    int GetDeg(const TEdgeDir& dir) const {
      int Deg = -1;
      switch(dir) {
        case edInDirected: Deg = GetInDeg(); break;
        case edOutDirected: Deg = GetOutDeg(); break;
        case edUnDirected: Deg = GetDeg(); break;
      }
      return Deg;
    }
    TEdgeW GetWInDeg() const;
    TEdgeW GetWOutDeg() const;
    TEdgeW GetWDeg() const { return GetWInDeg() + GetWOutDeg(); }
    TEdgeW GetWDeg(const TEdgeDir& dir) const {
      TEdgeW WDeg = -1;
      switch(dir) {
        case edInDirected: WDeg = GetWInDeg(); break;
        case edOutDirected: WDeg = GetWOutDeg(); break;
        case edUnDirected: WDeg = GetWDeg(); break;
      }
      return WDeg;
    }
    int GetInNId(const int& NodeN) const { return InEdgeV[NodeN].GetSrcNId(); }
    int GetOutNId(const int& NodeN) const { return OutEdgeV[NodeN].GetDstNId(); }
    int GetNbrNId(const int& NodeN) const { return NodeN < GetOutDeg() ? GetOutNId(NodeN) : GetInNId(NodeN - GetOutDeg()); }
    int GetNbrNId(const int& EdgeN, const TEdgeDir& dir) const {
      int NId = -1;
      switch(dir) {
        case edInDirected: NId = GetInNId(EdgeN); break;
        case edOutDirected: NId = GetOutNId(EdgeN); break;
        case edUnDirected: NId = GetNbrNId(EdgeN); break;
      }
      return NId;
    }
    int GetInEId(const int& EdgeN) const { return InEdgeV[EdgeN].GetId(); }
    int GetOutEId(const int& EdgeN) const { return OutEdgeV[EdgeN].GetId(); }
    int GetNbrEId(const int& EdgeN) const { return EdgeN < GetOutDeg() ? GetOutEId(EdgeN) : GetInEId(EdgeN - GetOutDeg()); }
    int GetNbrEId(const int& EdgeN, const TEdgeDir& dir) const {
      int EId = -1;
      switch(dir) {
        case edInDirected: EId = GetInEId(EdgeN); break;
        case edOutDirected: EId = GetOutEId(EdgeN); break;
        case edUnDirected: EId = GetNbrEId(EdgeN); break;
      }
      return EId;
    }
    TEdgeW GetInEW(const int& EdgeN) const { return InEdgeV[EdgeN].GetW(); }
    TEdgeW GetOutEW(const int& EdgeN) const { return OutEdgeV[EdgeN].GetW(); }
    TEdgeW GetNbrEW(const int& EdgeN) const { return EdgeN < GetOutDeg() ? GetOutEW(EdgeN) : GetInEW(EdgeN - GetOutDeg()); }
    TEdgeW GetNbrEW(const int& EdgeN, const TEdgeDir& dir) const {
      TEdgeW W = -1;
      switch(dir) {
        case edInDirected: W = GetInEW(EdgeN); break;
        case edOutDirected: W = GetOutEW(EdgeN); break;
        case edUnDirected: W = GetNbrEW(EdgeN); break;
      }
      return W;
    }
    void SetInEW(const int& EdgeN, const TEdgeW& W) { InEdgeV[EdgeN].SetW(W); }
    void SetOutEW(const int& EdgeN, const TEdgeW& W) { OutEdgeV[EdgeN].SetW(W); }
    void SetNbrEW(const int& EdgeN, const TEdgeW& W) { EdgeN < GetOutDeg() ? SetOutEW(EdgeN, W) : SetInEW(EdgeN - GetOutDeg(), W); }
    void SetNbrEW(const int& EdgeN, const TEdgeW& W, const TEdgeDir& dir) {
      switch(dir) {
        case edInDirected: SetInEW(EdgeN, W); break;
        case edOutDirected: SetOutEW(EdgeN, W); break;
        case edUnDirected: SetNbrEW(EdgeN, W); break;
      }
    }
    
    // Binary search on GetEdge(EId)
    bool IsInEId(const int& EId, int& EdgeN) const;
    bool IsInEId(const int& EId) const { int EdgeN; return IsInEId(EId, EdgeN); }
    // Binary search on GetEdge(EId)
    bool IsOutEId(const int& EId, int& EdgeN) const;
    bool IsOutEId(const int& EId) const { int EdgeN; return IsOutEId(EId, EdgeN); }
    // Extented
    bool IsNbrEId(const int& EId, int& EdgeN) const { if (IsOutEId(EId, EdgeN)) { return true; } else if (IsInEId(EId, EdgeN)) { EdgeN += GetOutDeg(); return true; } else { return false; } }
    bool IsNbrNEd(const int& EId) const { return IsOutEId(EId) || IsInEId(EId); }
    
    // Needed for IsEdge method (and for traingle counting algorithms)
    bool IsInNId(const int& NId) const;  // { int EId; return IsEdge(NId, Id, EId, true); }
    bool IsOutNId(const int& NId) const;  //{ int EId; return IsEdge(Id, NId, EId, true); }
    bool IsNbrNId(const int& NId) const { return IsOutNId(NId) || IsInNId(NId); }
    
    void PackInEdgeV() { InEdgeV.Pack(); }
    void PackOutEdgeV() { OutEdgeV.Pack(); }
    void Pack() { InEdgeV.Pack(); OutEdgeV.Pack(); }
    friend class TWNEGraph<TEdgeW>;
    // friend class TWNEGraphWMtx<TEdgeW>; // required for weighted adjacency matrix SVD
  };
  /// Node iterator. Only forward iteration (operator++) is supported.
  class TNodeI {
  private:
    typedef typename THash<TInt, TNode>::TIter THashIter;
    THashIter NodeHI;
  public:
    TNodeI() : NodeHI() { }
    TNodeI(const THashIter& NodeHIter) : NodeHI(NodeHIter) { }
    TNodeI(const TNodeI& NodeI) : NodeHI(NodeI.NodeHI) { }
    TNodeI& operator = (const TNodeI& NodeI) { NodeHI = NodeI.NodeHI; return *this; }
    /// Increment iterator.
    TNodeI& operator++ (int) { NodeHI++; return *this; }
    /// Decrement iterator.
    TNodeI& operator-- (int) { NodeHI--; return *this; }
    // Methods for ordering.
    bool operator == (const TNodeI& NodeI) const { return NodeHI == NodeI.NodeHI; }
    bool operator != (const TNodeI& NodeI) const { return NodeHI != NodeI.NodeHI; }
    bool operator < (const TNodeI& NodeI) const { return NodeHI < NodeI.NodeHI; }
    bool operator > (const TNodeI& NodeI) const { return NodeHI > NodeI.NodeHI; }
    /// Returns ID of the current node.
    int GetId() const { return NodeHI.GetDat().GetId(); }
    /// Returns in-degree of the current node.
    int GetInDeg() const { return NodeHI.GetDat().GetInDeg(); }
    /// Returns out-degree of the current node.
    int GetOutDeg() const { return NodeHI.GetDat().GetOutDeg(); }
    /// Returns degree of the current node, the sum of in-degree and out-degree.
    int GetDeg() const { return NodeHI.GetDat().GetDeg(); }
    /// Returns degree of the current node, for the direction specified.
    int GetDeg(const TEdgeDir& dir) const { return NodeHI.GetDat().GetDeg(dir); }
    /// Returns weighted in-degree of the current node.
    TEdgeW GetWInDeg() const { return NodeHI.GetDat().GetWInDeg(); }
    /// Returns weighted out-degree of the current node.
    TEdgeW GetWOutDeg() const { return NodeHI.GetDat().GetWOutDeg(); }
    /// Returns weighted degree of the current node, the sum of weighted in-degree and weighted out-degree.
    TEdgeW GetWDeg() const { return NodeHI.GetDat().GetWDeg(); }
    /// Returns weighted degree of the current node, for the direction specified.
    TEdgeW GetWDeg(const TEdgeDir& dir) const { return NodeHI.GetDat().GetWDeg(dir); }
    /// Returns ID of NodeN-th in-node (the node pointing to the current node).
    int GetInNId(const int& EdgeN) const { return NodeHI.GetDat().GetInNId(EdgeN); }
    /// Returns ID of NodeN-th out-node (the node the current node points to).
    int GetOutNId(const int& EdgeN) const { return NodeHI.GetDat().GetOutNId(EdgeN); }
    /// Returns ID of NodeN-th neighboring node.
    int GetNbrNId(const int& EdgeN) const { return NodeHI.GetDat().GetNbrNId(EdgeN); }
    /// Returns ID of NodeN-th neighboring node, for the direction specififed.
    int GetNbrNId(const int& EdgeN, const TEdgeDir& dir) const { return NodeHI.GetDat().GetNbrNId(EdgeN, dir); }
    /// Returns ID of EdgeN-th in-edge.
    int GetInEId(const int& EdgeN) const { return NodeHI.GetDat().GetInEId(EdgeN); }
    /// Returns ID of EdgeN-th out-edge.
    int GetOutEId(const int& EdgeN) const { return NodeHI.GetDat().GetOutEId(EdgeN); }
    /// Returns ID of EdgeN-th edge.
    int GetNbrEId(const int& EdgeN) const { return NodeHI.GetDat().GetNbrEId(EdgeN); }
    /// Returns ID of EdgeN-th edge, for the direction specified.
    int GetNbrEId(const int& EdgeN, const TEdgeDir& dir) const { return NodeHI.GetDat().GetNbrEId(EdgeN, dir); }
    
    /// Returns weight of EdgeN-th in-edge.
    TEdgeW GetInEW(const int& EdgeN) const { return NodeHI.GetDat().GetInEW(EdgeN); }
    /// Returns weight of EdgeN-th out-edge.
    TEdgeW GetOutEW(const int& EdgeN) const { return NodeHI.GetDat().GetOutEW(EdgeN); }
    /// Returns weight of EdgeN-th edge.
    TEdgeW GetNbrEW(const int& EdgeN) const { return NodeHI.GetDat().GetNbrEW(EdgeN); }
    /// Returns weight of EdgeN-th edge, for the direction specified.
    TEdgeW GetNbrEW(const int& EdgeN, const TEdgeDir& dir) const { return NodeHI.GetDat().GetNbrEW(EdgeN, dir); }
        
    /// Tests whether the current node has in edge with ID EId.
    bool IsInEId(const int& EId) const { return NodeHI.GetDat().IsInEId(EId); }
    /// Tests whether the current node has out edge with ID EId.
    bool IsOutEId(const int& EId) const { return NodeHI.GetDat().IsOutEId(EId); }
    /// Tests whether the current node has edge with ID EId
    bool IsNbrEId(const int& EId) const { return IsOutEId(EId) || IsInEId(EId); }
    
    /// Tests whether node with ID NId points to the current node.
    bool IsInNId(const int& NId) const { return NodeHI.GetDat().IsInNId(NId); }
    /// Tests whether the current node points to node with ID NId.
    bool IsOutNId(const int& NId) const { return NodeHI.GetDat().IsOutNId(NId); }
    /// Tests whether node with ID NId is a neighbor of the current node.
    bool IsNbrNId(const int& NId) const { return IsOutNId(NId) || IsInNId(NId); }
    
    friend class TWNEGraph<TEdgeW>;
  };
  /// Edge iterator. Only forward iteration (operator++) is supported.
  class TEdgeI {
  private:
    typedef typename THash<TInt, TEdge>::TIter THashIter;
    THashIter EdgeHI;
  public:
    TEdgeI() : EdgeHI() { }
    TEdgeI(const THashIter& EdgeHIter) : EdgeHI(EdgeHIter) { }
    TEdgeI(const TEdgeI& EdgeI) : EdgeHI(EdgeI.EdgeHI) { }
    TEdgeI& operator = (const TEdgeI& EdgeI) { if (this != &EdgeI) { EdgeHI = EdgeI.EdgeHI; } return *this; }
    /// Increment iterator.
    TEdgeI& operator++ (int) { EdgeHI++; return *this; }
    /// Decrement iterator.
    TEdgeI& operator-- (int) { EdgeHI--; return *this; }
    // Methods for ordering.
    bool operator == (const TEdgeI& EdgeI) const { return EdgeHI == EdgeI.EdgeHI; }
    bool operator != (const TEdgeI& EdgeI) const { return EdgeHI != EdgeI.EdgeHI; }
    bool operator < (const TEdgeI& EdgeI) const { return EdgeHI < EdgeI.EdgeHI; }
    bool operator > (const TEdgeI& EdgeI) const { return EdgeHI > EdgeI.EdgeHI; }
    /// Gets edge ID.
    int GetId() const { return EdgeHI.GetDat().GetId(); }
    /// Gets the source of an edge.
    int GetSrcNId() const { return EdgeHI.GetDat().GetSrcNId(); }
    /// Gets the destination of an edge.
    int GetDstNId() const { return EdgeHI.GetDat().GetDstNId(); }
    /// Gets the weight of an edge.
    TEdgeW GetW() const { return EdgeHI.GetDat().GetW(); }
    friend class TWNEGraph<TEdgeW>;
  };
private:
  // Get node methods (allows both const and non const return usage)
  TNode& GetNode(const int& NId) { return NodeH.GetDat(NId); }
  const TNode& GetNode(const int& NId) const { return NodeH.GetDat(NId); }
  TEdge& GetEdge(const int& EId) { return EdgeH.GetDat(EId); }
  const TEdge& GetEdge(const int& EId) const { return EdgeH.GetDat(EId); }
private:
  TCRef CRef;
  TInt MxNId, MxEId;
  THash<TInt, TNode> NodeH;
  THash<TInt, TEdge> EdgeH;
public:
  TWNEGraph() : CRef(), MxNId(0), MxEId(0), NodeH(), EdgeH() { }
  /// Constructor that reserves enough memory for a graph of Nodes nodes and Edges edges.
  explicit TWNEGraph(const int& Nodes, const int& Edges) : CRef(), MxNId(0), MxEId(0) { Reserve(Nodes, Edges); }
  TWNEGraph(const TWNEGraph& Graph) : MxNId(Graph.MxNId), NodeH(Graph.NodeH) { }
  /// Constructor that loads the graph from a (binary) stream SIn.
  TWNEGraph(TSIn& SIn) : MxNId(SIn), MxEId(SIn), NodeH(SIn), EdgeH(SIn) { }
  /// Saves the graph to a (binary) stream SOut.
  void Save(TSOut& SOut) const { MxNId.Save(SOut); MxEId.Save(SOut); NodeH.Save(SOut); EdgeH.Save(SOut); }
  /// Static constructor that returns a pointer to the graph. Call: PNGraph Graph = TWNEGraph::New().
  static PNet New() { return PNet(new TNet()); }
  /// Static constructor that returns a pointer to the graph and reserves enough memory for Nodes nodes and Edges edges.
  static PNet New(const int& Nodes, const int& Edges) { return PNet(new TNet(Nodes, Edges)); }
  /// Static constructor that loads the graph from a stream SIn and returns a pointer to it.
  static PNet Load(TSIn& SIn) { return PNet(new TNet(SIn)); }
  /// Allows for run-time checking the type of the graph (see the TGraphFlag for flags).
  bool HasFlag(const TGraphFlag& Flag) const {
    return HasGraphFlag(TWNEGraph::TNet, Flag);
  }
  TNet& operator = (const TNet& Graph) { if (this != &Graph) { MxNId = Graph.MxNId; MxEId = Graph.MxEId;  NodeH = Graph.NodeH; EdgeH = Graph.EdgeH; }  return *this; }
  
  /// Returns the number of nodes in the graph.
  int GetNodes() const { return NodeH.Len(); }
  /// Adds a node of ID NId to the graph.
  int AddNode(int NId = -1);
  /// Adds a node of ID NodeI.GetId() to the graph.
  int AddNode(const TNodeI& NodeId) { return AddNode(NodeId.GetId()); }
  /// Deletes node of ID NId from the graph.
  void DelNode(const int& NId);
  /// Deletes node of ID NodeI.GetId() from the graph.
  void DelNode(const TNode& NodeI) { DelNode(NodeI.GetId()); }
  /// Tests whether ID NId is a node.
  bool IsNode(const int& NId) const { return NodeH.IsKey(NId); }
  /// Returns an iterator referring to the first node in the graph.
  TNodeI BegNI() const { return TNodeI(NodeH.BegI()); }
  /// Returns an iterator referring to the past-the-end node in the graph.
  TNodeI EndNI() const { return TNodeI(NodeH.EndI()); }
  /// Returns an iterator referring to the node of ID NId in the graph.
  TNodeI GetNI(const int& NId) const { return TNodeI(NodeH.GetI(NId)); }
  /// Returns an ID that is larger than any node ID in the graph.
  int GetMxNId() const { return MxNId; }
  
  /// Returns the number of edges in the graph.
  int GetEdges() const { return EdgeH.Len(); }
  /// Adds an edge from node IDs SrcNId to node DstNId to the graph.
  int AddEdge(const int& SrcNId, const int& DstNId, TEdgeW W = 1, int EId = -1);
  /// Adds an edge from EdgeI.GetSrcNId() to EdgeI.GetDstNId() to the graph.
  int AddEdge(const TEdgeI& EdgeI) { return AddEdge(EdgeI.GetSrcNId(), EdgeI.GetDstNId(), EdgeI.GetW(), EdgeI.GetId()); }
  /// Deletes an edge with edge ID EId from the graph.
  void DelEdge(const int& EId);
  /// Deletes all edges from node IDs SrcNId to DstNId from the graph.
  void DelEdge(const int& SrcNId, const int& DstNId, const bool& IsDir = true);
  /// Deletes edge of ID EdgeI.GetId() from the graph.
  void DelEdge(TEdge& EdgeI) { DelEdge(EdgeI.GetId()); }
  /// Tests whether an edge with edge ID EId exists in the graph.
  bool IsEdge(const int& EId) const { return EdgeH.IsKey(EId); }
  
  // TODO: rename the following methods or allow for TVec<TInt> EIdV to be returned
  
  /// Tests whether an edge from node IDs SrcNId to DstNId exists in the graph.
  bool IsEdge(const int& SrcNId, const int& DstNId, const bool& IsDir = true) const { int EId; return IsEdge(SrcNId, DstNId, EId, IsDir); }
  /// Tests whether an edge between node IDs SrcNId and DstNId exists in the graph. if an edge exists, return its edge ID in EId.
  bool IsEdge(const int& SrcNId, const int& DstNId, int& EId, const bool& IsDir = true) const;
  /// Returns an edge ID between node IDs SrcNId and DstNId, if such an edge exists. Otherwise, return -1.
  int GetEId(const int& SrcNId, const int& DstNId) const { int EId; return IsEdge(SrcNId, DstNId, EId) ? EId : -1; }
  
  /// Returns an iterator referring to the first edge in the graph.
  TEdgeI BegEI() const { return TEdgeI(EdgeH.BegI()); }
  /// Returns an iterator referring to the past-the-end edge in the graph.
  TEdgeI EndEI() const { return TEdgeI(EdgeH.EndI()); }
  /// Returns an iterator referring to edge EId in the graph.
  TEdgeI GetEI(const int& EId) const { return TEdgeI(EdgeH.GetI(EId)); }
  /// Returns an iterator referring to edge (SrcNId, DstNId) in the graph.
  TEdgeI GetEI(const int& SrcNId, const int& DstNId) const { return GetEI(GetEId(SrcNId, DstNId)); }
  /// Returns an ID that is larger than any edge ID in the graph.
  int GetMxEId() const { return MxEId; }
  
  /// Sets weight for edge with edge ID EId in the graph.
  void SetEW(const int& EId, const TEdgeW& W);
  /// Returns the total weight in the graph.
  TEdgeW GetTotalW();
  /// Returns the maximum edge weight in the graph.
  TEdgeW GetMxW();
  
  /// Returns an ID of a random node in the graph.
  int GetRndNId(TRnd& Rnd = TInt::Rnd) { return NodeH.GetKey(NodeH.GetRndKeyId(Rnd, 0.8)); }
  /// Returns an interator referring to a random node in the graph.
  TNodeI GetRndNI(TRnd& Rnd = TInt::Rnd) { return GetNI(GetRndNId(Rnd)); }
  /// Returns an ID of a random edge in the graph.
  int GetRndEId(TRnd& Rnd = TInt::Rnd) { return EdgeH.GetKey(EdgeH.GetRndKeyId(Rnd, 0.8)); }
  /// Returns an interator referring to a random edge in the graph.
  TEdgeI GetRndEI(TRnd& Rnd = TInt::Rnd) { return GetEI(GetRndEId(Rnd)); }
  
  /// Gets a vector IDs of all nodes in the graph.
  void GetNIdV(TIntV& NIdV) const;
  /// Gets a vector IDs of all edges in the graph.
  void GetEIdV(TIntV& EIdV) const;

  /// Tests whether the graph is empty (has zero nodes).
  bool Empty() const { return GetNodes()==0; }
  /// Deletes all nodes and edges from the graph.
  void Clr() { MxNId=0; MxEId=0; NodeH.Clr(); EdgeH.Clr(); }
  /// Reserves memory for a graph of Nodes nodes and Edges edges.
  void Reserve(const int& Nodes, const int& Edges) { if (Nodes > 0) { NodeH.Gen(Nodes/2); } }
  /// Reserves memory for node ID NId having InDeg in-edges.
  void ReserveNIdInDeg(const int& NId, const int& InDeg) { GetNode(NId).InEdgeV.Reserve(InDeg); }
  /// Reserves memory for node ID NId having OutDeg out-edges.
  void ReserveNIdOutDeg(const int& NId, const int& OutDeg) { GetNode(NId).OutEdgeV.Reserve(OutDeg); }
  /// Defragments the graph. ##TWNEGraph::Defrag
  void Defrag(const bool& OnlyNodeLinks = false);
  /// Checks the graph data structure for internal consistency.
  bool IsOk(const bool& ThrowExcept = true) const;
  /// Print the graph in a human readable form to an output stream OutF.
  void Dump(FILE *OutF = stdout) const;
  /// Returns a small graph on 5 nodes and 6 edges.
  static PNet GetSmallGraph();
  friend class TPt<TWNEGraph<TEdgeW> >;
  // friend class TWNEGraphWMtx<TEdgeW>; // required for weighted adjacency matrix SVD
};

// set flags
namespace TSnap {
template <class TEdgeW> struct IsMultiGraph<TWNEGraph<TEdgeW> > { enum { Val = 1 }; };
template <class TEdgeW> struct IsDirected<TWNEGraph<TEdgeW> > { enum { Val = 1 }; };
}

// TNode methods implemented

// Effectively binary search on EId
template <class TEdgeW>
bool TWNEGraph<TEdgeW>::TNode::IsInEId(const int& EId, int& EdgeN) const {
  int LValN = 0, RValN = GetInDeg() - 1;
  while (RValN >= LValN) {
    EdgeN = (LValN + RValN) / 2;
    if (EId == InEdgeV[EdgeN].GetId()) { return true; }
    if (EId < InEdgeV[EdgeN].GetId()) {
      RValN = EdgeN - 1;
    } else {
      LValN = EdgeN + 1;
    }
  }
  EdgeN = -1;
  return false;
}

// Effectively binary search on EId 
template <class TEdgeW>
bool TWNEGraph<TEdgeW>::TNode::IsOutEId(const int& EId, int& EdgeN) const {
  int LValN = 0, RValN = GetOutDeg() - 1;
  while (RValN >= LValN) {
    EdgeN = (LValN + RValN) / 2;
    if (EId == OutEdgeV[EdgeN].GetId()) { return true; }
    if (EId < OutEdgeV[EdgeN].GetId()) {
      RValN = EdgeN - 1;
    } else {
      LValN = EdgeN + 1;
    }
  }
  EdgeN = -1;
  return false;
}

// Linear search on NId
template <class TEdgeW>
bool TWNEGraph<TEdgeW>::TNode::IsInNId(const int& NId) const {
  for (int edge = 0; edge < GetInDeg(); edge++) {
    if (NId == InEdgeV[edge].GetSrcNId()) {
      return true;
    }
  }
  return false;
}

// Effectively binary search on NId
template <class TEdgeW>
bool TWNEGraph<TEdgeW>::TNode::IsOutNId(const int& NId) const {
  for (int edge = 0; edge < GetOutDeg(); edge++) {
    if (NId == OutEdgeV[edge].GetDstNId()) {
      return true;
    }
  }
  return false;
}

// TNodeI methods implemented

template <class TEdgeW>
TEdgeW TWNEGraph<TEdgeW>::TNode::GetWInDeg() const {
  TEdgeW WDeg = 0;
  for (int edge = 0; edge < GetInDeg(); edge++) {
    WDeg += GetInEW(edge);
  }
  return WDeg;
}

template <class TEdgeW>
TEdgeW TWNEGraph<TEdgeW>::TNode::GetWOutDeg() const {
  TEdgeW WDeg = 0;
  for (int edge = 0; edge < GetOutDeg(); edge++) {
    WDeg += GetOutEW(edge);
  }
  return WDeg;
}

// TWNEGraph<TEdgeW> methods implemented

template <class TEdgeW>
int TWNEGraph<TEdgeW>::AddNode(int NId) {
  if (NId == -1) {
    NId = MxNId;  MxNId++;
  } else {
    IAssertR(!IsNode(NId), TStr::Fmt("NodeId %d already exists", NId));
    MxNId = TMath::Mx(NId+1, MxNId());
  }
  NodeH.AddDat(NId, TNode(NId));
  return NId;
}

template <class TEdgeW>
void TWNEGraph<TEdgeW>::DelNode(const int& NId) {
  const TNode& Node = GetNode(NId);
  for (int e = 0; e < Node.GetOutDeg(); e++) {
    const int EId = Node.GetOutEId(e);
    const TEdge& Edge = GetEdge(EId);
    IAssert(Edge.GetSrcNId() == NId);
    TNode& N = GetNode(Edge.GetDstNId());
    const int edge = N.InEdgeV.SearchBin(Edge);
    if (edge != -1) { N.InEdgeV.Del(edge); }
    EdgeH.DelKey(EId);
  }
  for (int e = 0; e < Node.GetInDeg(); e++) {
    const int EId = Node.GetInEId(e);
    const TEdge& Edge = GetEdge(EId);
    IAssert(Edge.GetDstNId() == NId);
    TNode& N = GetNode(Edge.GetSrcNId());
    const int edge = N.OutEdgeV.SearchBin(Edge);
    if (edge != -1) { N.OutEdgeV.Del(edge); }
    EdgeH.DelKey(EId);
  }
  NodeH.DelKey(NId);
}

template <class TEdgeW>
int TWNEGraph<TEdgeW>::AddEdge(const int& SrcNId, const int& DstNId, TEdgeW W, int EId) {
  IAssertR(!IsEdge(EId), TStr::Fmt("EdgeId %d already exists", EId));
  IAssertR(IsNode(SrcNId) && IsNode(DstNId), TStr::Fmt("%d or %d not a node.", SrcNId, DstNId).CStr());
  IAssertR(W > 0, "Weight not positive (stricly).");
  if (EId == -1) {
    EId = MxEId;  MxEId++;
  } else {
    MxEId = TMath::Mx(EId+1, MxEId());
  }
  TEdge Edge(EId, SrcNId, DstNId, W);
  EdgeH.AddDat(EId, Edge);
  GetNode(SrcNId).OutEdgeV.Add(Edge);
  GetNode(DstNId).InEdgeV.Add(Edge);
  return EId;
}

template <class TEdgeW>
void TWNEGraph<TEdgeW>::DelEdge(const int& EId) {
  IAssert(IsEdge(EId));
  const int SrcNId = GetEdge(EId).GetSrcNId();
  const int DstNId = GetEdge(EId).GetDstNId();
  const TEdge& Edge = GetEdge(EId);
  { TNode& N = GetNode(SrcNId);
  const int edge = N.OutEdgeV.SearchBin(Edge);
  if (edge != -1) { N.OutEdgeV.Del(edge); } }
  { TNode& N = GetNode(DstNId);
  const int edge = N.InEdgeV.SearchBin(Edge);
  if (edge != -1) { N.InEdgeV.Del(edge); }
  EdgeH.DelKey(EId); }
}

// Delete all edges between the two nodes
template <class TEdgeW>
void TWNEGraph<TEdgeW>::DelEdge(const int& SrcNId, const int& DstNId, const bool& IsDir) {
  int EId;
  IAssert(IsEdge(SrcNId, DstNId, EId, IsDir)); // there is at least one edge
  while (IsEdge(SrcNId, DstNId, EId, IsDir)) {
    const TEdge& Edge = GetEdge(EId);
    { TNode& N = GetNode(SrcNId);
    const int edge = N.OutEdgeV.SearchBin(Edge);
    if (edge != -1) { N.OutEdgeV.Del(edge); } }
    { TNode& N = GetNode(DstNId);
    const int edge = N.InEdgeV.SearchBin(Edge);
    if (edge != -1) { N.InEdgeV.Del(edge); } }
    EdgeH.DelKey(EId);
  }
}

template <class TEdgeW>
bool TWNEGraph<TEdgeW>::IsEdge(const int& SrcNId, const int& DstNId, int& EId, const bool& IsDir) const {
  const TNode& Node = GetNode(SrcNId);
  for (int edge = 0; edge < Node.GetOutDeg(); edge++) {
    const TEdge& Edge = Node.GetOutEdge(edge);
    if (DstNId == Edge.GetDstNId()) {
      EId = Edge.GetId();
      return true;
    }
  }
  if (! IsDir) {
    for (int edge = 0; edge < Node.GetInDeg(); edge++) {
      const TEdge& Edge = Node.GetInEdge(edge);
      if (DstNId == Edge.GetSrcNId()) {
        EId = Edge.GetId();
        return true;
      }
    }
  }
  return false;
}

template <class TEdgeW>
void TWNEGraph<TEdgeW>::SetEW(const int& EId, const TEdgeW& W) {
  IAssert(IsEdge(EId));
  const int SrcNId = GetEdge(EId).GetSrcNId();
  const int DstNId = GetEdge(EId).GetDstNId();
  const TEdge& Edge = GetEdge(EId);
  { TNode& N = GetNode(SrcNId);
  const int edge = N.OutEdgeV.SearchBin(Edge);
  if (edge != -1) { N.OutEdgeV[edge].SetW(W); } }
  { TNode& N = GetNode(DstNId);
  const int edge = N.InEdgeV.SearchBin(Edge);
  if (edge != -1) { N.InEdgeV[edge].SetW(W); }
  EdgeH.GetDat(EId).SetW(W); }
}

template <class TEdgeW>
TEdgeW TWNEGraph<TEdgeW>::GetTotalW() {
  TEdgeI EI;
  TEdgeW TotalW = 0;
  for (EI = BegEI(); EI < EndEI(); EI++) {
    TotalW += EI.GetW();
  }
  return TotalW;
}

template <class TEdgeW>
TEdgeW TWNEGraph<TEdgeW>::GetMxW() {
  TEdgeI EI;
  TEdgeW W, MxW = 0;
  for (EI = BegEI(); EI < EndEI(); EI++) {
    W = EI.GetW();
    if (W > MxW) { MxW = W; }
  }
  return MxW;
}

template <class TEdgeW>
void TWNEGraph<TEdgeW>::GetNIdV(TIntV& NIdV) const {
  NIdV.Gen(GetNodes(), 0);
  for (int N = NodeH.FFirstKeyId(); NodeH.FNextKeyId(N); ) {
    NIdV.Add(NodeH.GetKey(N));
  }
}

template <class TEdgeW>
void TWNEGraph<TEdgeW>::GetEIdV(TIntV& EIdV) const {
  EIdV.Gen(GetEdges(), 0);
  for (int E = EdgeH.FFirstKeyId(); EdgeH.FNextKeyId(E); ) {
    EIdV.Add(EdgeH.GetKey(E));
  }
}

template <class TEdgeW>
void TWNEGraph<TEdgeW>::Defrag(const bool& OnlyNodeLinks) {
  for (int KId = NodeH.FFirstKeyId(); NodeH.FNextKeyId(KId); ) {
    // TNode& Node = NodeH[KId];
    // Node.Pack();
    NodeH[KId].Pack();
  }
  if (! OnlyNodeLinks && ! NodeH.IsKeyIdEqKeyN()) { NodeH.Defrag(); }
  if (! OnlyNodeLinks && ! EdgeH.IsKeyIdEqKeyN()) { EdgeH.Defrag(); }
}

template <class TEdgeW>
bool TWNEGraph<TEdgeW>::IsOk(const bool& ThrowExcept) const {
  bool RetVal = true;
  for (int N = NodeH.FFirstKeyId(); NodeH.FNextKeyId(N); ) {
    const TNode& Node = NodeH[N];
    if (! Node.OutEdgeV.IsSorted()) {
      const TStr Msg = TStr::Fmt("Out-edge list of node %d is not sorted.", Node.GetId());
      if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
    }
    if (! Node.InEdgeV.IsSorted()) {
      const TStr Msg = TStr::Fmt("In-edge list of node %d is not sorted.", Node.GetId());
      if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
    }
    // Check out edge ID
    int prevEId = -1;
    for (int e = 0; e < Node.GetOutDeg(); e++) {
      if (! IsEdge(Node.GetOutEId(e))) {
        const TStr Msg = TStr::Fmt("Out-edge id %d of node %d does not exist.",  Node.GetOutEId(e), Node.GetId());
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      if (e > 0 && prevEId == Node.GetOutEId(e)) {
        const TStr Msg = TStr::Fmt("Node %d has duplidate out-edge id %d.", Node.GetId(), Node.GetOutEId(e));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      prevEId = Node.GetOutEId(e);
    }
    // Check in edge ID
    prevEId = -1;
    for (int e = 0; e < Node.GetInDeg(); e++) {
      if (! IsEdge(Node.GetInEId(e))) {
        const TStr Msg = TStr::Fmt("In-edge id %d of node %d does not exist.",  Node.GetInEId(e), Node.GetId());
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      if (e > 0 && prevEId == Node.GetInEId(e)) {
        const TStr Msg = TStr::Fmt("Node %d has duplidate out-edge id %d.", Node.GetId(), Node.GetInEId(e));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      prevEId = Node.GetInEId(e);
    }
    // Check out edge copy consistency
    for (int e = 0; e < Node.GetOutDeg(); e++) {
      if (Node.GetOutEW(e) != GetEdge(Node.GetOutEId(e)).GetW()) {
        const TStr Msg = TStr::Fmt("Out-edge weight %f of node %d is not consistency with master edge weight %f.", double(Node.GetOutEW(e)), Node.GetId(), double(GetEdge(Node.GetOutEId(e)).GetW()));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
    }
    // Check in edge copy consistency
    for (int e = 0; e < Node.GetInDeg(); e++) {
      if (Node.GetInEW(e) != GetEdge(Node.GetInEId(e)).GetW()) {
        const TStr Msg = TStr::Fmt("In-edge weight %f of node %d is not consistency with master edge weight %f.", double(Node.GetInEW(e)), Node.GetId(), double(GetEdge(Node.GetInEId(e)).GetW()));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
    }
  }
  for (int E = EdgeH.FFirstKeyId(); EdgeH.FNextKeyId(E); ) {
    const TEdge& Edge = EdgeH[E];
    // Check edge nodes
    if (! IsNode(Edge.GetSrcNId())) {
      const TStr Msg = TStr::Fmt("Edge %d source node %d does not exist.", Edge.GetId(), Edge.GetSrcNId());
      if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
    }
    if (! IsNode(Edge.GetDstNId())) {
      const TStr Msg = TStr::Fmt("Edge %d destination node %d does not exist.", Edge.GetId(), Edge.GetDstNId());
      if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
    }
  }
  return RetVal;
}

template <class TEdgeW>
void TWNEGraph<TEdgeW>::Dump(FILE *OutF) const {
  const int NodePlaces = (int) ceil(log10((double) GetNodes()));
  const int EdgePlaces = (int) ceil(log10((double) GetEdges()));
  const int WPlaces = (int) ceil(log10((double) GetMxW()));
  fprintf(OutF, "-------------------------------------------------\nDirected Node-Edge Graph: nodes: %d, edges: %d\n", GetNodes(), GetEdges());
  for (TNodeI NodeI = BegNI(); NodeI < EndNI(); NodeI++) {
    fprintf(OutF, "  %*d]\n", NodePlaces, NodeI.GetId());
    fprintf(OutF, "    in[%d]", NodeI.GetInDeg());
    for (int edge = 0; edge < NodeI.GetInDeg(); edge++) {
      fprintf(OutF, " %*d", EdgePlaces, NodeI.GetInEId(edge)); }
    fprintf(OutF, "\n    out[%d]", NodeI.GetOutDeg());
    for (int edge = 0; edge < NodeI.GetOutDeg(); edge++) {
      fprintf(OutF, " %*d", EdgePlaces, NodeI.GetOutEId(edge)); }
    fprintf(OutF, "\n");
  }
  for (TEdgeI EdgeI = BegEI(); EdgeI < EndEI(); EdgeI++) {
    fprintf(OutF, "  %*d]  %*d  ->  %*d  :  %*f)\n", EdgePlaces, EdgeI.GetId(), NodePlaces, EdgeI.GetSrcNId(), NodePlaces, EdgeI.GetDstNId(), WPlaces, double(EdgeI.GetW()));
  }
  fprintf(OutF, "\n");
}

#endif
