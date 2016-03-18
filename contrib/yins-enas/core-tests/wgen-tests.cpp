// Headers (?)

#include "stdafx.h"

//#//////////////////////////////////////////////
/// Weighted directed graphs and multigraphs

// template <class TEdgeW, template<class> class TGraph>
// struct TypePair {
//   typedef TEdgeW TypeEdgeW;
//   typedef TGraph<TEdgeW> TypeGraph;
// };

// typedef ::testing::Types<TypePair<TInt, TWNGraph> > Graphs; // , TypePair<TFlt, TWNGraph>, TypePair<TInt, TWNEGraph>, TypePair<TFlt, TWNEGraph>

// template <class TypePair>
// class WGenTest : public ::testing::Test {
// public:
//   WGenTest() {}
// };

// TYPED_TEST_CASE(WGenTest, Graphs);

// // Test default constructor
// TYPED_TEST(WGenTest, Constructor) {
  
//   typedef typename TypeParam::TypeEdgeW TEdgeW;
//   typedef typename TypeParam::TypeGraph TGraph;
//   typedef TPt<TGraph> PGraph;
  
//   typename TGraph::TNodeI NI;
//   typename TGraph::TEdgeI EI;

//   int Nodes = 1000;
//   int Edges = 10000;

//   int counter, NId, SrcNId, DstNId;

//   TEdgeW W, EdgeW;
//   TEdgeW MxW = 100, TotalW = 0, NodeTotalW = 0;

//   int InDeg, OutDeg, Deg;
//   int TotalInDeg = 0, TotalOutDeg = 0, TotalDeg = 0;  
//   TEdgeW WInDeg, WOutDeg, WDeg;
//   TEdgeW TotalWInDeg = 0, TotalWOutDeg = 0, TotalWDeg = 0;  

// }

//#//////////////////////////////////////////////
/// Weighted directed graphs

typedef ::testing::Types<TFlt> Weights;

template <class TEdgeW>
class TWNGenTest : public ::testing::Test {
public:
  TWNGenTest() {}
};

TYPED_TEST_CASE(TWNGenTest, Weights);

// Test graph edge weight consistency
TYPED_TEST(TWNGenTest, SpecificGraphFunctionality) {

  typedef TypeParam TEdgeW;
  typedef TPt<TWNGraph<TEdgeW> > PGraph;

  int Nodes = 1000;
  int OutDeg = 10;

  TEdgeW TotalW = 1000000, Threshold = 1, ErdosRenyiTotalW, PrefAttachTotalW;
  
  double Scale = (double) 1; // TotalW / (Nodes * OutDeg);
  double Shape = (double) TotalW / (TotalW - Scale * Nodes * OutDeg);

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
  printf("Geo Erdos Renyi\n-----------\n");
  printf("Nodes = %d\n", Graph->GetNodes());
  printf("Edges = %d\n", Graph->GetEdges());
  printf("TotalW = %f\n", (double) ErdosRenyiTotalW);
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
  printf("Exp Erdos Renyi\n-----------\n");
  printf("Nodes = %d\n", Graph->GetNodes());
  printf("Edges = %d\n", Graph->GetEdges());
  printf("TotalW = %f\n", (double) ErdosRenyiTotalW);
  EXPECT_GE(TotalW, ErdosRenyiTotalW);

  // WEIGHTED PREFERENTIAL ATTACHMENT

  Graph.Clr();
  Graph = TSnap::GenWPrefAttach<TEdgeW, TWNGraph>(Nodes, OutDeg, edUnDirected, Shape, Scale, Rnd);
  
  // graph properties, counts, and directed
  EXPECT_FALSE(Graph->Empty());
  EXPECT_TRUE(Graph->IsOk());
  EXPECT_EQ(Nodes, Graph->GetNodes());
  
  // check total weight within a 99% CI
  PrefAttachTotalW = Graph->GetTotalW();
  printf("Barabasi\n--------\n");
  printf("Nodes = %d\n", Graph->GetNodes());
  printf("Edges = %d\n", Graph->GetEdges());
  printf("TotalW = %f\n", (double) PrefAttachTotalW);

  // WEIGHT RESHUFFLING
  
  TSnap::WeightShuffling<TEdgeW>(Graph); // reshuffled Barabasi
  
  // graph properties, counts, and directed
  EXPECT_FALSE(Graph->Empty());
  EXPECT_TRUE(Graph->IsOk());
  EXPECT_EQ(Nodes, Graph->GetNodes());
  EXPECT_FLOAT_EQ(PrefAttachTotalW, Graph->GetTotalW());
  
  // TODO: implement test for weights being shuffled
  
  // check total weight within a 99% CI
  printf("Weight shuffled Barabasi\n------------------------\n");
  printf("Nodes = %d\n", Graph->GetNodes());
  printf("Edges = %d\n", Graph->GetEdges());
  printf("TotalW = %f\n", (double) Graph->GetTotalW());
  
}

//#//////////////////////////////////////////////
/// Weighted directed multigraphs

// typedef ::testing::Types<TInt, TFlt> Weights;

// template <class TEdgeW>
// class TWNEGenTest : public ::testing::Test {
// public:
//   TWNEGenTest() {}
// };

// TYPED_TEST_CASE(TWNEGraphTest, Weights);

// // Test graph edge weight consistency
// TYPED_TEST(TWNEGenTest, SpecificGraphFunctionality) {

//   // DECLARATIONS AND INITIALIZATIONS

//   typedef TypeParam TEdgeW;
//   typedef TPt<TWNEGraph<TEdgeW> > PGraph;

//   typename TWNEGraph<TEdgeW>::TNodeI NI;
//   typename TWNEGraph<TEdgeW>::TEdgeI EI;

//   int Nodes = 1000;
//   int Edges = 10000;

//   int counter, NId, SrcNId, DstNId;

//   TEdgeW W, EdgeW;
//   TEdgeW MxW = 100, TotalW = 0, NodeTotalW = 0;

//   int InDeg, OutDeg, Deg;
//   int TotalInDeg = 0, TotalOutDeg = 0, TotalDeg = 0;  
//   TEdgeW WInDeg, WOutDeg, WDeg;
//   TEdgeW TotalWInDeg = 0, TotalWOutDeg = 0, TotalWDeg = 0;  

// }
