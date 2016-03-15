// Headers (?)

#include "stdafx.h"

//#//////////////////////////////////////////////
/// Weighted directed graphs and multigraphs

template <class TEdgeW, template<class> class TGraph>
struct TypePair {
  typedef TEdgeW TypeEdgeW;
  typedef TGraph<TEdgeW> TypeGraph;
};

typedef ::testing::Types<TypePair<TInt, TWNGraph> > Graphs; // , TypePair<TFlt, TWNGraph>, TypePair<TInt, TWNEGraph>, TypePair<TFlt, TWNEGraph>

template <class TypePair>
class WGenTest : public ::testing::Test {
public:
  WGenTest() {}
};

TYPED_TEST_CASE(WGenTest, Graphs);

// Test default constructor
TYPED_TEST(WGenTest, Constructor) {
  
  typedef typename TypeParam::TypeEdgeW TEdgeW;
  typedef typename TypeParam::TypeGraph TGraph;
  typedef TPt<TGraph> PGraph;
  
  typename TGraph::TNodeI NI;
  typename TGraph::TEdgeI EI;

  int Nodes = 1000;
  int Edges = 10000;

  int counter, NId, SrcNId, DstNId;

  TEdgeW W, EdgeW;
  TEdgeW MxW = 100, TotalW = 0, NodeTotalW = 0;

  int InDeg, OutDeg, Deg;
  int TotalInDeg = 0, TotalOutDeg = 0, TotalDeg = 0;  
  TEdgeW WInDeg, WOutDeg, WDeg;
  TEdgeW TotalWInDeg = 0, TotalWOutDeg = 0, TotalWDeg = 0;  

}

//#//////////////////////////////////////////////
/// Weighted directed graphs

typedef ::testing::Types<TInt, TFlt> Weights;

template <class TEdgeW>
class TWNGenTest : public ::testing::Test {
public:
  TWNGenTest() {}
};

TYPED_TEST_CASE(TWNGraphTest, Weights);

// Test graph edge weight consistency
TYPED_TEST(TWNGenTest, SpecificGraphFunctionality) {

  typedef typename TypeParam::TypeEdgeW TEdgeW;
  typedef typename TypeParam::TypeGraph TGraph;
  typedef TPt<TGraph> PGraph;

  typename TGraph::TNodeI NI;
  typename TGraph::TEdgeI EI;

  int Nodes = 1000;
  int Edges = 10000;

  int counter, NId, SrcNId, DstNId;

  TEdgeW W, EdgeW;
  TEdgeW MxW = 100, TotalW = 0, NodeTotalW = 0;

  int InDeg, OutDeg, Deg;
  int TotalInDeg = 0, TotalOutDeg = 0, TotalDeg = 0;  
  TEdgeW WInDeg, WOutDeg, WDeg;
  TEdgeW TotalWInDeg = 0, TotalWOutDeg = 0, TotalWDeg = 0;  

}

//#//////////////////////////////////////////////
/// Weighted directed multigraphs

typedef ::testing::Types<TInt, TFlt> Weights;

template <class TEdgeW>
class TWNEGenTest : public ::testing::Test {
public:
  TWNEGenTest() {}
};

TYPED_TEST_CASE(TWNEGraphTest, Weights);

// Test graph edge weight consistency
TYPED_TEST(TWNEGenTest, SpecificGraphFunctionality) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TEdgeW;
  typedef TPt<TWNEGraph<TEdgeW> > PGraph;

  typename TWNEGraph<TEdgeW>::TNodeI NI;
  typename TWNEGraph<TEdgeW>::TEdgeI EI;

  int Nodes = 1000;
  int Edges = 10000;

  int counter, NId, SrcNId, DstNId;

  TEdgeW W, EdgeW;
  TEdgeW MxW = 100, TotalW = 0, NodeTotalW = 0;

  int InDeg, OutDeg, Deg;
  int TotalInDeg = 0, TotalOutDeg = 0, TotalDeg = 0;  
  TEdgeW WInDeg, WOutDeg, WDeg;
  TEdgeW TotalWInDeg = 0, TotalWOutDeg = 0, TotalWDeg = 0;  

}
