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

  // int InDeg, OutDeg, Deg;
  // int TotalInDeg = 0, TotalOutDeg = 0, TotalDeg = 0;  
  // TEdgeW WInDeg, WOutDeg, WDeg;
  // TEdgeW TotalWInDeg = 0, TotalWOutDeg = 0, TotalWDeg = 0;  

  THash<TInt, TEdgeW> WInDegH, WOutDegH, WDegH;
  typename  THash<TInt, TEdgeW>::TIter WHI;

  THash<TInt, TVec<TEdgeW> > WDegVH;
  TVec<TEdgeW> WDegV;
  typename THash<TInt, TVec<TEdgeW> >::TIter WVHI;
  typename TVec<TEdgeW>::TIter VI;

  TEdgeW WInDegTotalW, WOutDegTotalW, WDegTotalW;

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

  TSnap::GetWInDegH(Graph, WInDegH);
  WInDegTotalW = 0;
  for (WHI = WInDegH.BegI(); WHI < WInDegH.EndI(); WHI++) {
    WInDegTotalW += WHI.GetDat();
    EXPECT_FLOAT_EQ(Graph->GetNI(WHI.GetKey()).GetWInDeg(), WHI.GetDat());
  }
  EXPECT_FLOAT_EQ(TotalW, WInDegTotalW);

  TSnap::GetWOutDegH(Graph, WOutDegH);
  WOutDegTotalW = 0;
  for (WHI = WOutDegH.BegI(); WHI < WOutDegH.EndI(); WHI++) {
    WOutDegTotalW += WHI.GetDat();
    EXPECT_FLOAT_EQ(Graph->GetNI(WHI.GetKey()).GetWOutDeg(), WHI.GetDat());
  }
  EXPECT_FLOAT_EQ(TotalW, WOutDegTotalW);

  TSnap::GetWDegH(Graph, WDegH);
  WDegTotalW = 0;
  for (WHI = WDegH.BegI(); WHI < WDegH.EndI(); WHI++) {
    WDegTotalW += WHI.GetDat();
    EXPECT_FLOAT_EQ(Graph->GetNI(WHI.GetKey()).GetWDeg(), WHI.GetDat());
  }
  EXPECT_FLOAT_EQ(TotalW, WDegTotalW/2);

  TSnap::GetWDegVH(Graph, WDegVH);
  WInDegTotalW = 0;
  WOutDegTotalW = 0;
  WDegTotalW = 0;
  for (WVHI = WDegVH.BegI(); WVHI < WDegVH.EndI(); WVHI++) {
    WDegV = WVHI.GetDat();
    WInDegTotalW += WDegV[0];
    WOutDegTotalW += WDegV[1];
    WDegTotalW += WDegV[2];
  }
  EXPECT_FLOAT_EQ(TotalW, WInDegTotalW);
  EXPECT_FLOAT_EQ(TotalW, WOutDegTotalW);
  EXPECT_FLOAT_EQ(TotalW, WDegTotalW/2);

  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    EXPECT_FLOAT_EQ(NI.GetWInDeg(), WInDegH.GetDat(NI.GetId()));
    EXPECT_FLOAT_EQ(NI.GetWOutDeg(), WOutDegH.GetDat(NI.GetId()));
    EXPECT_FLOAT_EQ(NI.GetWDeg(), WDegH.GetDat(NI.GetId()));
    WDegV = WDegVH.GetDat(NI.GetId());
    EXPECT_FLOAT_EQ(NI.GetWInDeg(), WDegV[0]);
    EXPECT_FLOAT_EQ(NI.GetWOutDeg(), WDegV[1]);
    EXPECT_FLOAT_EQ(NI.GetWDeg(), WDegV[2]);
  }

}