// Headers (?)

#include "stdafx.h"

#include "galg.h"
#include "stats.h"

//#//////////////////////////////////////////////
/// Directed graphs and multigraphs

typedef ::testing::Types<TUNGraph, TNGraph, TNEGraph, TWNGraph<TInt>, TWNEGraph<TInt> > Graphs;

template <class TEdgeW>
class StatsTest : public ::testing::Test {
public:
  StatsTest() {}
};

TYPED_TEST_CASE(StatsTest, Graphs);

TYPED_TEST(StatsTest, SmallGraphStatistics) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TGraph;
  typedef TPt<TGraph> PGraph;

  PGraph Graph = TGraph::New();
  
  int depth, counter, divisions = 100;
  double p;
  TUInt64V InNF, OutNF, NF;
  
  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED
  
  CreateSmallGraph(Graph);
  
  // Fixed memory exact neighborhood function (exhaustive BFS)
  
  TSnap::TFixedMemoryNeighborhood<PGraph> TFixedMemoryNeighborhood(Graph);
  TFixedMemoryNeighborhood.ComputeInNF(InNF);
  TFixedMemoryNeighborhood.ComputeOutNF(OutNF);
  TFixedMemoryNeighborhood.ComputeNF(NF);
  
  // Check number of nodes
  EXPECT_EQ(7, InNF[0]);
  EXPECT_EQ(7, OutNF[0]);
  EXPECT_EQ(7, NF[0]);
  
  // Check diameters
  if (Graph->HasFlag(gfDirected)) {
    EXPECT_EQ(5, InNF.Len());
    EXPECT_EQ(5, OutNF.Len());
  } else {
    EXPECT_EQ(4, InNF.Len());
    EXPECT_EQ(4, OutNF.Len());
  }
  EXPECT_EQ(4, NF.Len());
  
  // Check in and out NF are the same (and undirected when undirected)
  EXPECT_EQ(InNF.Len(), OutNF.Len());
  for (depth = 0; depth < InNF.Len(); depth++) {
    EXPECT_EQ(InNF[depth], OutNF[depth]); 
    if (!Graph->HasFlag(gfDirected)) {
      EXPECT_EQ(InNF[depth], NF[depth]);
    }
  }
  
  // Interpolate
  
  // Limits
  EXPECT_EQ(0, TSnap::InterpolateNF(NF, 0));
  EXPECT_EQ(3, TSnap::InterpolateNF(NF, 1));
  
  // Smooth (hacky)
  EXPECT_TRUE(TSnap::InterpolateNF(NF, 0 + 1e-10) - TSnap::InterpolateNF(NF, 0) < 1e-5);
  EXPECT_TRUE(TSnap::InterpolateNF(NF, 1) - TSnap::InterpolateNF(NF, 1 - 1e-10) < 1e-5);
  for (counter = 1; counter < divisions - 1; counter++) {
    p = ((double) counter) / ((double) divisions);
    EXPECT_TRUE(TSnap::InterpolateNF(NF, p + 1e-10) - TSnap::InterpolateNF(NF, p) < 1e-5);
    EXPECT_TRUE(TSnap::InterpolateNF(NF, p) - TSnap::InterpolateNF(NF, p - 1e-10) < 1e-5);
  }
  
}

TYPED_TEST(StatsTest, TinyGraphStatistics) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TGraph;
  typedef TPt<TGraph> PGraph;

  PGraph Graph = TGraph::New();
  
  TUInt64V InNF, OutNF, NF;
  
  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED
  
  Graph->AddNode(0);
  
  // Fixed memory exact neighborhood function (exhaustive BFS)
  
  TSnap::TFixedMemoryNeighborhood<PGraph> TFixedMemoryNeighborhood(Graph);
  TFixedMemoryNeighborhood.ComputeInNF(InNF);
  TFixedMemoryNeighborhood.ComputeOutNF(OutNF);
  TFixedMemoryNeighborhood.ComputeNF(NF);
  
  // Check number of nodes
  EXPECT_EQ(1, InNF[0]);
  EXPECT_EQ(1, OutNF[0]);
  EXPECT_EQ(1, NF[0]);
  
  // Check diameters
  EXPECT_EQ(1, InNF.Len());
  EXPECT_EQ(1, OutNF.Len());
  EXPECT_EQ(1, NF.Len());
  
  // Interpolate
  
  // Limits
  EXPECT_EQ(0, TSnap::InterpolateNF(NF, 0));
  EXPECT_EQ(0, TSnap::InterpolateNF(NF, 0.5));
  EXPECT_EQ(0, TSnap::InterpolateNF(NF, 1));
  
}

TYPED_TEST(StatsTest, RandomGraphStatistics) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TGraph;
  typedef TPt<TGraph> PGraph;

  PGraph Graph = TGraph::New();
  
  int Nodes = 1000;
  int Edges = 10000;

  int counter;

  int depth, divisions = 100;
  double p;
  TUInt64V InNF, OutNF, NF;
  
  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED

  CreateRandomGraph(Graph, Nodes, Edges);

  // Fixed memory exact neighborhood function (exhaustive BFS)
  
  TSnap::TFixedMemoryNeighborhood<PGraph> TFixedMemoryNeighborhood(Graph);
  TFixedMemoryNeighborhood.ComputeInNF(InNF);
  TFixedMemoryNeighborhood.ComputeOutNF(OutNF);
  TFixedMemoryNeighborhood.ComputeNF(NF);
  
  // Check number of nodes
  EXPECT_EQ(Graph->GetNodes(), InNF[0]);
  EXPECT_EQ(Graph->GetNodes(), OutNF[0]);
  EXPECT_EQ(Graph->GetNodes(), NF[0]);
  
  // Check diameters are equal (and less than or equal for undirected)
  EXPECT_EQ(InNF.Len(), OutNF.Len());
  if (Graph->HasFlag(gfDirected)) {
    EXPECT_LE(NF.Len(), InNF.Len());
  } else {
    EXPECT_EQ(NF.Len(), InNF.Len());
  }
  
  // Check in and out NF are the same (and undirected when undirected)
  for (depth = 0; depth < InNF.Len(); depth++) {
    EXPECT_EQ(InNF[depth], OutNF[depth]); 
    if (!Graph->HasFlag(gfDirected)) {
      EXPECT_EQ(InNF[depth], NF[depth]);
    }
  }
  
  // Interpolate
  
  // Limits
  EXPECT_EQ(0, TSnap::InterpolateNF(NF, 0));
  EXPECT_EQ(NF.Len() - 1, TSnap::InterpolateNF(NF, 1));
  
  // Smooth (hacky)
  EXPECT_TRUE(TSnap::InterpolateNF(NF, 0 + 1e-10) - TSnap::InterpolateNF(NF, 0) < 1e-5);
  EXPECT_TRUE(TSnap::InterpolateNF(NF, 1) - TSnap::InterpolateNF(NF, 1 - 1e-10) < 1e-5);
  for (counter = 1; counter < divisions - 1; counter++) {
    p = ((double) counter) / ((double) divisions);
    EXPECT_TRUE(TSnap::InterpolateNF(NF, p + 1e-10) - TSnap::InterpolateNF(NF, p) < 1e-5);
    EXPECT_TRUE(TSnap::InterpolateNF(NF, p) - TSnap::InterpolateNF(NF, p - 1e-10) < 1e-5);
  }
  
}

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
  
  AvDirClustCoeff = TSnap::GetAvLocalClustCoeff(Graph);
  
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
  
  AvDirClustCoeff = TSnap::GetAvLocalClustCoeff(Graph);
  
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
  
  AvDirClustCoeff = TSnap::GetAvLocalClustCoeff(Graph);
  
  // Expected ranges
  EXPECT_LE(0, GlobClustCf);
  EXPECT_GE(1, GlobClustCf);
  EXPECT_LE(0, AvClustCf);
  EXPECT_GE(1, AvClustCf);
  EXPECT_LE(0, AvDirClustCoeff);
  EXPECT_GE(1, AvDirClustCoeff);
  
}
