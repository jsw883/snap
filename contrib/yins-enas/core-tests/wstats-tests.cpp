// Headers (?)

#include "stdafx.h"

#include "galg.h"
#include "wstats.h"
#include "stats.h"

//#//////////////////////////////////////////////
/// Weighted directed graphs (no multigraphs)

typedef ::testing::Types<TInt, TFlt> Weights;

template <class TEdgeW>
class WStatsTest : public ::testing::Test {
public:
  WStatsTest() {}
};

TYPED_TEST_CASE(WStatsTest, Weights);

  
// Test directed graph clustering coefficients
TYPED_TEST(WStatsTest, SmallGraphWeightedClustering) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TEdgeW;
  typedef TPt<TWNGraph<TEdgeW> > PGraph;

  PGraph Graph = TWNGraph<TEdgeW>::New();
  
  double WAvDirClustCoeff;
  
  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED
  
  CreateSmallGraph(Graph);
  
  // Clustering coefficients
  
  WAvDirClustCoeff = TSnap::GetWAvLocalClustCoeff(Graph);
  
  EXPECT_EQ(0.25, WAvDirClustCoeff);
  
}
 
// Test directed graph clustering coefficients
TYPED_TEST(WStatsTest, RandomGraphWeightedClustering) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TEdgeW;
  typedef TPt<TWNGraph<TEdgeW> > PGraph;

  PGraph Graph = TWNGraph<TEdgeW>::New();
  
  int Nodes = 1000;
  int Edges = 10000;

  TEdgeW MxW = 100, TotalW = 0;
  
  double AvDirClustCoeff, WAvDirClustCoeff;
  
  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED
  
  CreateRandomGraph(Graph, Nodes, Edges);
  
  // Clustering coefficients
  
  AvDirClustCoeff = TSnap::GetAvDirLocalClustCoeff(Graph);
  WAvDirClustCoeff = TSnap::GetWAvDirLocalClustCoeff(Graph);
  
  EXPECT_FLOAT_EQ(AvDirClustCoeff, WAvDirClustCoeff);
  
  // Expected ranges
  EXPECT_LE(0, WAvDirClustCoeff);
  EXPECT_GE(1, WAvDirClustCoeff);
  
}
