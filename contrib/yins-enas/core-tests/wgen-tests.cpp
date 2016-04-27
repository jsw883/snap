// Headers (?)

#include "stdafx.h"

//#//////////////////////////////////////////////
/// Weighted directed graphs

typedef ::testing::Types<TFlt> Weights;

template <class TEdgeW>
class TWNGenTest : public ::testing::Test {
public:
  TWNGenTest() {}
};

TYPED_TEST_CASE(TWNGenTest, Weights);

// Test graph generators
TYPED_TEST(TWNGenTest, RandomGraphGenerators) {

  typedef TypeParam TEdgeW;
  typedef TPt<TWNGraph<TEdgeW> > PGraph;

  int Nodes = 1000;
  int OutDeg = 10;

  TEdgeW TotalW = 1000000, Threshold = 1, ErdosRenyiTotalW, GenParetoBarabasiTotalW;
  
  double Scale = (double) 1; // TotalW / (Nodes * OutDeg);
  double Shape = (double) TotalW / (TotalW - Scale * Nodes * OutDeg);

  printf("Scale = %f\n", Scale);
  printf("Shape = %f\n", Shape);

  TRnd Rnd(0);
  
  PGraph Graph;

  // GEOMETRIC WEIGHTED ERDOS RENYI
  
  Graph.Clr();
  Graph = TSnap::GenGeoErdosRenyi<TEdgeW, TWNGraph>(Nodes, TotalW, Rnd);
  
  // graph properties, counts, and directed
  EXPECT_FALSE(Graph->Empty());
  EXPECT_TRUE(Graph->IsOk());
  EXPECT_EQ(Nodes, Graph->GetNodes());
  
  // check total weight within a 99% CI
  ErdosRenyiTotalW = Graph->GetTotalW();
  EXPECT_LE(TotalW * 0.95, ErdosRenyiTotalW);
  EXPECT_GE(TotalW * 1.05, ErdosRenyiTotalW);

  // EXPONENTIAL WEIGHTED ERDOS RENYI

  Graph.Clr();
  Graph = TSnap::GenExpErdosRenyi<TEdgeW, TWNGraph>(Nodes, TotalW, (double) TotalW / (Nodes * (Nodes - 1)), Rnd);
  
  // graph properties, counts, and directed
  EXPECT_FALSE(Graph->Empty());
  EXPECT_TRUE(Graph->IsOk());
  EXPECT_EQ(Nodes, Graph->GetNodes());
  
  // check total weight within a 99% CI
  ErdosRenyiTotalW = Graph->GetTotalW();
  EXPECT_GE(TotalW, ErdosRenyiTotalW);

  // WEIGHTED PREFERENTIAL ATTACHMENT

  Graph.Clr();
  Graph = TSnap::GenParetoBarabasi<TEdgeW, TWNGraph>(Nodes, Shape, Scale, OutDeg, edUnDirected, Rnd);
  
  TSnap::FitParetoWeights<TEdgeW>(Graph, Scale, Shape);

  printf("Scale = %f\n", Scale);
  printf("Shape = %f\n", Shape);

  // graph properties, counts, and directed
  EXPECT_FALSE(Graph->Empty());
  EXPECT_TRUE(Graph->IsOk());
  EXPECT_EQ(Nodes, Graph->GetNodes());
  
  // check total weight within a 99% CI
  GenParetoBarabasiTotalW = Graph->GetTotalW();

}

// // Test graph randomizers
// TYPED_TEST(TWNGenTest, RandomGraphAlgorithms) {

//   typedef TypeParam TEdgeW;
//   typedef TPt<TWNGraph<TEdgeW> > PGraph;

//   int Nodes = 1000;
//   int OutDeg = 10;

//   TEdgeW TotalW = 1000000;

//   TRnd Rnd(0);
  
//   PGraph Graph;

//   // GEOMETRIC WEIGHTED ERDOS RENYI
  
//   Graph.Clr();
//   Graph = TSnap::GenGeoErdosRenyi<TEdgeW, TWNGraph>(Nodes, TotalW, Rnd);
  
//   // WEIGHT RESHUFFLING
  
//   TSnap::WeightShuffling<TEdgeW>(Graph);
  
//   // graph properties, counts, and directed
//   EXPECT_FALSE(Graph->Empty());
//   EXPECT_TRUE(Graph->IsOk());
//   EXPECT_EQ(Nodes, Graph->GetNodes());
//   EXPECT_FLOAT_EQ(GenParetoBarabasiTotalW, Graph->GetTotalW());
  
//   // TODO: implement test for weights being shuffled
//   // TODO: implement test for degree, weight distributions being preserved
  
// }
