// Headers (?)

#include "stdafx.h"
#include "wgraph.h"

// Test the default constructor
TEST(TWNGraph, DefaultConstructor) {
  PIntWNEGraph IntWGraph;
  // PFltWNEGraph FltWGraph;

  IntWGraph = TIntWNEGraph::New();
  // FltWGraph = TFltWNEGraph::New();

  EXPECT_EQ(0, IntWGraph->GetNodes());
  EXPECT_EQ(0, IntWGraph->GetEdges());

  EXPECT_EQ(1, IntWGraph->IsOk());
  EXPECT_EQ(1, IntWGraph->Empty());
  EXPECT_EQ(1, IntWGraph->HasFlag(gfDirected));
}
