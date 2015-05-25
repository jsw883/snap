// Headers (?)

#include "stdafx.h"
#include "wgraph.h"

/// Returns a small graph on 5 nodes and 6 edges (integer weights) ##TIntWNEGraph::GetSmallGraph
template <>
PIntWNEGraph TIntWNEGraph::GetSmallGraph() {
  PIntWNEGraph Graph = TIntWNEGraph::New();
  for (int i = 0; i < 5; i++) { Graph->AddNode(i); }
  Graph->AddEdge(0,1,1);  Graph->AddEdge(0,2,1);
  Graph->AddEdge(0,3,1);  Graph->AddEdge(0,4,1);
  Graph->AddEdge(1,2,1);  Graph->AddEdge(1,2,1);
  return Graph;
}

/// Returns a small graph on 5 nodes and 6 edges (float weights) ##TFltWNEGraph::GetSmallGraph
template <>
PFltWNEGraph TFltWNEGraph::GetSmallGraph() {
  PFltWNEGraph Graph = TFltWNEGraph::New();
  for (int i = 0; i < 5; i++) { Graph->AddNode(i); }
  Graph->AddEdge(0,1,1.0);  Graph->AddEdge(0,2,1.0);
  Graph->AddEdge(0,3,1.0);  Graph->AddEdge(0,4,1.0);
  Graph->AddEdge(1,2,1.0);  Graph->AddEdge(1,2,1.0);
  return Graph;
}
