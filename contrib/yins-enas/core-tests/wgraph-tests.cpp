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

typedef ::testing::Types<TypePair<TInt, TWNGraph>, TypePair<TFlt, TWNGraph>, TypePair<TInt, TWNEGraph>, TypePair<TFlt, TWNEGraph> > Graphs;

template <class TypePair>
class GraphTest : public ::testing::Test {
public:
  GraphTest() {}
};

TYPED_TEST_CASE(GraphTest, Graphs);

// Test default constructor
TYPED_TEST(GraphTest, DefaultConstructor) {
  
  // typedef typename TypeParam::TypeEdgeW TEdgeW;
  typedef typename TypeParam::TypeGraph TGraph;
  typedef TPt<TGraph> PGraph;
  
  PGraph Graph = TGraph::New();
  
  EXPECT_EQ(0, Graph->GetNodes());
  EXPECT_EQ(0, Graph->GetEdges());

  EXPECT_EQ(1, Graph->Empty());
  EXPECT_EQ(1, Graph->IsOk());
  EXPECT_EQ(1, Graph->HasFlag(gfDirected));
  
  EXPECT_EQ(0, Graph->GetNodes());
  EXPECT_EQ(0, Graph->GetEdges());

  EXPECT_EQ(1, Graph->Empty());
  EXPECT_EQ(1, Graph->IsOk());
  EXPECT_EQ(1, Graph->HasFlag(gfDirected));

}

// Test graph manipulation (create and delete nodes and edges)
TYPED_TEST(GraphTest, GeneralGraphFunctionality) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef typename TypeParam::TypeEdgeW TEdgeW;
  typedef typename TypeParam::TypeGraph TGraph;
  typedef TPt<TGraph> PGraph;

  typename TGraph::TNodeI NI;
  typename TGraph::TEdgeI EI;

  PGraph Graph = TGraph::New();
  
  int Nodes = 10000;
  int Edges = 1000000;

  int i, counter;
  int SrcNId, DstNId;

  TEdgeW W = 1;

  EXPECT_TRUE(Graph->Empty());

  // CREATE NODES AND EDGES

  // create nodes
  for (i = 0; i < Nodes; i++) {
    Graph->AddNode(i);
  }
  EXPECT_FALSE(Graph->Empty());
  EXPECT_TRUE(Graph->IsOk());
  EXPECT_EQ(Nodes, Graph->GetNodes());

  // create edges (unique)
  for (i = 0; i < Edges; ) {
    SrcNId = (long) (Nodes * drand48());
    DstNId = (long) (Nodes * drand48());
    if (SrcNId != DstNId  &&  !Graph->IsEdge(SrcNId, DstNId)) {
      Graph->AddEdge(SrcNId, DstNId, W);
      i++;
    }
  }
  EXPECT_FALSE(Graph->Empty());
  EXPECT_TRUE(Graph->IsOk());
  EXPECT_EQ(Edges, Graph->GetEdges());

  // NODE ITERATORS

  // nodes forward iterator
  counter = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    counter++;
    EXPECT_TRUE(Graph->IsNode(NI.GetId()));
  }
  EXPECT_EQ(Nodes, counter);

  // nodes backward iterator
  counter = 0;
  for (NI = Graph->EndNI(); NI > Graph->BegNI(); ) {
    NI--;
    counter++;
    EXPECT_TRUE(Graph->IsNode(NI.GetId()));
  }
  EXPECT_EQ(Nodes, counter);

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
      Graph->AddEdge(SrcNId, DstNId, W);
      TotalW += W;
      EXPECT_DOUBLE_EQ(W, Graph->GetEI(SrcNId, DstNId).GetW());
      counter++;
    }
  }
  EXPECT_FLOAT_EQ(TotalW, Graph->GetTotalW());

  // EDGE ITERATORS

  // edges forward iterator
  counter = 0;
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    EdgeTotalW += EI.GetW();
    counter++;
    EXPECT_TRUE(Graph->IsEdge(EI.GetSrcNId(), EI.GetDstNId()));
  }
  EXPECT_EQ(Edges, counter);
  EXPECT_FLOAT_EQ(TotalW, EdgeTotalW);

  // edges backward iterator
  counter = 0;
  for (EI = Graph->EndEI(); EI > Graph->BegEI(); ) {
    EI--;
    counter++;
    EXPECT_TRUE(Graph->IsEdge(EI.GetSrcNId(), EI.GetDstNId()));
  }
  EXPECT_EQ(Edges, counter);

  // count edges (per node)
  counter = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    for (int e = 0; e < NI.GetOutDeg(); e++) {
      NodeTotalW += NI.GetOutEW(e);
      counter++;
      EXPECT_TRUE(Graph->IsEdge(NI.GetId(), NI.GetOutNId(e)));
    }
  }
  EXPECT_EQ(Edges, counter);
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
      EId = Graph->AddEdge(SrcNId, DstNId, W);
      TotalW += W;
      EXPECT_DOUBLE_EQ(W, Graph->GetEI(EId).GetW());
      counter++;
    }
  }
  EXPECT_FLOAT_EQ(TotalW, Graph->GetTotalW());

  // EDGE ITERATORS

  // edges forward iterator
  counter = 0;
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    EdgeTotalW += EI.GetW();
    counter++;
    EXPECT_TRUE(Graph->IsEdge(EI.GetId()));
  }
  EXPECT_EQ(Edges, counter);
  EXPECT_FLOAT_EQ(TotalW, EdgeTotalW);

  // edges backward iterator
  counter = 0;
  for (EI = Graph->EndEI(); EI > Graph->BegEI(); ) {
    EI--;
    counter++;
    EXPECT_TRUE(Graph->IsEdge(EI.GetId()));
  }
  EXPECT_EQ(Edges, counter);

  // count edges (per node)
  counter = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    for (int e = 0; e < NI.GetOutDeg(); e++) {
      NodeTotalW += NI.GetOutEW(e);
      counter++;
      EXPECT_TRUE(Graph->IsEdge(NI.GetOutEId(e)));
    }
  }
  EXPECT_EQ(Edges, counter);
  EXPECT_FLOAT_EQ(TotalW, NodeTotalW);

}

  // // create edges
  // for (i = 0; i < Edges; i++) {
  //   SrcNId = (long) (Nodes * drand48());
  //   DstNId = (long) (Nodes * drand48());
  //   while (SrcNId == DstNId) {
  //     DstNId = (long) ( Nodes * drand48());
  //   }
  //   Graph->AddEdge(SrcNId, DstNId);
  // }
