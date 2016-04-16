// Headers (?)

#include "stdafx.h"

#include "wcmty.h"

//#//////////////////////////////////////////////
/// Weighted directed graphs

typedef ::testing::Types<TInt, TFlt> Weights;

template <class TEdgeW>
class TWNGraphWCmtyTest : public ::testing::Test {
public:
  TWNGraphWCmtyTest() {}
};

TYPED_TEST_CASE(TWNGraphWCmtyTest, Weights);

// Test graph edge weight consistency
TYPED_TEST(TWNGraphWCmtyTest, LouvainMethod) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TEdgeW;
  typedef TPt<TWNGraph<TEdgeW> > PGraph;

  typename TWNGraph<TEdgeW>::TNodeI NI;
  typename TWNGraph<TEdgeW>::TEdgeI EI;
  TIntIntVH::TIter HI;
  TIntV::TIter VI;

  PGraph Graph = TWNGraph<TEdgeW>::New();
  
  // int Nodes = 25;
  // int Edges = 100;

  // int counter, SrcNId, DstNId;

  int NId;

  TEdgeW W, EdgeW;
  TEdgeW MxW = 100, TotalW = 0;
  
  double quality;
  
  TIntIntVH NIdCmtyVH;
  
  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED
  
  CreateSmallGraph(Graph);
  
  // LOUVAIN METHOD
  
  quality = TSnap::LouvainMethod<TSnap::ModularityCommunity<TEdgeW>, TEdgeW>(Graph, NIdCmtyVH, edUnDirected, 1e-5, 1e-2, 1000);

  TSnap::CmtyHierarchySummary(NIdCmtyVH, 10);
  
  // Test community hierarchy
  
  printf("quality: %f\n", quality);
  EXPECT_EQ(quality, quality);
  
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    NId = NI.GetId();
    EXPECT_EQ(NId, NIdCmtyVH.GetDat(NId)[0]);
  }
  int CmtyA = NIdCmtyVH.GetDat(1)[1];
  int CmtyB = NIdCmtyVH.GetDat(3)[1];
  EXPECT_EQ(CmtyA, NIdCmtyVH.GetDat(0)[1]);
  EXPECT_EQ(CmtyA, NIdCmtyVH.GetDat(1)[1]);
  EXPECT_EQ(CmtyA, NIdCmtyVH.GetDat(2)[1]);
  EXPECT_EQ(CmtyB, NIdCmtyVH.GetDat(3)[1]);
  EXPECT_EQ(CmtyB, NIdCmtyVH.GetDat(4)[1]);
  EXPECT_EQ(CmtyB, NIdCmtyVH.GetDat(5)[1]);
  EXPECT_EQ(CmtyB, NIdCmtyVH.GetDat(6)[1]);
  
}