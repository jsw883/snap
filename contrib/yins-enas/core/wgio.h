#ifndef core_wgio
#define core_wgio

// Headers (?)

#include "io.h"

//#//////////////////////////////////////////////
/// Weighted graph save and load

namespace TSnap {

// template <class TEdgeW> TPt<TWNEGraph<TEdgeW> > LoadWEdgeList(const TStr& InFNm, const int& SrcColId = 0, const int& DstColId = 1, const int& WColId = 2);

// template <class TEdgeW>
// TPt<TWNEGraph<TEdgeW> > LoadWEdgeList(const TStr& InFNm, const int& SrcColId, const int& DstColId, const int& WColId) {
//   TSsParser Ss(InFNm, ssfWhiteSep, true, true, true);
//   TPt<TWNEGraph<TEdgeW> > WGraph = TPt<TWNEGraph<TEdgeW> >::TObj::New();
//   int SrcNId, DstNId;
//   TEdgeW EdgeW;
//   while (Ss.Next()) {
//     if (! Ss.GetInt(SrcColId, SrcNId) || ! Ss.GetInt(DstColId, DstNId) || ! Ss.GetInt(WColId, EdgeW)) { continue; } // can't template TInt / TFlt (?)
//     if (! WGraph->IsNode(SrcNId)) { WGraph->AddNode(SrcNId); }
//     if (! WGraph->IsNode(DstNId)) { WGraph->AddNode(DstNId); }
//     WGraph->AddEdge(SrcNId, DstNId, EdgeW);
//   }
//   WGraph->Defrag();
//   return WGraph;
// }

// Loads the format saved by TSnap::SaveWEdgeList()
template <template <class> class TGraph> TPt<TGraph<TInt> > LoadIntWEdgeList(const TStr& InFNm, const int& SrcColId = 0, const int& DstColId = 1, const int& WColId = 2);
template <template <class> class TGraph> TPt<TGraph<TFlt> > LoadFltWEdgeList(const TStr& InFNm, const int& SrcColId = 0, const int& DstColId = 1, const int& WColId = 2);

// Saves a weighted graph into a text file. Each line contains two columns and encodes a single edge: <source node id><tab><destination node id><tab><edge weight>
template <template <class> class TGraph> void SaveIntWEdgeList(const TPt<TGraph<TInt> >& Graph, const TStr& OutFNm, const TStr& Desc);
template <template <class> class TGraph> void SaveFltWEdgeList(const TPt<TGraph<TFlt> >& Graph, const TStr& OutFNm, const TStr& Desc);

// Load and save methods Implemented
  
// Loads the format saved by TSnap::SaveWEdgeList()
template <template <class> class TGraph> 
TPt<TGraph<TInt> > LoadIntWEdgeList(const TStr& InFNm, const int& SrcColId, const int& DstColId, const int& WColId) {
  TSsParser Ss(InFNm, ssfWhiteSep, true, true, true);
  TPt<TGraph<TInt> > Graph = TGraph<TInt>::New();
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
template <template <class> class TGraph> 
TPt<TGraph<TFlt> > LoadFltWEdgeList(const TStr& InFNm, const int& SrcColId, const int& DstColId, const int& WColId) {
  TSsParser Ss(InFNm, ssfWhiteSep, true, true, true);
  TPt<TGraph<TFlt> > Graph = TGraph<TFlt>::New();
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
template <template <class> class TGraph> 
void SaveIntWEdgeList(const TPt<TGraph<TInt> >& Graph, const TStr& OutFNm, const TStr& Desc) {
  FILE *F = fopen(OutFNm.CStr(), "wt");
  if (HasGraphFlag(typename TGraph<TInt>, gfDirected)) { fprintf(F, "# Directed graph: %s \n", OutFNm.CStr()); } 
  else { fprintf(F, "# Undirected graph (each unordered pair of nodes is saved once): %s\n", OutFNm.CStr()); }
  if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
  fprintf(F, "# Nodes: %d Edges: %d\n", Graph->GetNodes(), Graph->GetEdges());
  if (HasGraphFlag(typename TGraph<TInt>, gfDirected)) { fprintf(F, "# SrcNodeId\tDstNodeId\tEdgeW\n"); }
  else { fprintf(F, "# NodeId\tNodeId\tEdgeW\n"); }
  for (typename TGraph<TInt>::TEdgeI EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    fprintf(F, "%d\t%d\t%d\n", EI.GetSrcNId(), EI.GetDstNId(), int(EI.GetW()));
  }
  fclose(F);
}
template <template <class> class TGraph>  
void SaveFltWEdgeList(const TPt<TGraph<TFlt> >& Graph, const TStr& OutFNm, const TStr& Desc) {
  FILE *F = fopen(OutFNm.CStr(), "wt");
  if (HasGraphFlag(typename TGraph<TFlt>, gfDirected)) { fprintf(F, "# Directed graph: %s \n", OutFNm.CStr()); } 
  else { fprintf(F, "# Undirected graph (each unordered pair of nodes is saved once): %s\n", OutFNm.CStr()); }
  if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
  fprintf(F, "# Nodes: %d Edges: %d\n", Graph->GetNodes(), Graph->GetEdges());
  if (HasGraphFlag(typename TGraph<TFlt>, gfDirected)) { fprintf(F, "# SrcNodeId\tDstNodeId\tEdgeW\n"); }
  else { fprintf(F, "# NodeId\tNodeId\tEdgeW\n"); }
  for (typename TGraph<TFlt>::TEdgeI EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    fprintf(F, "%d\t%d\t%f\n", EI.GetSrcNId(), EI.GetDstNId(), double(EI.GetW()));
  }
  fclose(F);
}

} // namespace TSnap

//#//////////////////////////////////////////////
/// Weighted graph summary

namespace TSnap {

template<class PGraph>
void printIntWGraphSummary(const PGraph& Graph, const bool& Weights, const TStr& Desc = "WGraph Summary\n--------------") {
  typename PGraph::TObj::TEdgeI EI;
  TFltV ValV;
  int Nodes = Graph->GetNodes();
  int Edges = Graph->GetEdges();
  double TotalW = Graph->GetTotalW();
  double AvW = TotalW/double(Edges);
  double AvWDeg = 2*TotalW/double(Nodes);
  printGraphSummary(Graph, Desc);
  printf("TotalW: %e\n", TotalW);
  printf("AvW: %e\n", AvW);
  printf("AvWDeg: %e\n", AvWDeg);
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    ValV.Add(EI.GetW());
  }
  printValVSummary(ValV, "Weights\n");
}

template<class PGraph>
void printFltWGraphSummary(const PGraph& Graph, const bool& Weights, const TStr& Desc = "WGraph Summary\n--------------") {
  typename PGraph::TObj::TEdgeI EI;
  TFltV ValV;
  int Nodes = Graph->GetNodes();
  int Edges = Graph->GetEdges();
  double TotalW = Graph->GetTotalW();
  double AvW = TotalW/double(Edges);
  double AvWDeg = 2*TotalW/double(Nodes);
  printGraphSummary(Graph, Desc);
  printf("TotalW: %e\n", TotalW);
  printf("AvW: %e\n", AvW);
  printf("AvWDeg: %e\n", AvWDeg);
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    ValV.Add(EI.GetW());
  }
  printValVSummary(ValV, "\nWeights\n");
}

} // namespace TSnap


#endif
