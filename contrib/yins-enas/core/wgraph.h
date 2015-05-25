//#//////////////////////////////////////////////
/// Weighted graphs

#ifndef core_wgraph
#define core_wgraph

// Headers (?)

#include "Snap.h"

// TODO: Write a load method for weighted directed graphs

// NOTE: TWNEGraph is based on TNEGraph to decentralise edge weight data, and
// allow future flexibility, but adding edges is restricted so multiple edges
// are disallowed.

// NOTE: In order to define smart pointers, TEdgeW needs to be defined (C++98)

template<class TEdgeW> class TWNEGraph;

/// Integer weighted directed graph (TWNEGraph<TInt>)
typedef TWNEGraph<TInt> TIntWNEGraph;
/// Float weighted directed graph (TWNEGraph<TFlt>)
typedef TWNEGraph<TFlt> TFltWNEGraph;

/// Pointer to an integer weighted directed graph (TFltWNEGraph)
typedef TPt<TIntWNEGraph> PIntWNEGraph;
/// Pointer to a float weighted directed graph (TIntWNEGraph)
typedef TPt<TFltWNEGraph> PFltWNEGraph;

/// Template class for weighted directed graphs.  ##TWNEGraph::Class
template <class TEdgeW> // TEdgeW can be TInt or TFlt (should this be restricted (?))
class TWNEGraph {
public:
  typedef TWNEGraph<TEdgeW> TNet;
  typedef TPt<TNet> PNet;
public:
  class TNode {
  private:
    TInt Id;
    TIntV InEIdV, OutEIdV;
  public:
    TNode() : Id(-1), InEIdV(), OutEIdV() { }
    TNode(const int& NId) : Id(NId), InEIdV(), OutEIdV() { }
    TNode(const TNode& Node) : Id(Node.Id), InEIdV(Node.InEIdV), OutEIdV(Node.OutEIdV) { }
    TNode(TSIn& SIn) : Id(SIn), InEIdV(SIn), OutEIdV(SIn) { }
    void Save(TSOut& SOut) const { Id.Save(SOut); InEIdV.Save(SOut); OutEIdV.Save(SOut); }
    int GetId() const { return Id; }
    int GetDeg() const { return GetInDeg() + GetOutDeg(); }
    int GetInDeg() const { return InEIdV.Len(); }
    int GetOutDeg() const { return OutEIdV.Len(); }
    int GetInEId(const int& EdgeN) const { return InEIdV[EdgeN]; }
    int GetOutEId(const int& EdgeN) const { return OutEIdV[EdgeN]; }
    int GetNbrEId(const int& EdgeN) const { return EdgeN<GetOutDeg()?GetOutEId(EdgeN):GetInEId(EdgeN-GetOutDeg()); }
    bool IsInEId(const int& EId) const { return InEIdV.SearchBin(EId) != -1; }
    bool IsOutEId(const int& EId) const { return OutEIdV.SearchBin(EId) != -1; }
    friend class TWNEGraph<TEdgeW>;
  };
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
    int GetId() const { return Id; }
    int GetSrcNId() const { return SrcNId; }
    int GetDstNId() const { return DstNId; }
    TEdgeW GetW() const { return EdgeW; } // don't need to use references, only TInt or TFlt being used
    friend class TWNEGraph<TEdgeW>;
  };
  /// Node iterator. Only forward iteration (operator++) is supported.
  class TNodeI {
  private:
    typedef typename THash<TInt, TNode>::TIter THashIter;
    THashIter NodeHI;
    const TWNEGraph *Graph;
  public:
    TNodeI() : NodeHI(), Graph(NULL) { }
    TNodeI(const THashIter& NodeHIter, const TWNEGraph* GraphPt) : NodeHI(NodeHIter), Graph(GraphPt) { }
    TNodeI(const TNodeI& NodeI) : NodeHI(NodeI.NodeHI), Graph(NodeI.Graph) { }
    TNodeI& operator = (const TNodeI& NodeI) { NodeHI = NodeI.NodeHI; Graph=NodeI.Graph; return *this; }
    /// Increment iterator.
    TNodeI& operator++ (int) { NodeHI++; return *this; }
    bool operator < (const TNodeI& NodeI) const { return NodeHI < NodeI.NodeHI; }
    bool operator == (const TNodeI& NodeI) const { return NodeHI == NodeI.NodeHI; }
    /// Returns ID of the current node.
    int GetId() const { return NodeHI.GetDat().GetId(); }
    /// Returns degree of the current node, the sum of in-degree and out-degree.
    int GetDeg() const { return NodeHI.GetDat().GetDeg(); }
    /// Returns in-degree of the current node.
    int GetInDeg() const { return NodeHI.GetDat().GetInDeg(); }
    /// Returns out-degree of the current node.
    int GetOutDeg() const { return NodeHI.GetDat().GetOutDeg(); }
    /// Returns weighted degree of the current node, the sum of weighted in-degree and weighted out-degree.
    TEdgeW GetWDeg() const { return GetWInDeg() + GetWOutDeg(); }
    /// Returns weighted in-degree of the current node.
    TEdgeW GetWInDeg() const;
    /// Returns weighted out-degree of the current node.
    TEdgeW GetWOutDeg() const;
    /// Returns ID of EdgeN-th in-node (the node pointing to the current node).
    int GetInNId(const int& EdgeN) const { return Graph->GetEdge(NodeHI.GetDat().GetInEId(EdgeN)).GetSrcNId(); }
    /// Returns ID of EdgeN-th out-node (the node the current node points to).
    int GetOutNId(const int& EdgeN) const { return Graph->GetEdge(NodeHI.GetDat().GetOutEId(EdgeN)).GetDstNId(); }
    /// Returns ID of EdgeN-th neighboring node.
    int GetNbrNId(const int& EdgeN) const { const TEdge& E = Graph->GetEdge(NodeHI.GetDat().GetNbrEId(EdgeN));
      return GetId()==E.GetSrcNId() ? E.GetDstNId():E.GetSrcNId(); }
    /// Tests whether node with ID NId points to the current node.
    bool IsInNId(const int& NId) const { int EId; return IsInNId(NId, EId); }
    /// Tests whether node with ID NId points to the current node. If it does, returns the edge ID in EId
    bool IsInNId(const int& NId, int& EId) const;
    /// Tests whether the current node points to node with ID NId.
    bool IsOutNId(const int& NId) const { int EId; return IsOutNId(NId, EId); }
    /// Tests whether the current node points to node with ID NId. If it does, returns the edge ID in EId
    bool IsOutNId(const int& NId, int& EId) const;
    /// Tests whether node with ID NId is a neighbor of the current node.
    bool IsNbrNId(const int& NId) const { return IsOutNId(NId) || IsInNId(NId); }
    /// Tests whether node with ID NId is a neighbor of the current node. If it does, returns the edge ID in EId
    bool IsNbrNId(const int& NId, int& EId) const { return IsOutNId(NId, EId) || IsInNId(NId, EId); }
    /// Returns ID of EdgeN-th in-edge.
    int GetInEId(const int& EdgeN) const { return NodeHI.GetDat().GetInEId(EdgeN); }
    /// Returns ID of EdgeN-th out-edge.
    int GetOutEId(const int& EdgeN) const { return NodeHI.GetDat().GetOutEId(EdgeN); }
    /// Returns ID of EdgeN-th edge.
    int GetNbrEId(const int& EdgeN) const { return NodeHI.GetDat().GetNbrEId(EdgeN); }
    /// Tests whether the edge with ID EId is an in-edge of current node.
    bool IsInEId(const int& EId) const { return NodeHI.GetDat().IsInEId(EId); }
    /// Tests whether the edge with ID EId is an out-edge of current node.
    bool IsOutEId(const int& EId) const { return NodeHI.GetDat().IsOutEId(EId); }
    /// Tests whether the edge with ID EId is an in or out-edge of current node.
    bool IsNbrEId(const int& EId) const { return IsInEId(EId) || IsOutEId(EId); }
    /// Returns weight of EdgeN-th in-edge.
    TEdgeW GetInEW(const int& EdgeN) const { return Graph->GetEdge(NodeHI.GetDat().GetInEId(EdgeN)).GetW(); }
    /// Returns weight of EdgeN-th out-edge.
    TEdgeW GetOutEW(const int& EdgeN) const { return Graph->GetEdge(NodeHI.GetDat().GetOutEId(EdgeN)).GetW(); }
    /// Returns weight of EdgeN-th edge.
    TEdgeW GetNbrEW(const int& EdgeN) const { return Graph->GetEdge(NodeHI.GetDat().GetNbrEId(EdgeN)).GetW(); }
    friend class TWNEGraph<TEdgeW>;
  };
  /// Edge iterator. Only forward iteration (operator++) is supported.
  class TEdgeI {
  private:
    typedef typename THash<TInt, TEdge>::TIter THashIter;
    THashIter EdgeHI;
    const TWNEGraph *Graph;
  public:
    TEdgeI() : EdgeHI(), Graph(NULL) { }
    TEdgeI(const THashIter& EdgeHIter, const TWNEGraph *GraphPt) : EdgeHI(EdgeHIter), Graph(GraphPt) { }
    TEdgeI(const TEdgeI& EdgeI) : EdgeHI(EdgeI.EdgeHI), Graph(EdgeI.Graph) { }
    TEdgeI& operator = (const TEdgeI& EdgeI) { if (this!=&EdgeI) { EdgeHI=EdgeI.EdgeHI; Graph=EdgeI.Graph; }  return *this; }
    /// Increment iterator.
    TEdgeI& operator++ (int) { EdgeHI++; return *this; }
    bool operator < (const TEdgeI& EdgeI) const { return EdgeHI < EdgeI.EdgeHI; }
    bool operator == (const TEdgeI& EdgeI) const { return EdgeHI == EdgeI.EdgeHI; }
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
  TWNEGraph() : CRef(), MxNId(0), MxEId(0) { }
  /// Constructor that reserves enough memory for a graph of Nodes nodes and Edges edges.
  explicit TWNEGraph(const int& Nodes, const int& Edges) : CRef(), MxNId(0), MxEId(0) { Reserve(Nodes, Edges); }
  TWNEGraph(const TWNEGraph& Graph) : MxNId(Graph.MxNId), MxEId(Graph.MxEId), NodeH(Graph.NodeH), EdgeH(Graph.EdgeH) { }
  /// Constructor for loading the graph from a (binary) stream SIn.
  TWNEGraph(TSIn& SIn) : MxNId(SIn), MxEId(SIn), NodeH(SIn), EdgeH(SIn) { }
  /// Saves the graph to a (binary) stream SOut.
  void Save(TSOut& SOut) const { MxNId.Save(SOut); MxEId.Save(SOut); NodeH.Save(SOut); EdgeH.Save(SOut); }
  /// Static constructor that returns a pointer to the graph. Call: PNEGraph Graph = TWNEGraph<TEdgeW>::New().
  static PNet New() { return PNet(new TNet()); }
  /// Static constructor that returns a pointer to the graph and reserves enough memory for Nodes nodes and Edges edges.
  static PNet New(const int& Nodes, const int& Edges) { return PNet(new TNet(Nodes, Edges)); }
  /// Static constructor that loads the graph from a stream SIn and returns a pointer to it.
  static PNet Load(TSIn& SIn) { return PNet(new TNet(SIn)); }
  /// Allows for run-time checking the type of the graph (see the TGraphFlag for flags).
  bool HasFlag(const TGraphFlag& Flag) const;
  TWNEGraph& operator = (const TWNEGraph& Graph) { if (this!=&Graph) {
    MxNId=Graph.MxNId; MxEId=Graph.MxEId; NodeH=Graph.NodeH; EdgeH=Graph.EdgeH; }  return *this; }

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
  TNodeI BegNI() const { return TNodeI(NodeH.BegI(), this); }
  /// Returns an iterator referring to the past-the-end node in the graph.
  TNodeI EndNI() const { return TNodeI(NodeH.EndI(), this); }
  /// Returns an iterator referring to the node of ID NId in the graph.
  TNodeI GetNI(const int& NId) const { return TNodeI(NodeH.GetI(NId), this); }
  /// Returns an ID that is larger than any node ID in the graph.
  int GetMxNId() const { return MxNId; }

  /// Returns the number of edges in the graph.
  int GetEdges() const { return EdgeH.Len(); }
  /// Adds an edge with ID EId between node IDs SrcNId and DstNId to the graph.
  int AddEdge(const int& SrcNId, const int& DstNId, TEdgeW W = 1, int EId = -1);
  /// Adds an edge between EdgeI.GetSrcNId() and EdgeI.GetDstNId() to the graph.
  int AddEdge(const TEdgeI& EdgeI) { return AddEdge(EdgeI.GetSrcNId(), EdgeI.GetDstNId(), EdgeI.GetW(), EdgeI.GetId()); }
  /// Deletes an edge with edge ID EId from the graph.
  void DelEdge(const int& EId);
  /// Deletes all edges between node IDs SrcNId and DstNId from the graph.
  void DelEdge(const int& SrcNId, const int& DstNId, const bool& IsDir = true);
  /// Tests whether an edge with edge ID EId exists in the graph.
  bool IsEdge(const int& EId) const { return EdgeH.IsKey(EId); }
  /// Tests whether an edge between node IDs SrcNId and DstNId exists in the graph.
  bool IsEdge(const int& SrcNId, const int& DstNId, const bool& IsDir = true) const { int EId; return IsEdge(SrcNId, DstNId, EId, IsDir); }
  /// Tests whether an edge between node IDs SrcNId and DstNId exists in the graph. if an edge exists, return its edge ID in EId
  bool IsEdge(const int& SrcNId, const int& DstNId, int& EId, const bool& IsDir = true) const;
  /// Returns an edge ID between node IDs SrcNId and DstNId, if such an edge exists. Otherwise, return -1.
  int GetEId(const int& SrcNId, const int& DstNId) const { int EId; return IsEdge(SrcNId, DstNId, EId) ? EId : -1; }
  /// Returns an iterator referring to the first edge in the graph.
  TEdgeI BegEI() const { return TEdgeI(EdgeH.BegI(), this); }
  /// Returns an iterator referring to the past-the-end edge in the graph.
  TEdgeI EndEI() const { return TEdgeI(EdgeH.EndI(), this); }
  /// Returns an iterator referring to edge with edge ID EId. 
  TEdgeI GetEI(const int& EId) const { return TEdgeI(EdgeH.GetI(EId), this); }
  /// Returns an iterator referring to edge (SrcNId, DstNId) in the graph. 
  TEdgeI GetEI(const int& SrcNId, const int& DstNId) const { return GetEI(GetEId(SrcNId, DstNId)); }
  
  /// Returns the total weight in the graph.
  TEdgeW GetTotalW();
  
  /// Returns an ID of a random node in the graph.
  int GetRndNId(TRnd& Rnd=TInt::Rnd) { return NodeH.GetKey(NodeH.GetRndKeyId(Rnd, 0.8)); }
  /// Returns an interator referring to a random node in the graph.
  TNodeI GetRndNI(TRnd& Rnd=TInt::Rnd) { return GetNI(GetRndNId(Rnd)); }
  /// Returns an ID of a random edge in the graph.
  int GetRndEId(TRnd& Rnd=TInt::Rnd) { return EdgeH.GetKey(EdgeH.GetRndKeyId(Rnd, 0.8)); }
  /// Returns an interator referring to a random edge in the graph.
  TEdgeI GetRndEI(TRnd& Rnd=TInt::Rnd) { return GetEI(GetRndEId(Rnd)); }
  /// Gets a vector IDs of all nodes in the graph.
  void GetNIdV(TIntV& NIdV) const;
  /// Gets a vector IDs of all edges in the graph.
  void GetEIdV(TIntV& EIdV) const;

  /// Tests whether the graph is empty (has zero nodes).
  bool Empty() const { return GetNodes()==0; }
  /// Deletes all nodes and edges from the graph.
  void Clr() { MxNId=0; MxEId=0; NodeH.Clr(); EdgeH.Clr(); }
  /// Reserves memory for a graph of Nodes nodes and Edges edges.
  void Reserve(const int& Nodes, const int& Edges) {
    if (Nodes>0) { NodeH.Gen(Nodes/2); } if (Edges>0) { EdgeH.Gen(Edges/2); } }
  /// Defragments the graph.
  void Defrag(const bool& OnlyNodeLinks=false);
  /// Checks the graph data structure for internal consistency.
  bool IsOk(const bool& ThrowExcept=true) const;
  /// Print the graph in a human readable form to an output stream OutF.
  void Dump(FILE *OutF=stdout) const;
  /// Returns a small multigraph on 5 nodes and 6 edges.
  static PNet GetSmallGraph();
  friend class TPt<TWNEGraph<TEdgeW> >;
};

// Templated (requires template alias / C++11)
// template <class TEdgeW>
// using PWNEGraph = TPt<TWNEGraph<TEdgeW> >;

// set flags
namespace TSnap {
template <class TEdgeW> struct IsMultiGraph<TWNEGraph<TEdgeW> > { enum { Val = 1 }; };
template <class TEdgeW> struct IsDirected<TWNEGraph<TEdgeW> > { enum { Val = 1 }; };
}

// TNodeI methods implemented

template <class TEdgeW>
TEdgeW TWNEGraph<TEdgeW>::TNodeI::GetWInDeg() const {
  int edge, deg = NodeHI.GetDat().GetInDeg();
  TEdgeW WDeg = 0;
  for (edge = 0; edge < deg; edge++) {
    WDeg += GetInEW(edge); // Graph->GetEdge(NodeHI.GetDat().GetInEId(edge)).GetW();
  }
  return WDeg;
}

template <class TEdgeW>
TEdgeW TWNEGraph<TEdgeW>::TNodeI::GetWOutDeg() const {
  int edge, deg = NodeHI.GetDat().GetOutDeg();
  TEdgeW WDeg = 0;
  for (edge = 0; edge < deg; edge++) {
    WDeg += GetOutEW(edge); // Graph->GetEdge(NodeHI.GetDat().GetOutEId(edge)).GetW();
  }
  return WDeg;
}

// TWNEGraph<TEdgeW> methods implemented

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
bool TWNEGraph<TEdgeW>::HasFlag(const TGraphFlag& Flag) const {
  return HasGraphFlag(TWNEGraph::TNet, Flag);
}

template <class TEdgeW>
bool TWNEGraph<TEdgeW>::TNodeI::IsInNId(const int& NId, int& EId) const {
  const TNode& Node = NodeHI.GetDat();
  for (int edge = 0; edge < Node.GetInDeg(); edge++) {
    EId = Node.GetInEId(edge);
    if (NId == Graph->GetEdge(EId).GetSrcNId())
      return true;
  }
  EId = -1;
  return false;
}

template <class TEdgeW>
bool TWNEGraph<TEdgeW>::TNodeI::IsOutNId(const int& NId, int& EId) const {
  const TNode& Node = NodeHI.GetDat();
  for (int edge = 0; edge < Node.GetOutDeg(); edge++) {
    EId = Node.GetOutEId(edge);
    if (NId == Graph->GetEdge(EId).GetDstNId())
      return true;
  }
  EId = -1;
  return false;
}

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
  for (int out = 0; out < Node.GetOutDeg(); out++) {
    const int EId = Node.GetOutEId(out);
    const TEdge& Edge = GetEdge(EId);
    IAssert(Edge.GetSrcNId() == NId);
    GetNode(Edge.GetDstNId()).InEIdV.DelIfIn(EId);
    EdgeH.DelKey(EId);
  }
  for (int in = 0; in < Node.GetInDeg(); in++) {
    const int EId = Node.GetInEId(in);
    const TEdge& Edge = GetEdge(EId);
    IAssert(Edge.GetDstNId() == NId);
    GetNode(Edge.GetSrcNId()).OutEIdV.DelIfIn(EId);
    EdgeH.DelKey(EId);
  }
  NodeH.DelKey(NId);
}

// Now modified to restrict to only having one edge from unique SrcNId to DstNId,
// where adding another edge will result in having the weights of the existing
// and the new edge summed (except when an explicit EId is provided, this will
// still result in an error message).
template <class TEdgeW>
int TWNEGraph<TEdgeW>::AddEdge(const int& SrcNId, const int& DstNId, TEdgeW W, int EId) {
  IAssertR(IsNode(SrcNId) && IsNode(DstNId), TStr::Fmt("%d or %d not a node.", SrcNId, DstNId).CStr());
  IAssertR(W > 0, "Weight not positive (stricly).");
  int ExistingEId = -1; // does this need to be initialised here (?)
  if (IsEdge(SrcNId, DstNId, ExistingEId)) {
    GetEdge(ExistingEId).EdgeW += W;
    return ExistingEId;
  } else {
    IAssertR(!IsEdge(EId), TStr::Fmt("EdgeId %d already exists", EId));
    if (EId == -1) {
      EId = MxEId;  MxEId++;
    } else {
      MxEId = TMath::Mx(EId+1, MxEId());
    }
    EdgeH.AddDat(EId, TEdge(EId, SrcNId, DstNId, W));
    GetNode(SrcNId).OutEIdV.AddSorted(EId);
    GetNode(DstNId).InEIdV.AddSorted(EId);
    return EId;
  }
}

template <class TEdgeW>
void TWNEGraph<TEdgeW>::DelEdge(const int& EId) {
  IAssert(IsEdge(EId));
  const int SrcNId = GetEdge(EId).GetSrcNId();
  const int DstNId = GetEdge(EId).GetDstNId();
  GetNode(SrcNId).OutEIdV.DelIfIn(EId);
  GetNode(DstNId).InEIdV.DelIfIn(EId);
  EdgeH.DelKey(EId);
}

// delete all edges between the two nodes
template <class TEdgeW>
void TWNEGraph<TEdgeW>::DelEdge(const int& SrcNId, const int& DstNId, const bool& IsDir) {
  int EId;
  IAssert(IsEdge(SrcNId, DstNId, EId, IsDir)); // there is at least one edge
  while (IsEdge(SrcNId, DstNId, EId, IsDir)) {
    GetNode(SrcNId).OutEIdV.DelIfIn(EId);
    GetNode(DstNId).InEIdV.DelIfIn(EId);
  }
  EdgeH.DelKey(EId);
}

template <class TEdgeW>
bool TWNEGraph<TEdgeW>::IsEdge(const int& SrcNId, const int& DstNId, int& EId, const bool& IsDir) const {
  const TNode& SrcNode = GetNode(SrcNId);
  for (int edge = 0; edge < SrcNode.GetOutDeg(); edge++) {
    const TEdge& Edge = GetEdge(SrcNode.GetOutEId(edge));
    if (DstNId == Edge.GetDstNId()) {
      EId = Edge.GetId();  return true; }
  }
  if (! IsDir) {
    for (int edge = 0; edge < SrcNode.GetInDeg(); edge++) {
    const TEdge& Edge = GetEdge(SrcNode.GetInEId(edge));
    if (DstNId == Edge.GetSrcNId()) {
      EId = Edge.GetId();  return true; }
    }
  }
  return false;
}

template <class TEdgeW>
void TWNEGraph<TEdgeW>::GetNIdV(TIntV& NIdV) const {
  NIdV.Gen(GetNodes(), 0);
  for (int N=NodeH.FFirstKeyId(); NodeH.FNextKeyId(N); ) {
    NIdV.Add(NodeH.GetKey(N)); }
}

template <class TEdgeW>
void TWNEGraph<TEdgeW>::GetEIdV(TIntV& EIdV) const {
  EIdV.Gen(GetEdges(), 0);
  for (int E=EdgeH.FFirstKeyId(); EdgeH.FNextKeyId(E); ) {
    EIdV.Add(EdgeH.GetKey(E));
  }
}

template <class TEdgeW>
void TWNEGraph<TEdgeW>::Defrag(const bool& OnlyNodeLinks) {
  for (int kid = NodeH.FFirstKeyId(); NodeH.FNextKeyId(kid); ) {
    TNode& Node = NodeH[kid];
    Node.InEIdV.Pack();  Node.OutEIdV.Pack();
  }
  if (! OnlyNodeLinks && ! NodeH.IsKeyIdEqKeyN()) { NodeH.Defrag(); }
  if (! OnlyNodeLinks && ! EdgeH.IsKeyIdEqKeyN()) { EdgeH.Defrag(); }
}

template <class TEdgeW>
bool TWNEGraph<TEdgeW>::IsOk(const bool& ThrowExcept) const {
  bool RetVal = true;
  for (int N = NodeH.FFirstKeyId(); NodeH.FNextKeyId(N); ) {
    const TNode& Node = NodeH[N];
    if (! Node.OutEIdV.IsSorted()) {
      const TStr Msg = TStr::Fmt("Out-edge list of node %d is not sorted.", Node.GetId());
      if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
    }
    if (! Node.InEIdV.IsSorted()) {
      const TStr Msg = TStr::Fmt("In-edge list of node %d is not sorted.", Node.GetId());
      if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
    }
    // check out-edge ids
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
    // check in-edge ids
    prevEId = -1;
    for (int e = 0; e < Node.GetInDeg(); e++) {
      if (! IsEdge(Node.GetInEId(e))) {
        const TStr Msg = TStr::Fmt("Out-edge id %d of node %d does not exist.",  Node.GetInEId(e), Node.GetId());
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      if (e > 0 && prevEId == Node.GetInEId(e)) {
        const TStr Msg = TStr::Fmt("Node %d has duplidate out-edge id %d.", Node.GetId(), Node.GetInEId(e));
        if (ThrowExcept) { EAssertR(false, Msg); } else { ErrNotify(Msg.CStr()); } RetVal=false;
      }
      prevEId = Node.GetInEId(e);
    }
  }
  for (int E = EdgeH.FFirstKeyId(); EdgeH.FNextKeyId(E); ) {
    const TEdge& Edge = EdgeH[E];
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
    fprintf(OutF, "  %*d]  %*d  ->  %*d\n", EdgePlaces, EdgeI.GetId(), NodePlaces, EdgeI.GetSrcNId(), NodePlaces, EdgeI.GetDstNId());
  }
  fprintf(OutF, "\n");
}

// Return a small graph on 5 nodes and 6 edges (requires template alias / C++11)
// template <class TEdgeW>
// PNWEGraph<TEdgeW> TWNEGraph<TEdgeW>::GetSmallGraph() {
//   PNWEGraph<TEdgeW> Graph = TWNEGraph<TEdgeW>::New();
//   for (int i = 0; i < 5; i++) { Graph->AddNode(i); }
//   Graph->AddEdge(0,1);  Graph->AddEdge(0,2);
//   Graph->AddEdge(0,3);  Graph->AddEdge(0,4);
//   Graph->AddEdge(1,2);  Graph->AddEdge(1,2);
//   return Graph;
// }

#endif
