// Headers (?)

#include "stdafx.h"

#include "percolate.h"

//#//////////////////////////////////////////////
/// Directed graphs and multigraphs

typedef ::testing::Types<TUNGraph, TNGraph, TNEGraph, TWNGraph<TInt>, TWNEGraph<TInt> > Graphs;
// typedef ::testing::Types<TUNGraph> Graphs;

template <class TEdgeW>
class GraphPercolationTest : public ::testing::Test {
public:
  GraphPercolationTest() {}
};

TYPED_TEST_CASE(GraphPercolationTest, Graphs);

// Test graph generators
TYPED_TEST(GraphPercolationTest, PercolationThreshold) {

  typedef TypeParam TGraph;
  typedef TPt<TGraph> PGraph;

  int Nodes = 100;
  
  PGraph Graph;
  
  int rep = 100;
  double p, tol = 1e-3, lowerBound = 0.0, upperBound = 1.0;
  
  TRnd Rnd(0);
  
  // Line graph   
  
  Graph = TSnap::GenLine<PGraph>(Nodes);
  
  printf("Line\n====\n");
  printf("Nodes = %d\n", Graph->GetNodes());
  printf("Edges = %d\n", Graph->GetEdges());
  p = TSnap::FindPercolationThreshold(Graph, tol, lowerBound, upperBound, rep);
  printf("p = %f\n", p);
  
  // Line graph   
  
  Graph = TSnap::GenGrid<PGraph>((int) round(sqrt(Nodes)), (int) round(sqrt(Nodes)));
  
  printf("Grid\n====\n");
  printf("Nodes = %d\n", Graph->GetNodes());
  printf("Edges = %d\n", Graph->GetEdges());
  p = TSnap::FindPercolationThreshold(Graph, tol, lowerBound, upperBound, rep);
  printf("p = %f\n", p);
  
  // Full graph 
  
  Graph = TSnap::GenFull<PGraph>(Nodes);
  
  printf("Full\n====\n");
  printf("Nodes = %d\n", Graph->GetNodes());
  printf("Edges = %d\n", Graph->GetEdges());
  p = TSnap::FindPercolationThreshold(Graph, tol, lowerBound, upperBound, rep);
  printf("p = %f\n", p);
  
}
