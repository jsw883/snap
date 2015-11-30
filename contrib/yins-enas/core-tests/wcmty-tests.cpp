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

  TEdgeW W, EdgeW;
  TEdgeW MxW = 100, TotalW = 0;
  
  double quality;
  
  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED

  // // create nodes
  // for (counter = 0; counter < Nodes; counter++) {
  //   Graph->AddNode(counter);
  // }
  // EXPECT_TRUE(Graph->IsOk());

  // // create edges (unique with random weights)
  // for (counter = 0; counter < Edges; ) {
  //   SrcNId = (long) (Nodes * drand48());
  //   DstNId = (long) (Nodes * drand48());
  //   W = (TEdgeW) (MxW * drand48() + 1);
  //   if (SrcNId != DstNId  &&  !Graph->IsEdge(SrcNId, DstNId)) {
  //     // create edge
  //     Graph->AddEdge(SrcNId, DstNId, W);
  //     counter++;
  //   }
  // }

  Graph->AddNode(0);
  Graph->AddNode(1);
  Graph->AddNode(2);
  Graph->AddNode(3);
  Graph->AddNode(4);

  Graph->AddEdge(0, 1, 1);
  Graph->AddEdge(1, 0, 1);
  Graph->AddEdge(1, 2, 1);
  Graph->AddEdge(2, 3, 1);
  Graph->AddEdge(2, 4, 1);
  Graph->AddEdge(3, 4, 1);
  Graph->AddEdge(3, 2, 1);
  Graph->AddEdge(4, 2, 1);
  Graph->AddEdge(4, 3, 1);

  // LOUVAIN METHOD
  
  TIntIntVH CmtyVH;
  
  quality = TSnap::LouvainMethod<TSnap::ModularityCommunity<TEdgeW>, TEdgeW>(Graph, CmtyVH, edUnDirected, 1e-5, 1e-2, 1000);

  printf("graph edges and weights:\n");
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    printf("(%d, %d) %f\n", EI.GetSrcNId(), EI.GetDstNId(), (double) EI.GetW());
  }
  
  printf("community hierarchy:\n");
  for (HI = CmtyVH.BegI(); HI < CmtyVH.EndI(); HI++) {
    printf("%d: ", (int) HI.GetKey());
    for (VI = HI.GetDat().BegI(); VI < HI.GetDat().EndI(); VI++) {
      printf("%d, ", VI->Val);
    }
    printf("\n");
  }
  
  printf("quality: %f\n", quality);
  
}