// Headers (?)

#include "stdafx.h"

#include "galg.h"
#include "stats.h"

//#//////////////////////////////////////////////
/// Directed graphs and multigraphs

typedef ::testing::Types<TUNGraph, TNGraph, TWNGraph<TInt> > NoMultiGraphs;
// typedef ::testing::Types<TNGraph> Graphs;

template <class TEdgeW>
class GraphAlgorithmsTest : public ::testing::Test {
public:
  GraphAlgorithmsTest() {}
};

TYPED_TEST_CASE(GraphAlgorithmsTest, NoMultiGraphs);

TYPED_TEST(GraphAlgorithmsTest, SmallGraphStatistics) {

  // DECLARATIONS AND INITIALIZATIONS

  TIntIntVH::TIter HI;
  TIntV::TIter VI;
  
  typedef TypeParam TGraph;
  typedef TPt<TGraph> PGraph;

  PGraph Graph = TGraph::New();
  
  int depth, counter, divisions = 100;
  double p;
  TUInt64V InNF, OutNF, NF;

  TIntV NIdV;

  TIntIntVH InShortestPathVH, OutShortestPathVH, ShortestPathVH;

  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED
  
  CreateSmallGraph(Graph);
  
  // Fixed memory exact neighborhood function (exhaustive BFS)
  
  Graph->GetNIdV(NIdV);
  
  TSnap::TCustomFixedMemoryNeighborhood<PGraph> TCustomFixedMemoryNeighborhood(Graph, NIdV);
  TCustomFixedMemoryNeighborhood.ComputeCustomInNF(InNF, InShortestPathVH);
  TCustomFixedMemoryNeighborhood.ComputeCustomOutNF(OutNF, OutShortestPathVH);
  TCustomFixedMemoryNeighborhood.ComputeCustomNF(NF, ShortestPathVH);
  
  printf("InShortestPathVH\n----------------\n");
  for (int i = 0; i < InShortestPathVH.Len(); i++) {
    printf("(%d)", i);
    const TIntV& InShortestPathV = InShortestPathVH.GetDat(i);
    for (int j = 0; j < InShortestPathV.Len(); j++) {
      printf(" %2d", (int) InShortestPathV[j]);
      if (i == j) {
        EXPECT_EQ(0, InShortestPathV[j]);
      } else {
        EXPECT_GT(Graph->GetNodes(), InShortestPathV[j]);
      }
      if (Graph->HasFlag(gfDirected)) {
        if (i == 0 && j != 0) {
          EXPECT_EQ(-1, InShortestPathV[j]);
        }
      } else {
        EXPECT_LE(0, InShortestPathV[j]);
      }
    }
    printf("\n");
  }

  printf("OutShortestPathVH\n----------------\n");
  for (int i = 0; i < OutShortestPathVH.Len(); i++) {
    printf("(%d)", i);
    const TIntV& OutShortestPathV = OutShortestPathVH.GetDat(i);
    for (int j = 0; j < OutShortestPathV.Len(); j++) {
      printf(" %2d", (int) OutShortestPathV[j]);
      if (i == j) {
        EXPECT_EQ(0, OutShortestPathV[j]);
      } else {
        EXPECT_GT(Graph->GetNodes(), OutShortestPathV[j]);
      }
      if (Graph->HasFlag(gfDirected)) {
        if (i != 0 && j == 0) {
          EXPECT_EQ(-1, OutShortestPathV[j]);
        }
      } else {
        EXPECT_LE(0, OutShortestPathV[j]);
      }
    }
    printf("\n");
  }

  printf("ShortestPathVH\n----------------\n");
  for (int i = 0; i < ShortestPathVH.Len(); i++) {
    printf("(%d)", i);
    const TIntV& ShortestPathV = ShortestPathVH.GetDat(i);
    for (int j = 0; j < ShortestPathV.Len(); j++) {
      printf(" %2d", (int) ShortestPathV[j]);
      if (i == j) {
        EXPECT_EQ(0, ShortestPathV[j]);
      } else {
        EXPECT_GT(Graph->GetNodes(), ShortestPathV[j]);
        EXPECT_LE(0, ShortestPathV[j]);
      }
    }
    printf("\n");
  }

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

TYPED_TEST(GraphAlgorithmsTest, TinyGraphStatistics) {

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

TYPED_TEST(GraphAlgorithmsTest, RandomGraphStatistics) {

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
