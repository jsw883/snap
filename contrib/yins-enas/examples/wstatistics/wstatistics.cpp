#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Weighted graph statistics. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges with edge weights)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  // Declare variables
  int nodes, edges;
  double TotalW, AvW;
  double AvWDeg;
  double MxWInDeg, MxWOutDeg, MxWDeg;
  double WAvDirClustCoeff;
  
  TStrFltH WStatsV;
  TIntV NIdV;
  TIntFltH NIdWClustCoeffH;
  
  TStr Name;

  WGraph->GetNIdV(NIdV);
  
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
  
  printf("Computing weighted, directed average clustering coefficients...");
  WAvDirClustCoeff = TSnap::GetWAvDirLocalClustCoeff(WGraph, NIdV, NIdWClustCoeffH);
  WStatsV.AddDat("WAvDirClustCoeff", WAvDirClustCoeff);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  printf("\nStatistics summary\n------------------\n");
  printf("Nodes: %d\n", nodes);
  printf("Edges: %d\n", edges);
  printf("TotalW: %e\n", TotalW);
  printf("AvW: %e\n", AvW);
  printf("AvWDeg: %e\n", AvWDeg);
  printf("MxWInDeg: %e\n", MxWInDeg);
  printf("MxWOutDeg: %e\n", MxWOutDeg);
  printf("MxWDeg: %e\n", MxWDeg);
  printf("WAvDirClustCoeff: %f\n", WAvDirClustCoeff);
  
  Name = TStr::Fmt("%s.WStatsV", OutFNm.CStr());
  printf("\nSaving %s...", Name.CStr());
  TSnap::SaveTxt(WStatsV, Name.CStr(), "Weighted graph statistics summary", "Stat", "Value");
  printf(" DONE\n");
  
  Name = TStr::Fmt("%s.NIdWClustCoeffH", OutFNm.CStr());
  printf("\nSaving %s...", Name.CStr());
  TSnap::SaveTxt(NIdWClustCoeffH, Name.CStr(), "Weighted local clustering coefficients", "NId", "WDirLocalClustCoeff");
  printf(" DONE\n");
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}