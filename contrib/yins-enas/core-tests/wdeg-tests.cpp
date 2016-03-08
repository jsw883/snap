// Headers (?)

#include "stdafx.h"

#include "wdeg.h"

//#//////////////////////////////////////////////
/// Weighted directed graphs and multigraphs

template <class TEdgeW, template<class> class TGraph>
struct TypePair {
  typedef TEdgeW TypeEdgeW;
  typedef TGraph<TEdgeW> TypeGraph;
};

typedef ::testing::Types<TypePair<TInt, TWNGraph>, TypePair<TFlt, TWNGraph>, TypePair<TInt, TWNEGraph>, TypePair<TFlt, TWNEGraph> > Graphs;

template <class TypePair>
class WDegTest : public ::testing::Test {
public:
  WDegTest() {}
};

TYPED_TEST_CASE(WDegTest, Graphs);

// Weighted degree
TYPED_TEST(WDegTest, WeightedDegrees) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef typename TypeParam::TypeEdgeW TEdgeW;
  typedef typename TypeParam::TypeGraph TGraph;
  typedef TPt<TGraph> PGraph;

  PGraph Graph = TGraph::New();

  int Nodes = 10000;
  int Edges = 1000000;

  TEdgeW MxW = 100, TotalW = 0;

  TEdgeW WInDegTotalW, WOutDegTotalW, WDegTotalW;

  THash<TInt, TEdgeW> WInDegH, WOutDegH, WDegH;
  typename  THash<TInt, TEdgeW>::TIter WHI;

  THash<TInt, TVec<TEdgeW> > WDegVH;
  TVec<TEdgeW> WDegV;
  typename THash<TInt, TVec<TEdgeW> >::TIter WVHI;

  // CREATE NODES AND EDGES
  
  CreateRandomWGraph(Graph, Nodes, Edges, MxW, TotalW);
  
  // WEIGHTED DEGREES

  // weighted in degrees
  TSnap::GetWInDegH(Graph, WInDegH);
  WInDegTotalW = 0;
  for (WHI = WInDegH.BegI(); WHI < WInDegH.EndI(); WHI++) {
    WInDegTotalW += WHI.GetDat();
    EXPECT_FLOAT_EQ(Graph->GetNI(WHI.GetKey()).GetWInDeg(), WHI.GetDat());
  }
  EXPECT_FLOAT_EQ(TotalW, WInDegTotalW);

  // weighted out degrees
  TSnap::GetWOutDegH(Graph, WOutDegH);
  WOutDegTotalW = 0;
  for (WHI = WOutDegH.BegI(); WHI < WOutDegH.EndI(); WHI++) {
    WOutDegTotalW += WHI.GetDat();
    EXPECT_FLOAT_EQ(Graph->GetNI(WHI.GetKey()).GetWOutDeg(), WHI.GetDat());
  }
  EXPECT_FLOAT_EQ(TotalW, WOutDegTotalW);

  // weighted degrees
  TSnap::GetWDegH(Graph, WDegH);
  WDegTotalW = 0;
  for (WHI = WDegH.BegI(); WHI < WDegH.EndI(); WHI++) {
    WDegTotalW += WHI.GetDat();
    EXPECT_FLOAT_EQ(Graph->GetNI(WHI.GetKey()).GetWDeg(), WHI.GetDat());
  }
  EXPECT_FLOAT_EQ(TotalW, WDegTotalW/2);

  // vector of weighted degrees
  TSnap::GetWDegVH(Graph, WDegVH);
  WInDegTotalW = 0;
  WOutDegTotalW = 0;
  WDegTotalW = 0;
  for (WVHI = WDegVH.BegI(); WVHI < WDegVH.EndI(); WVHI++) {
    WDegV = WVHI.GetDat();
    WInDegTotalW += WDegV[0];
    WOutDegTotalW += WDegV[1];
    WDegTotalW += WDegV[2];
    EXPECT_FLOAT_EQ(Graph->GetNI(WVHI.GetKey()).GetWInDeg(), WDegV[0]);
    EXPECT_FLOAT_EQ(Graph->GetNI(WVHI.GetKey()).GetWOutDeg(), WDegV[1]);
    EXPECT_FLOAT_EQ(Graph->GetNI(WVHI.GetKey()).GetWDeg(), WDegV[2]);
  }
  EXPECT_FLOAT_EQ(TotalW, WInDegTotalW);
  EXPECT_FLOAT_EQ(TotalW, WOutDegTotalW);
  EXPECT_FLOAT_EQ(TotalW, WDegTotalW/2);

}

//#//////////////////////////////////////////////
/// Weighted directed graphs

typedef ::testing::Types<TInt, TFlt> Weights;

template <class TEdgeW>
class TWNGraphWDegTest : public ::testing::Test {
public:
  TWNGraphWDegTest() {}
};

TYPED_TEST_CASE(TWNGraphWDegTest, Weights);

// Test graph edge weight consistency
TYPED_TEST(TWNGraphWDegTest, SpecificGraphFunctionality) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TEdgeW;
  typedef TPt<TWNGraph<TEdgeW> > PGraph;

  PGraph Graph = TWNGraph<TEdgeW>::New();
  
  int Nodes = 10000;
  int Edges = 1000000;

  TEdgeW MxW = 100, TotalW = 0;
  
  TEdgeW WInDegTotalW, WOutDegTotalW, WDegTotalW;

  THash<TInt, TVec<TEdgeW> > kWInDegVH, kWOutDegVH, kWDegVH;
  TVec<TEdgeW> WDegV;
  typename THash<TInt, TVec<TEdgeW> >::TIter WVHI;

  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED
  
  CreateRandomWGraph(Graph, Nodes, Edges, MxW, TotalW);
  
  // ONE STEP k WEIGHTED DEGREES

  TSnap::TFixedMemorykWDeg<TEdgeW, TWNGraph> FixedMemorykWDeg(Graph, 1);
  FixedMemorykWDeg.GetkWInDegH(kWInDegVH);
  FixedMemorykWDeg.GetkWOutDegH(kWOutDegVH);
  FixedMemorykWDeg.GetkWDegH(kWDegVH);

  // weighted in degrees
  WInDegTotalW = 0;
  for (WVHI = kWInDegVH.BegI(); WVHI < kWInDegVH.EndI(); WVHI++) {
    WDegV = WVHI.GetDat();
    WInDegTotalW += WDegV[0];
    EXPECT_FLOAT_EQ(Graph->GetNI(WVHI.GetKey()).GetWInDeg(), WDegV[0]);
  }
  EXPECT_FLOAT_EQ(TotalW, WInDegTotalW);

  // weighted out degrees
  WOutDegTotalW = 0;
  for (WVHI = kWOutDegVH.BegI(); WVHI < kWOutDegVH.EndI(); WVHI++) {
    WDegV = WVHI.GetDat();
    WOutDegTotalW += WDegV[0];
    EXPECT_FLOAT_EQ(Graph->GetNI(WVHI.GetKey()).GetWOutDeg(), WDegV[0]);
  }
  EXPECT_FLOAT_EQ(TotalW, WOutDegTotalW);

  // weighted  degrees
  WDegTotalW = 0;
  for (WVHI = kWDegVH.BegI(); WVHI < kWDegVH.EndI(); WVHI++) {
    WDegV = WVHI.GetDat();
    WDegTotalW += WDegV[0];
    EXPECT_FLOAT_EQ(Graph->GetNI(WVHI.GetKey()).GetWDeg(), WDegV[0]);
  }
  EXPECT_FLOAT_EQ(TotalW, WDegTotalW/2);

}

//#//////////////////////////////////////////////
/// Weighted directed multigraphs

typedef ::testing::Types<TInt, TFlt> Weights;

template <class TEdgeW>
class TWNEGraphWDegTest : public ::testing::Test {
public:
  TWNEGraphWDegTest() {}
};

TYPED_TEST_CASE(TWNEGraphWDegTest, Weights);

// Test graph edge weight consistency
TYPED_TEST(TWNEGraphWDegTest, SpecificGraphFunctionality) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TEdgeW;
  typedef TPt<TWNEGraph<TEdgeW> > PGraph;

  typename TWNEGraph<TEdgeW>::TNodeI NI;
  typename TWNEGraph<TEdgeW>::TEdgeI EI;

  PGraph Graph = TWNEGraph<TEdgeW>::New();
  
  int Nodes = 10000;
  int Edges = 1000000;

  TEdgeW MxW = 100, TotalW = 0;
  
  TEdgeW WInDegTotalW, WOutDegTotalW, WDegTotalW;

  THash<TInt, TVec<TEdgeW> > kWInDegVH, kWOutDegVH, kWDegVH;
  TVec<TEdgeW> WDegV;
  typename THash<TInt, TVec<TEdgeW> >::TIter WVHI;

  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED
  
  CreateRandomWGraph(Graph, Nodes, Edges, MxW, TotalW);
  
  // ONE STEP k WEIGHTED DEGREES

  TSnap::TFixedMemorykWDeg<TEdgeW, TWNEGraph> FixedMemorykWDeg(Graph, 1);
  FixedMemorykWDeg.GetkWInDegH(kWInDegVH);
  FixedMemorykWDeg.GetkWOutDegH(kWOutDegVH);
  FixedMemorykWDeg.GetkWDegH(kWDegVH);

  // weighted in degrees
  WInDegTotalW = 0;
  for (WVHI = kWInDegVH.BegI(); WVHI < kWInDegVH.EndI(); WVHI++) {
    WDegV = WVHI.GetDat();
    WInDegTotalW += WDegV[0];
    EXPECT_FLOAT_EQ(Graph->GetNI(WVHI.GetKey()).GetWInDeg(), WDegV[0]);
  }
  EXPECT_FLOAT_EQ(TotalW, WInDegTotalW);

  // weighted out degrees
  WOutDegTotalW = 0;
  for (WVHI = kWOutDegVH.BegI(); WVHI < kWOutDegVH.EndI(); WVHI++) {
    WDegV = WVHI.GetDat();
    WOutDegTotalW += WDegV[0];
    EXPECT_FLOAT_EQ(Graph->GetNI(WVHI.GetKey()).GetWOutDeg(), WDegV[0]);
  }
  EXPECT_FLOAT_EQ(TotalW, WOutDegTotalW);

  // weighted  degrees
  WDegTotalW = 0;
  for (WVHI = kWDegVH.BegI(); WVHI < kWDegVH.EndI(); WVHI++) {
    WDegV = WVHI.GetDat();
    WDegTotalW += WDegV[0];
    EXPECT_FLOAT_EQ(Graph->GetNI(WVHI.GetKey()).GetWDeg(), WDegV[0]);
  }
  EXPECT_FLOAT_EQ(TotalW, WDegTotalW/2);

}
