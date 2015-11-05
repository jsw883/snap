// Headers (?)

#include "stdafx.h"
#include "wgraph.h"

//#//////////////////////////////////////////////
/// Weighted directed graphs and multigraphs

template <class TEdgeW, template<class> class TGraph>
struct TypePair {
  typedef TEdgeW TypeEdgeW;
  typedef TGraph<TEdgeW> TypeGraph;
};

typedef ::testing::Types<TypePair<TInt, TWNGraph> > Graphs; // , TypePair<TFlt, TWNGraph>, TypePair<TInt, TWNEGraph>, TypePair<TFlt, TWNEGraph> > Graphs;

template <class TypePair>
class GraphTest : public ::testing::Test {
public:
  GraphTest() {}
};

TYPED_TEST_CASE(GraphTest, Graphs);

// Test default constructor
TYPED_TEST(GraphTest, Constructor) {
  
  // typedef typename TypeParam::TypeEdgeW TEdgeW;
  typedef typename TypeParam::TypeGraph TGraph;
  typedef TPt<TGraph> PGraph;
  
  PGraph Graph = TGraph::New();
  
  // empty
  EXPECT_EQ(0, Graph->GetNodes());
  EXPECT_EQ(0, Graph->GetEdges());

  // graph properties
  EXPECT_TRUE(Graph->Empty());
  EXPECT_TRUE(Graph->IsOk());
  EXPECT_TRUE(Graph->HasFlag(gfDirected));
  
}

// Test graph manipulation (create and delete nodes and edges)
TYPED_TEST(GraphTest, GeneralGraphFunctionality) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef typename TypeParam::TypeEdgeW TEdgeW;
  typedef typename TypeParam::TypeGraph TGraph;
  typedef TPt<TGraph> PGraph;

  typename TGraph::TNodeI NI;
  typename TGraph::TEdgeI EI;

  PGraph Graph = TGraph::New(), Clone = TGraph::New(), Copy;

  const char *FName = "graph.dat";
  
  int Nodes = 10000;
  int Edges = 1000000;

  int counter, NId;
  int SrcNId, DstNId;

  int InDeg, OutDeg, Deg;
  int TotalInDeg = 0, TotalOutDeg = 0, TotalDeg = 0;  
  TEdgeW WInDeg, WOutDeg, WDeg;
  TEdgeW TotalWInDeg = 0, TotalWOutDeg = 0, TotalWDeg = 0;  

  TEdgeW W, EdgeW;
  TEdgeW MxW = 100, TotalW = 0, NodeTotalW = 0;

  EXPECT_TRUE(Graph->Empty());

  // CREATE NODES AND EDGES

  // create nodes
  for (counter = 0; counter < Nodes; counter++) {
    Graph->AddNode(counter);
  }
  // graph properties
  EXPECT_FALSE(Graph->Empty());
  EXPECT_TRUE(Graph->IsOk());
  // count nodes
  EXPECT_EQ(Nodes, Graph->GetNodes());

  // create edges (unique)
  for (counter = 0; counter < Edges; ) {
    SrcNId = (long) (Nodes * drand48());
    DstNId = (long) (Nodes * drand48());
    W = (TEdgeW) (MxW * drand48() + 1);
    if (SrcNId != DstNId  &&  !Graph->IsEdge(SrcNId, DstNId)) {
      Graph->AddEdge(SrcNId, DstNId, W);
      counter++;
      // weights
      TotalW += W;
    }
  }
  // graph properties
  EXPECT_FALSE(Graph->Empty());
  EXPECT_TRUE(Graph->IsOk());
  // count edges
  EXPECT_EQ(Edges, Graph->GetEdges());
  // weights
  EXPECT_FLOAT_EQ(TotalW, Graph->GetTotalW());

  // NODE ITERATORS

  // nodes forward iterator
  counter = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    // check node existence
    EXPECT_TRUE(Graph->IsNode(NI.GetId()));
    // degrees
    InDeg = NI.GetInDeg(); TotalInDeg += InDeg;   
    OutDeg = NI.GetOutDeg(); TotalOutDeg += OutDeg;
    Deg = NI.GetDeg(); TotalDeg += Deg;
    EXPECT_EQ(Deg, InDeg + OutDeg);
    // weighted degrees
    WInDeg = NI.GetWInDeg(); TotalWInDeg += WInDeg;
    WOutDeg = NI.GetWOutDeg(); TotalWOutDeg += WOutDeg;
    WDeg = NI.GetWDeg(); TotalWDeg += WDeg;
    EXPECT_EQ(WDeg, WInDeg + WOutDeg);
    // count nodes
    counter++;
  }
  // degrees
  EXPECT_FLOAT_EQ(Edges, TotalInDeg);
  EXPECT_FLOAT_EQ(Edges, TotalOutDeg);
  EXPECT_FLOAT_EQ(Edges, TotalDeg/2);
  // weighted degrees
  EXPECT_FLOAT_EQ(TotalW, TotalWInDeg);
  EXPECT_FLOAT_EQ(TotalW, TotalWOutDeg);
  EXPECT_FLOAT_EQ(TotalW, TotalWDeg/2);
  // count nodes
  EXPECT_EQ(Nodes, counter);

  // nodes backward iterator
  counter = 0;
  for (NI = Graph->EndNI(); NI > Graph->BegNI(); ) {
    NI--;
    counter++;
    EXPECT_TRUE(Graph->IsNode(NI.GetId()));
  }
  EXPECT_EQ(Nodes, counter);

  // ASSIGNMENT, SAVE, AND LOAD

  *Clone = *Graph;

  // counts and weights
  EXPECT_EQ(Nodes, Clone->GetNodes());
  EXPECT_EQ(Edges, Clone->GetEdges());
  EXPECT_FLOAT_EQ(TotalW, Clone->GetTotalW());
  // graph properties  
  EXPECT_FALSE(Clone->Empty());
  EXPECT_TRUE(Clone->IsOk());

  // save
  {
    TFOut FOut(FName);
    Graph->Save(FOut);
    FOut.Flush();
  }
  // load
  {
    TFIn FIn(FName);
    Copy = TGraph::Load(FIn);
  }
  // counts and weights
  EXPECT_EQ(Nodes, Copy->GetNodes());
  EXPECT_EQ(Edges, Copy->GetEdges());
  EXPECT_FLOAT_EQ(TotalW, Copy->GetTotalW());
  // graph properties
  EXPECT_FALSE(Copy->Empty());
  EXPECT_TRUE(Copy->IsOk());

  // delete nodes and edges (randomly)
  for (counter = 0; counter < Nodes; counter++) {
    NId = Graph->GetRndNId();
    Graph->DelNode(NId);
  }

  // empty
  EXPECT_EQ(0, Graph->GetNodes());
  EXPECT_EQ(0, Graph->GetEdges());
  EXPECT_FLOAT_EQ(0, Graph->GetTotalW());
  // graph properties
  EXPECT_TRUE(Graph->Empty());
  EXPECT_TRUE(Graph->IsOk());

  // clear graph automatically
  Copy->Clr();

  // counts and weights
  EXPECT_EQ(0, Copy->GetNodes());
  EXPECT_EQ(0, Copy->GetEdges());
  EXPECT_FLOAT_EQ(0, Copy->GetTotalW());
  // graph properties
  EXPECT_TRUE(Copy->Empty());
  EXPECT_TRUE(Copy->IsOk());

}

//#//////////////////////////////////////////////
/// Weighted directed graphs

typedef ::testing::Types<TInt, TFlt> Weights;

template <class TEdgeW>
class TWNGraphTest : public ::testing::Test {
public:
  TWNGraphTest() {}
};

TYPED_TEST_CASE(TWNGraphTest, Weights);

// Test graph edge weight consistency
TYPED_TEST(TWNGraphTest, SpecificGraphFunctionality) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TEdgeW;
  typedef TPt<TWNGraph<TEdgeW> > PGraph;

  typename TWNGraph<TEdgeW>::TNodeI NI;
  typename TWNGraph<TEdgeW>::TEdgeI EI;

  PGraph Graph = TWNGraph<TEdgeW>::New();
  
  int Nodes = 10000;
  int Edges = 1000000;

  int counter;
  int SrcNId, DstNId;

  TEdgeW W, EdgeW;
  TEdgeW MxW = 100, TotalW = 0, EdgeTotalW = 0, NodeTotalW = 0;

  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED

  // create nodes
  for (counter = 0; counter < Nodes; counter++) {
    Graph->AddNode(counter);
  }
  EXPECT_TRUE(Graph->IsOk());

  // create edges (unique with random weights)
  for (counter = 0; counter < Edges; ) {
    SrcNId = (long) (Nodes * drand48());
    DstNId = (long) (Nodes * drand48());
    W = (TEdgeW) (MxW * drand48() + 1);
    if (SrcNId != DstNId  &&  !Graph->IsEdge(SrcNId, DstNId)) {
      // create edge
      Graph->AddEdge(SrcNId, DstNId, W);
      counter++;
      // weights
      TotalW += W;
      EXPECT_DOUBLE_EQ(W, Graph->GetEI(SrcNId, DstNId).GetW());
    }
  }

  // EDGE ITERATORS

  // edges forward iterator
  counter = 0;
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    // weights
    EdgeTotalW += EI.GetW();
    // edge existence
    EXPECT_TRUE(Graph->IsEdge(EI.GetSrcNId(), EI.GetDstNId()));
    // count edges
    counter++;
  }
  // count edges
  EXPECT_EQ(Edges, counter);
  // weights
  EXPECT_FLOAT_EQ(TotalW, EdgeTotalW);

  // edges backward iterator
  counter = 0;
  for (EI = Graph->EndEI(); EI > Graph->BegEI(); ) {
    EI--;
    // edge existence
    EXPECT_TRUE(Graph->IsEdge(EI.GetSrcNId(), EI.GetDstNId()));
    // count edges
    counter++;
  }
  // count edges
  EXPECT_EQ(Edges, counter);

  // edges (per node)
  counter = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    for (int e = 0; e < NI.GetOutDeg(); e++) {
      // weights
      NodeTotalW += NI.GetOutEW(e);
      // edge existence
      EXPECT_TRUE(Graph->IsEdge(NI.GetId(), NI.GetOutNId(e)));
      // count edges
      counter++;
    }
  }
  // count edges
  EXPECT_EQ(Edges, counter);
  // weights
  EXPECT_FLOAT_EQ(TotalW, NodeTotalW);

}

//#//////////////////////////////////////////////
/// Weighted directed multigraphs

typedef ::testing::Types<TInt, TFlt> Weights;

template <class TEdgeW>
class TWNEGraphTest : public ::testing::Test {
public:
  TWNEGraphTest() {}
};

TYPED_TEST_CASE(TWNEGraphTest, Weights);

// Test graph edge weight consistency
TYPED_TEST(TWNEGraphTest, SpecificGraphFunctionality) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TEdgeW;
  typedef TPt<TWNEGraph<TEdgeW> > PGraph;

  typename TWNEGraph<TEdgeW>::TNodeI NI;
  typename TWNEGraph<TEdgeW>::TEdgeI EI;

  PGraph Graph = TWNEGraph<TEdgeW>::New();
  
  int Nodes = 10000;
  int Edges = 1000000;
  
  int counter, EId;
  int SrcNId, DstNId;

  TEdgeW W, EdgeW;
  TEdgeW MxW = 100, TotalW = 0, EdgeTotalW = 0, NodeTotalW = 0;

  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED

  // create nodes
  for (counter = 0; counter < Nodes; counter++) {
    Graph->AddNode(counter);
  }
  EXPECT_TRUE(Graph->IsOk());

  // create edges (unique with random weights)
  for (counter = 0; counter < Edges; ) {
    SrcNId = (long) (Nodes * drand48());
    DstNId = (long) (Nodes * drand48());
    W = (TEdgeW) (MxW * drand48() + 1);
    if (SrcNId != DstNId  &&  !Graph->IsEdge(SrcNId, DstNId)) {
      // create edge
      EId = Graph->AddEdge(SrcNId, DstNId, W);
      counter++;
      // weights
      TotalW += W;
      EXPECT_DOUBLE_EQ(W, Graph->GetEI(EId).GetW());
    }
  }

  // EDGE ITERATORS

  // edges forward iterator
  counter = 0;
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    // weights
    EdgeTotalW += EI.GetW();
    // check edge existence
    EXPECT_TRUE(Graph->IsEdge(EI.GetId()));
    // count edges
    counter++;
  }
  // count edges
  EXPECT_EQ(Edges, counter);
  // weights
  EXPECT_FLOAT_EQ(TotalW, EdgeTotalW);

  // edges backward iterator
  counter = 0;
  for (EI = Graph->EndEI(); EI > Graph->BegEI(); ) {
    EI--;
    // edge existence
    EXPECT_TRUE(Graph->IsEdge(EI.GetId()));
    // count edges
    counter++;
  }
  // count edges
  EXPECT_EQ(Edges, counter);

  // edges (per node)
  counter = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    for (int e = 0; e < NI.GetOutDeg(); e++) {
      // weights
      NodeTotalW += NI.GetOutEW(e);
      // check edge existence
      EXPECT_TRUE(Graph->IsEdge(NI.GetOutEId(e)));
      // count edges
      counter++;
    }
  }
  // count edges
  EXPECT_EQ(Edges, counter);
  // weights
  EXPECT_FLOAT_EQ(TotalW, NodeTotalW);

}

  // // create edges
  // for (counter = 0; counter < Edges; counter++) {
  //   SrcNId = (long) (Nodes * drand48());
  //   DstNId = (long) (Nodes * drand48());
  //   while (SrcNId == DstNId) {
  //     DstNId = (long) ( Nodes * drand48());
  //   }
  //   Graph->AddEdge(SrcNId, DstNId);
  // }
