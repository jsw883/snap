#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Weighted graph statistics. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", WGraph->GetNodes());
  printf("  edges: %d\n", WGraph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Declare variables
  int nodes, edges;
  double TotalW, AvW;
  double AvWDeg;
  double MxWInDeg, MxWOutDeg, MxWDeg;
  // double AvClustCf, GlobClustCf;
  TStrFltH WStatsV;
  
  // WEIGHTED STATISTICS (computations)
  
  // Computes nodes, edges, total weight, average weight, and average weighted degree
  printf("\nComputing total weight, average weight, and average weighted degree...");
  nodes = WGraph->GetNodes();
  edges = WGraph->GetEdges();
  TotalW = WGraph->GetTotalW();
  AvW = TotalW/double(edges);
  AvWDeg = 2*TotalW/double(nodes);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  WStatsV.AddDat("nodes", nodes);
  WStatsV.AddDat("edges", edges);
  WStatsV.AddDat("TotalW", TotalW);
  WStatsV.AddDat("AvW", AvW);
  WStatsV.AddDat("AvWDeg", AvWDeg);
  
  // Computes weighted average degree, maximum weighted degrees (in / out / undirected)
  printf("\nComputing maximum weighted degrees...");
  MxWInDeg = TSnap::GetMxWInDeg(WGraph);
  MxWOutDeg = TSnap::GetMxWOutDeg(WGraph);
  MxWDeg = TSnap::GetMxWDeg(WGraph);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  WStatsV.AddDat("MxWInDeg", MxWInDeg);
  WStatsV.AddDat("MxWOutDeg", MxWOutDeg);
  WStatsV.AddDat("MxWDeg", MxWDeg);
  
  // Computes weighted average and global clustering coefficients (need to check this for method)
  // TODO: Implemented generalised, weighted measures (Tore Opsahl) 
  
  // printf("Computing average / global clustering coefficients...");
  // AvClustCf = TSnap::GetAvClustCf(WGraph);
  // GlobClustCf = TSnap::GetGlobClustCf(WGraph);
  // StatsV.AddDat("AvClustCf", AvClustCf);
  // StatsV.AddDat("GlobClustCf", GlobClustCf);
  // printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  printf("\nStatistics summary:\n");
  printf("  nodes: %d edges: %d\n", nodes, edges);
  printf("  total weight: %f\n", TotalW);
  printf("  average weight: %f\n", AvW);
  printf("  average weighted degree: %f\n", AvWDeg);
  printf("  maximum weighted degree (in / out / undirected): %f %f %f\n", MxWInDeg, MxWOutDeg, MxWDeg);
  // printf("  average clustering coefficient: %f\n", AvClustCf);
  // printf("  global clustering coefficient: %f\n", GlobClustCf);
  
  printf("\nSaving %s.wsummary...", BseFNm.CStr());
  TSnap::SaveTxt(WStatsV, TStr::Fmt("%s.wsummary", OutFNm.CStr()), "Weighted graph statistics summary", "Stat", "Value");
  printf(" DONE\n");
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}