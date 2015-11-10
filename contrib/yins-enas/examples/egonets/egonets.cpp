#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Egonet characteristics. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
   
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const TEdgeDir d = (TEdgeDir) Env.GetIfArgPrefixInt("-d:", 3, "direction: in (1), out (2), undirected (3)");
  const int k = Env.GetIfArgPrefixInt("-k:", 1, "depth of egonet traversal (1 / 2 / ...)");
  const bool collate = Env.GetIfArgPrefixBool("--collate:", false, "collate characteristics into matrix (T / F)");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", WGraph->GetNodes());
  printf("  edges: %d\n", WGraph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Declare variables
  TIntIntH NodesH, EdgesH;
  TIntFltH TotalWH, DensityH, GiniH;
  TFltWNGraph::TNodeI NI;
  
  // Loop over egonets (node iterator)
  printf("\n----------------------------------------\n");
  printf("Computing egonet properties (");
  switch(d) {
    case edInDirected: printf("in"); break;
    case edOutDirected: printf("out"); break;
    case edUnDirected: printf("undirected"); break;
  }
  printf(")");
  printf("\n----------------------------------------\n");
  printf("000%% : %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  int i = 1, lastPercentage = 0, currentPercentage = 0, percentageStep = 5;  
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++, i++) {
    
    // print only every 5%
    currentPercentage = floor(double(i) / double(WGraph->GetNodes()) * 100);
    if (currentPercentage >= lastPercentage + percentageStep) {
      printf("%03d%% : %s (%s)\n", currentPercentage, ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
      lastPercentage = currentPercentage;
    }
    
    const int NId = NI.GetId();
    
    TSnap::TFixedMemorykWEgo<TFlt, TWNGraph> FixedMemorykWEgo(WGraph, k);
    switch(d) {
      case edInDirected: FixedMemorykWEgo.ComputeInEgonetStatistics(NId); break;
      case edOutDirected: FixedMemorykWEgo.ComputeOutEgonetStatistics(NId); break;
      case edUnDirected: FixedMemorykWEgo.ComputeEgonetStatistics(NId); break;
    }
    NodesH.AddDat(NId, FixedMemorykWEgo.GetNodes());
    EdgesH.AddDat(NId, FixedMemorykWEgo.GetEdges());
    DensityH.AddDat(NId, FixedMemorykWEgo.GetDensity());
    TotalWH.AddDat(NId, FixedMemorykWEgo.GetTotalW());
    GiniH.AddDat(NId, FixedMemorykWEgo.GetGiniCoefficient());
    
    // Need to implement a different efficient subgraphing class for centrality 
    
  }
  printf("----------------------------------------\n");
  printf("\n... DONE\n");
  
  if (collate) {
    
    printf("\nSaving %s.egonets...", BseFNm.CStr());
    const TStr AggFNm = TStr::Fmt("%s.egonets", OutFNm.CStr());
    FILE *F = fopen(AggFNm.CStr(), "wt");
    fprintf(F,"# Egonet properties on the weighted / unweighted graph with k = %d\n", k);
    fprintf(F,"# Nodes: %d\tEdges: %d\n", WGraph->GetNodes(), WGraph->GetEdges());
    fprintf(F,"# NodeId\tNodes\tEdges\tDensity\tGini\tWeights\n");
    for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
      const int NId = NI.GetId(); fprintf(F, "%d", NId);
      const int nodes = NodesH.GetDat(NId); fprintf(F, "\t%d", nodes);
      const int edges = EdgesH.GetDat(NId); fprintf(F, "\t%d", edges);
      const double density = DensityH.GetDat(NId); fprintf(F, "\t%f", density);
      const double weights = TotalWH.GetDat(NId); fprintf(F, "\t%f", weights);
      const double gini = GiniH.GetDat(NId); fprintf(F, "\t%f", gini);
      fprintf(F, "\n");
    }
    printf(" DONE\n");
    
  } else {
    
    printf("\nSaving %s.ego.nodes...", BseFNm.CStr());
    TSnap::SaveTxt(NodesH, TStr::Fmt("%s.ego.nodes", OutFNm.CStr()), TStr::Fmt("Number of nodes in node egonet with k = %d and d = %d", k, d));
    printf(" DONE\n");
    
    printf("Saving %s.ego.edges...", BseFNm.CStr());
    TSnap::SaveTxt(EdgesH, TStr::Fmt("%s.ego.edges", OutFNm.CStr()), TStr::Fmt("Number of edges in node egonet with k = %d and d = %d", k, d));
    printf(" DONE\n");
    
    printf("Saving %s.ego.density...", BseFNm.CStr());
    TSnap::SaveTxt(DensityH, TStr::Fmt("%s.ego.density", OutFNm.CStr()), TStr::Fmt("Edge density in node egonet with k = %d and d = %d", k, d));
    printf(" DONE\n");
    
    printf("Saving %s.ego.weights...", BseFNm.CStr());
    TSnap::SaveTxt(TotalWH, TStr::Fmt("%s.ego.weights", OutFNm.CStr()), TStr::Fmt("Total weight with k = %d and d = %d", k, d));
    printf(" DONE\n");
    
    printf("Saving %s.ego.gini...", BseFNm.CStr());
    TSnap::SaveTxt(GiniH, TStr::Fmt("%s.ego.gini", OutFNm.CStr()), TStr::Fmt("Egonet GINI coefficient with k = %d and d = %d (edges into and out of the egonet are NOT considered)", k, d));
    printf(" DONE\n");
    
  }
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}