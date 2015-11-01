// Headers (?)

#include "stdafx.h"
#include "wgraph.h"

template <class TEdgeW, template<class> class TGraph>
struct TypePair {
  typedef TEdgeW TypeEdgeW;
  typedef TGraph<TEdgeW> TypeGraph;
};

typedef ::testing::Types<TypePair<TInt, TWNGraph>, TypePair<TFlt, TWNGraph>, TypePair<TInt, TWNEGraph>, TypePair<TFlt, TWNEGraph> > Graphs;

//#//////////////////////////////////////////////
/// Weighted directed graphs and multigraphs

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
TYPED_TEST(GraphTest, GraphManipulating) {

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

  // create graph for maniupating
  Graph = TGraph::New();

  EXPECT_EQ(1, Graph->Empty());
  
  // create nodes
  for (i = 0; i < Nodes; i++) {
    Graph->AddNode(i);
  }

  EXPECT_EQ(0, Graph->Empty());
  EXPECT_EQ(1, Graph->IsOk());
  EXPECT_EQ(Nodes, Graph->GetNodes());
  for (i = 0; i < Nodes; i++) {
    EXPECT_EQ(1, Graph->IsNode(i));
  }
  EXPECT_EQ(0, Graph->IsNode(Nodes));

  // create edges
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



//#//////////////////////////////////////////////
/// Weighted directed multigraphs


