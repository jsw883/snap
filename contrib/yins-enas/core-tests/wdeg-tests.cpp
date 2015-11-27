// Headers (?)

#include "stdafx.h"

//#//////////////////////////////////////////////
/// Weighted directed graphs and multigraphs

template <class TEdgeW, template<class> class TGraph>
struct TypePair {
  typedef TEdgeW TypeEdgeW;
  typedef TGraph<TEdgeW> TypeGraph;
};

typedef ::testing::Types<TypePair<TInt, TWNGraph>, TypePair<TFlt, TWNGraph>, TypePair<TInt, TWNEGraph>, TypePair<TFlt, TWNEGraph> > Graphs;

template <class TypePair>
class GraphTest : public ::testing::Test {
public:
  GraphTest() {}
};

TYPED_TEST_CASE(GraphTest, Graphs);

// Weighted degree
TYPED_TEST(GraphTest, WeightedDegrees) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef typename TypeParam::TypeEdgeW TEdgeW;
  typedef typename TypeParam::TypeGraph TGraph;
  typedef TPt<TGraph> PGraph;

  typename TGraph::TNodeI NI;
  typename TGraph::TEdgeI EI;

  PGraph Graph = TGraph::New();

  int Nodes = 10000;
  int Edges = 1000000;

  int counter, SrcNId, DstNId;

  TEdgeW W, EdgeW;
  TEdgeW MxW = 100, TotalW = 0;

  TEdgeW WInDegTotalW, WOutDegTotalW, WDegTotalW;

  THash<TInt, TEdgeW> WInDegH, WOutDegH, WDegH;
  typename  THash<TInt, TEdgeW>::TIter WHI;

  THash<TInt, TVec<TEdgeW> > WDegVH;
  TVec<TEdgeW> WDegV;
  typename THash<TInt, TVec<TEdgeW> >::TIter WVHI;

  // CREATE NODES AND EDGES

  // create nodes
  for (counter = 0; counter < Nodes; counter++) {
    Graph->AddNode(counter);
  }
  // create edges (unique)
  for (counter = 0; counter < Edges; ) {
    SrcNId = (long) (Nodes * drand48());
    DstNId = (long) (Nodes * drand48());
    W = (TEdgeW) (MxW * drand48() + 1);
    if (SrcNId != DstNId  &&  !Graph->IsEdge(SrcNId, DstNId)) {
      Graph->AddEdge(SrcNId, DstNId, W);
      counter++;
      TotalW += W;
    }
  }

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
class TWNGraphTest : public ::testing::Test {
public:
  TWNGraphTest() {}
};

TYPED_TEST_CASE(TWNGraphTest, Weights);

// Test graph edge weight consistency
TYPED_TEST(TWNGraphTest, SpecificGraphFunctionality) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TEdgeW;
  typedef TPt<TWNGraph<TEdgeW> > PGraph;

  typename TWNGraph<TEdgeW>::TNodeI NI;
  typename TWNGraph<TEdgeW>::TEdgeI EI;

  PGraph Graph = TWNGraph<TEdgeW>::New();
  
  int Nodes = 10000;
  int Edges = 1000000;

  int counter, SrcNId, DstNId;

  TEdgeW W, EdgeW;
  TEdgeW MxW = 100, TotalW = 0;
  
  TEdgeW WInDegTotalW, WOutDegTotalW, WDegTotalW;

  THash<TInt, TVec<TEdgeW> > kWInDegVH, kWOutDegVH, kWDegVH;
  TVec<TEdgeW> WDegV;
  typename THash<TInt, TVec<TEdgeW> >::TIter WVHI;

  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED

  // create nodes
  for (counter = 0; counter < Nodes; counter++) {
    Graph->AddNode(counter);
  }
  EXPECT_TRUE(Graph->IsOk());

  // create edges (unique with random weights)
  for (counter = 0; counter < Edges; ) {
    SrcNId = (long) (Nodes * drand48());
    DstNId = (long) (Nodes * drand48());
    W = (TEdgeW) (MxW * drand48() + 1);
    if (SrcNId != DstNId  &&  !Graph->IsEdge(SrcNId, DstNId)) {
      // create edge
      Graph->AddEdge(SrcNId, DstNId, W);
      counter++;
      // weights
      TotalW += W;
    }
  }

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
class TWNEGraphTest : public ::testing::Test {
public:
  TWNEGraphTest() {}
};

TYPED_TEST_CASE(TWNEGraphTest, Weights);

// Test graph edge weight consistency
TYPED_TEST(TWNEGraphTest, SpecificGraphFunctionality) {

  // DECLARATIONS AND INITIALIZATIONS

  typedef TypeParam TEdgeW;
  typedef TPt<TWNEGraph<TEdgeW> > PGraph;

  typename TWNEGraph<TEdgeW>::TNodeI NI;
  typename TWNEGraph<TEdgeW>::TEdgeI EI;

  PGraph Graph = TWNEGraph<TEdgeW>::New();
  
  int Nodes = 10000;
  int Edges = 1000000;

  int counter, SrcNId, DstNId;

  TEdgeW W, EdgeW;
  TEdgeW MxW = 100, TotalW = 0;
  
  TEdgeW WInDegTotalW, WOutDegTotalW, WDegTotalW;

  THash<TInt, TVec<TEdgeW> > kWInDegVH, kWOutDegVH, kWDegVH;
  TVec<TEdgeW> WDegV;
  typename THash<TInt, TVec<TEdgeW> >::TIter WVHI;

  // CREATE NODES AND EDGES AND CHECK WEIGHTS CREATED

  // create nodes
  for (counter = 0; counter < Nodes; counter++) {
    Graph->AddNode(counter);
  }
  EXPECT_TRUE(Graph->IsOk());

  // create edges (unique with random weights)
  for (counter = 0; counter < Edges; ) {
    SrcNId = (long) (Nodes * drand48());
    DstNId = (long) (Nodes * drand48());
    W = (TEdgeW) (MxW * drand48() + 1);
    if (SrcNId != DstNId  &&  !Graph->IsEdge(SrcNId, DstNId)) {
      // create edge
      Graph->AddEdge(SrcNId, DstNId, W);
      counter++;
      // weights
      TotalW += W;
    }
  }

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
