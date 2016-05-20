// Headers (?)

#include "stdafx.h"

#include "galg.h"
#include "stats.h"

//#//////////////////////////////////////////////
/// Directed graphs (no multigraphs)

typedef ::testing::Types<TUNGraph, TNGraph, TWNGraph<TInt> > NoMultiGraphs;

template <class TEdgeW>
class StatsClustTest : public ::testing::Test {
public:
  StatsClustTest() {}
};

TYPED_TEST_CASE(StatsClustTest, NoMultiGraphs);

TYPED_TEST(StatsClustTest, SmallGraphStatistics) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TGraph;
  typedef TPt<TGraph> PGraph;

  PGraph Graph = TGraph::New();
  
  double GlobClustCf, AvClustCf, AvDirClustCoeff;
  
  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED
  
  CreateSmallGraph(Graph);
  
  // Clustering coefficients
  
  GlobClustCf = TSnap::GetGlobClustCf(Graph);
  AvClustCf = TSnap::GetAvClustCf(Graph);
  
  AvDirClustCoeff = TSnap::GetAvDirLocalClustCoeff(Graph);
  
  // Known answers
  EXPECT_EQ(0.4, GlobClustCf); // 4 closed triples, 10 connected triples 
  EXPECT_EQ(0.5, AvClustCf); // degree < 2 are not counted
  if (Graph->HasFlag(gfDirected)) {
    EXPECT_EQ(0.25, AvDirClustCoeff);
  } else {
    EXPECT_EQ(0.5, AvDirClustCoeff);
  }
  
}

TYPED_TEST(StatsClustTest, TinyGraphStatistics) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TGraph;
  typedef TPt<TGraph> PGraph;

  PGraph Graph = TGraph::New();
  
  double GlobClustCf, AvClustCf, AvDirClustCoeff;
  
  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED
  
  Graph->AddNode(0);
    
  // Clustering coefficients
  
  GlobClustCf = TSnap::GetGlobClustCf(Graph);
  AvClustCf = TSnap::GetAvClustCf(Graph);
  
  AvDirClustCoeff = TSnap::GetAvDirLocalClustCoeff(Graph);
  
  // Known answers
  EXPECT_EQ(0, GlobClustCf); 
  EXPECT_EQ(0, AvClustCf);
  EXPECT_EQ(0, AvDirClustCoeff);
  
}

TYPED_TEST(StatsClustTest, RandomGraphStatistics) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TGraph;
  typedef TPt<TGraph> PGraph;

  PGraph Graph = TGraph::New();
  
  int Nodes = 1000;
  int Edges = 10000;

  double GlobClustCf, AvClustCf, AvDirClustCoeff;
  
  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED

  CreateRandomGraph(Graph, Nodes, Edges);
    
  // Clustering coefficients
  
  GlobClustCf = TSnap::GetGlobClustCf(Graph);
  AvClustCf = TSnap::GetAvClustCf(Graph);
  
  AvDirClustCoeff = TSnap::GetAvDirLocalClustCoeff(Graph);
  
  // Expected ranges
  EXPECT_LE(0, GlobClustCf);
  EXPECT_GE(1, GlobClustCf);
  EXPECT_LE(0, AvClustCf);
  EXPECT_GE(1, AvClustCf);
  EXPECT_LE(0, AvDirClustCoeff);
  EXPECT_GE(1, AvDirClustCoeff);
  
}
