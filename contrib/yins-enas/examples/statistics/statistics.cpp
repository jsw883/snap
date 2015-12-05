#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Graph statistics. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", " input network (tab separated list of edges with edge weights)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const bool exact = Env.GetIfArgPrefixBool("--exact:", true, "compute exact neighborhood function (exhaustive BFS): T / F");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", Graph->GetNodes());
  printf("  edges: %d\n", Graph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Declare variables
  int nodes, edges;
  double AvDeg, density;
  int MxInDeg, MxOutDeg, MxDeg;
  TFltV percentiles, pvalues;
  TIntFltKdV DistNbrsV;
  double EffDiam, AppDiam, AvPath;
  double AvClustCf, GlobClustCf;
  TStrFltH StatsV;
  
  TIntV NIdV, NF;
  
  // STATISTICS (computations)
  
  // Computes nodes, edges, average degree, and density
  printf("\nComputing nodes, edges, average degree, and density...");
  nodes = Graph->GetNodes();
  edges = Graph->GetEdges();
  AvDeg = 2*double(edges)/double(nodes);
  density = double(edges)/(pow(double(nodes), 2) - 1);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  StatsV.AddDat("nodes", nodes);
  StatsV.AddDat("edges", edges);
  StatsV.AddDat("AvDeg", AvDeg);
  StatsV.AddDat("density", density);
  
  // Computes maximum degree (in / out / directed)
  printf("Computing maximum degrees...");
  MxInDeg = TSnap::GetMxInDeg(Graph);
  MxOutDeg = TSnap::GetMxOutDeg(Graph);
  MxDeg = TSnap::GetMxDeg(Graph);
  StatsV.AddDat("MxInDeg", MxInDeg);
  StatsV.AddDat("MxOutDeg", MxOutDeg);
  StatsV.AddDat("MxDeg", MxDeg);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // NEIGHBORHOOD FUNCTION
  
  if (exact) {
    
    Graph->GetNIdV(NIdV);
    
    printf("\nComputing exact neighborhood function...");
    TSnap::TFixedMemoryNeighborhood<PNGraph> FixedMemoryNeighborhood(Graph);
    FixedMemoryNeighborhood.ComputeSubsetExactNF(NIdV, edOutDirected, NF);
    printf(" DONE: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
    
      // printf("NF:\n");
      // TIntV::TIter VI;
      // int depth = 0;
      // for (VI = NF.BegI(), depth = 0; VI < NF.EndI(); VI++, depth++) {
      //   printf("%d: %d\n", depth, VI->Val);
      // }
    
    EffDiam = TSnap::InterpolateNF(NF, 0.9);
    AppDiam = TSnap::InterpolateNF(NF, 1.0);
    AvPath = TSnap::InterpolateNF(NF, 0.5);
    
  } else {
    
    // Computes approximate neighborhood function / shortest path cumulative density (hacky)
    printf("Computing approximate neighborhood function...");
    TSnap::GetAnf(Graph, DistNbrsV, -1, false, 128);
    printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
    
    // Computes diameter (effective / approximate) and average path length derived (hacky)
    printf("Computing average path length and effective / approximate diameters...");
    percentiles.Add(0.9); percentiles.Add(1.0); percentiles.Add(0.5);
    TSnap::GetAnfEffDiam(Graph, false, percentiles, pvalues, 10, -1);
    EffDiam = pvalues[0];
    AppDiam = pvalues[1];
    AvPath = pvalues[2];
    
  }
  
  StatsV.AddDat("EffDiam", EffDiam);
  StatsV.AddDat("AppDiam", AppDiam);
  StatsV.AddDat("AvPath", AvPath);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // CLUSTERING COEFFICIENTS
  
  // Computes average and global clustering coefficients (need to check this for method)
  printf("Computing average / global clustering coefficients...");
  AvClustCf = TSnap::GetAvClustCf(Graph);
  GlobClustCf = TSnap::GetGlobClustCf(Graph);
  StatsV.AddDat("AvClustCf", AvClustCf);
  StatsV.AddDat("GlobClustCf", GlobClustCf);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  printf("\nStatistics summary:\n");
  printf("  nodes: %d edges: %d\n", nodes, edges);
  printf("  average degree: %f\n", AvDeg);
  printf("  density: %f\n", density);
  printf("  maximum degree (in / out / undirected): %d %d %d\n", MxInDeg, MxOutDeg, MxDeg);
  printf("  diameter (effective / approximate): %f %f\n", EffDiam, AppDiam);
  printf("  average path length: %f\n", AvPath);
  printf("  average clustering coefficient: %f\n", AvClustCf);
  printf("  global clustering coefficient: %f\n", GlobClustCf);
  
  printf("\nSaving %s.summary...", BseFNm.CStr());
  TSnap::SaveTxt(StatsV, TStr::Fmt("%s.summary", OutFNm.CStr()), "Graph statistics summary", "Stat", "Value");
  printf(" DONE\n");
  
  if (exact) {
    
    printf("\nSaving %s.hop.NF...", BseFNm.CStr());
    TSnap::SaveTxt(NF, TStr::Fmt("%s.hop.NF", OutFNm.CStr()), "Exact neighbourhood function / shortest path cumulative density (hop)");
    printf(" DONE\n");
    
  } else {
  
    printf("\nSaving %s.hop.ANF...", BseFNm.CStr());
    TSnap::SaveTxtTIntFltKdV(DistNbrsV, TStr::Fmt("%s.hop.ANF", OutFNm.CStr()), "Approximate neighbourhood function / shortest path cumulative density (hop)");
    printf(" DONE\n");
      
  }
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}