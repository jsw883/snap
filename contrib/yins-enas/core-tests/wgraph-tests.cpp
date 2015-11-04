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
TYPED_TEST(GraphTest, NodeEdgeManipulating) {

  // typedef typename TypeParam::TypeEdgeW TEdgeW;
  typedef typename TypeParam::TypeGraph TGraph;
  typedef TPt<TGraph> PGraph;

  PGraph Graph = TGraph::New();
  
  int Nodes = 10000;
  int Edges = 1000000;
  // const char *FName = "graph.dat";

  int i, counter;
  int x, y;
  // int Deg, InDeg, OutDeg;

  EXPECT_EQ(1, Graph->Empty());
  
  // create nodes
  for (i = 0; i < Nodes; i++) {
    Graph->AddNode(i);
  }
  EXPECT_EQ(0, Graph->Empty());
  EXPECT_EQ(1, Graph->IsOk());
  EXPECT_EQ(Nodes, Graph->GetNodes());

  // create edges (unique)
  for (i = 0; i < Edges; ) {
    x = (long) (Nodes * drand48());
    y = (long) (Nodes * drand48());
    if (x != y  &&  !Graph->IsEdge(x, y)) {
      Graph->AddEdge(x, y);
      i++;
    }
  }
  EXPECT_EQ(0, Graph->Empty());
  EXPECT_EQ(1, Graph->IsOk());
  EXPECT_EQ(Edges, Graph->GetEdges());

  // nodes iterator
  counter = 0;
  for (typename TGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    counter++;
  }
  EXPECT_EQ(Nodes, counter);

  // edges per node iterator
  counter = 0;
  for (typename TGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    for (int e = 0; e < NI.GetOutDeg(); e++) {
      counter++;
    }
  }
  EXPECT_EQ(Edges, counter);

  // edges iterator
  counter = 0;
  for (typename TGraph::TEdgeI EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    counter++;
  }
  EXPECT_EQ(Edges, counter);

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
TYPED_TEST(TWNGraphTest, EdgeWeights) {

  // typedef typename TypeParam::TypeEdgeW TEdgeW;
  typedef TypeParam TEdgeW;
  typedef TPt<TWNGraph<TEdgeW> > PGraph;

  typename TWNGraph<TEdgeW>::TNodeI NI;
  typename TWNGraph<TEdgeW>::TEdgeI EI;

  PGraph Graph = TWNGraph<TEdgeW>::New();
  
  int Nodes = 10000;
  int Edges = 1000000;
  // const char *FName = "graph.dat";

  int i, counter;
  int x, y;
  // int Deg, InDeg, OutDeg;

  TEdgeW w, EdgeW;
  TEdgeW MxW = 100, TotalW = 0, EdgeTotalW = 0, NodeTotalW = 0;

  // create nodes
  for (i = 0; i < Nodes; i++) {
    Graph->AddNode(i);
  }
  EXPECT_EQ(1, Graph->IsOk());

  // edges created
  typedef TPair<TInt, TInt> TIntPair;
  THash<TIntPair, TEdgeW> EdgeH;

  // create edges (unique with random weights)
  for (i = 0; i < Edges; ) {
    x = (long) (Nodes * drand48());
    y = (long) (Nodes * drand48());
    w = (TEdgeW) (MxW * drand48() + 1);
    if (x != y  &&  !Graph->IsEdge(x, y)) {
      Graph->AddEdge(x, y, w);
      TotalW += w;
      EXPECT_DOUBLE_EQ(w, Graph->GetEI(x, y).GetW());
      i++;
    }
  }
  EXPECT_FLOAT_EQ(TotalW, Graph->GetTotalW());
  
  // check edge iterator weights
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    EdgeTotalW += EI.GetW();
  }
  EXPECT_FLOAT_EQ(TotalW, EdgeTotalW);
  
  // check node iterator weights
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    for (int e = 0; e < NI.GetOutDeg(); e++) {
      NodeTotalW += NI.GetOutEW(e);
    }
  }
  EXPECT_FLOAT_EQ(TotalW, NodeTotalW);

}

  // // create edges
  // for (i = 0; i < Edges; i++) {
  //   x = (long) (Nodes * drand48());
  //   y = (long) (Nodes * drand48());
  //   while (x == y) {
  //     y = (long) ( Nodes * drand48());
  //   }
  //   Graph->AddEdge(x, y);
  // }

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
TYPED_TEST(TWNEGraphTest, EdgeWeights) {

  // typedef typename TypeParam::TypeEdgeW TEdgeW;
  typedef TypeParam TEdgeW;
  typedef TPt<TWNEGraph<TEdgeW> > PGraph;

  typename TWNEGraph<TEdgeW>::TNodeI NI;
  typename TWNEGraph<TEdgeW>::TEdgeI EI;

  PGraph Graph = TWNEGraph<TEdgeW>::New();
  
  int Nodes = 10000;
  int Edges = 1000000;
  
  int i, counter, EId;
  int x, y;
  // int Deg, InDeg, OutDeg;

  TEdgeW w, EdgeW;
  TEdgeW MxW = 100, TotalW = 0, EdgeTotalW = 0, NodeTotalW = 0;

  // create nodes
  for (i = 0; i < Nodes; i++) {
    Graph->AddNode(i);
  }
  EXPECT_EQ(1, Graph->IsOk());

  // edges created
  typedef TPair<TInt, TInt> TIntPair;
  THash<TIntPair, TEdgeW> EdgeH;

  // create edges (unique with random weights)
  for (i = 0; i < Edges; ) {
    x = (long) (Nodes * drand48());
    y = (long) (Nodes * drand48());
    w = (TEdgeW) (MxW * drand48() + 1);
    if (x != y  &&  !Graph->IsEdge(x, y)) {
      EId = Graph->AddEdge(x, y, w);
      TotalW += w;
      EXPECT_DOUBLE_EQ(w, Graph->GetEI(EId).GetW());
      i++;
    }
  }
  EXPECT_FLOAT_EQ(TotalW, Graph->GetTotalW());
  
  // check edge iterator weights
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    EdgeTotalW += EI.GetW();
  }
  EXPECT_FLOAT_EQ(TotalW, EdgeTotalW);
  
  // check node iterator weights
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    for (int e = 0; e < NI.GetOutDeg(); e++) {
      NodeTotalW += NI.GetOutEW(e);
    }
  }
  EXPECT_FLOAT_EQ(TotalW, NodeTotalW);

}


