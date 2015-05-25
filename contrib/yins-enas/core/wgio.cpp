// Headers (?)

#include "stdafx.h"
#include "wgio.h"

// Loads the format saved by TSnap::SaveWEdgeList()
PIntWNEGraph TSnap::LoadIntWEdgeList(const TStr& InFNm, const int& SrcColId, const int& DstColId, const int& WColId) {
  TSsParser Ss(InFNm, ssfWhiteSep, true, true, true);
  PIntWNEGraph Graph = PIntWNEGraph::TObj::New();
  int SrcNId, DstNId;
  int EdgeW;
  while (Ss.Next()) {
    if (! Ss.GetInt(SrcColId, SrcNId) || ! Ss.GetInt(DstColId, DstNId) || ! Ss.GetInt(WColId, EdgeW)) { continue; }
    if (! Graph->IsNode(SrcNId)) { Graph->AddNode(SrcNId); }
    if (! Graph->IsNode(DstNId)) { Graph->AddNode(DstNId); }
    Graph->AddEdge(SrcNId, DstNId, EdgeW);
  }
  Graph->Defrag();
  return Graph;
}
PFltWNEGraph TSnap::LoadFltWEdgeList(const TStr& InFNm, const int& SrcColId, const int& DstColId, const int& WColId) {
  TSsParser Ss(InFNm, ssfWhiteSep, true, true, true);
  PFltWNEGraph Graph = PFltWNEGraph::TObj::New();
  int SrcNId, DstNId;
  double EdgeW;
  while (Ss.Next()) {
    if (! Ss.GetInt(SrcColId, SrcNId) || ! Ss.GetInt(DstColId, DstNId) || ! Ss.GetFlt(WColId, EdgeW)) { continue; }
    if (! Graph->IsNode(SrcNId)) { Graph->AddNode(SrcNId); }
    if (! Graph->IsNode(DstNId)) { Graph->AddNode(DstNId); }
    Graph->AddEdge(SrcNId, DstNId, EdgeW);
  }
  Graph->Defrag();
  return Graph;
}

// Saves a weighted graph into a text file. Each line contains two columns and encodes a single edge: <source node id><tab><destination node id><tab><edge weight>
void TSnap::SaveIntWEdgeList(const PIntWNEGraph& Graph, const TStr& OutFNm, const TStr& Desc) {
  FILE *F = fopen(OutFNm.CStr(), "wt");
  if (HasGraphFlag(typename PIntWNEGraph::TObj, gfDirected)) { fprintf(F, "# Directed graph: %s \n", OutFNm.CStr()); } 
  else { fprintf(F, "# Undirected graph (each unordered pair of nodes is saved once): %s\n", OutFNm.CStr()); }
  if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
  fprintf(F, "# Nodes: %d Edges: %d\n", Graph->GetNodes(), Graph->GetEdges());
  if (HasGraphFlag(typename PIntWNEGraph::TObj, gfDirected)) { fprintf(F, "# SrcNodeId\tDstNodeId\tEdgeW\n"); }
  else { fprintf(F, "# NodeId\tNodeId\tEdgeW\n"); }
  for (typename PIntWNEGraph::TObj::TEdgeI EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    fprintf(F, "%d\t%d\t%d\n", EI.GetSrcNId(), EI.GetDstNId(), int(EI.GetW()));
  }
  fclose(F);
}
void TSnap::SaveFltWEdgeList(const PFltWNEGraph& Graph, const TStr& OutFNm, const TStr& Desc) {
  FILE *F = fopen(OutFNm.CStr(), "wt");
  if (HasGraphFlag(typename PFltWNEGraph::TObj, gfDirected)) { fprintf(F, "# Directed graph: %s \n", OutFNm.CStr()); } 
  else { fprintf(F, "# Undirected graph (each unordered pair of nodes is saved once): %s\n", OutFNm.CStr()); }
  if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
  fprintf(F, "# Nodes: %d Edges: %d\n", Graph->GetNodes(), Graph->GetEdges());
  if (HasGraphFlag(typename PFltWNEGraph::TObj, gfDirected)) { fprintf(F, "# SrcNodeId\tDstNodeId\tEdgeW\n"); }
  else { fprintf(F, "# NodeId\tNodeId\tEdgeW\n"); }
  for (typename PFltWNEGraph::TObj::TEdgeI EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    fprintf(F, "%d\t%d\t%f\n", EI.GetSrcNId(), EI.GetDstNId(), double(EI.GetW()));
  }
  fclose(F);
}