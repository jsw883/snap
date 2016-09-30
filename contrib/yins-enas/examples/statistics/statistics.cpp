#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Graph statistics. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges with edge weights)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const bool exact = Env.GetIfArgPrefixBool("--exact:", true, "compute exact neighborhood function (exhaustive BFS): T / F");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printGraphSummary(Graph, "\nGraph\n-----");
  
  // Declare variables
  int nodes, edges;
  double AvDeg, density;
  int MxInDeg, MxOutDeg, MxDeg;
  TFltV percentiles, pvalues;
  TIntFltKdV ANF;
  double EffDiam, AppDiam, Radius;
  double AvClustCf, GlClustCf, AvDirClustCoeff;
  TStrFltH StatsV;
  TIntIntH DegH;
  TIntV NIdV;
  TIntFltH NIdClustCoeffH;

  TUInt64V NF;

  TStr Name;

  Graph->GetNIdV(NIdV);

  // STATISTICS (computations)
  
  // Computes nodes, edges, average degree, and density
  printf("\nComputing nodes, edges, average degree, and density...");
  nodes = Graph->GetNodes();
  edges = Graph->GetEdges();
  AvDeg = 2*double(edges)/double(nodes);
  density = double(edges)/(double(nodes) * (double(nodes) - 1));
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
    
    // Computes diameter (effective / approximate) and median path length exactly
    printf("\nComputing exact neighborhood function...");
    TSnap::TFixedMemoryNeighborhood<PNGraph> FixedMemoryNeighborhood(Graph);
    FixedMemoryNeighborhood.ComputeSubsetNF(NIdV, edOutDirected, NF);
    printf(" DONE: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
    
    EffDiam = TSnap::InterpolateNF(NF, 0.9);
    AppDiam = TSnap::InterpolateNF(NF, 1.0);
    Radius = TSnap::InterpolateNF(NF, 0.5);
    
  } else {
    
    // Computes approximate neighborhood function / shortest path cumulative density (hacky)
    printf("Computing approximate neighborhood function...");
    TSnap::GetAnf(Graph, ANF, -1, false, 128);
    printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
    
    // Computes diameter (effective / approximate) and median path length approximately (hacky)
    printf("Computing average path length and effective / approximate diameters...");
    percentiles.Add(0.9); percentiles.Add(1.0); percentiles.Add(0.5);
    TSnap::GetAnfEffDiam(Graph, false, percentiles, pvalues, 10, -1);
    printf(" DONE: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
    
    EffDiam = pvalues[0];
    AppDiam = pvalues[1];
    Radius = pvalues[2];
    
  }
  
  StatsV.AddDat("EffDiam", EffDiam);
  StatsV.AddDat("AppDiam", AppDiam);
  StatsV.AddDat("Radius", Radius);
  
  // CLUSTERING COEFFICIENTS
  
  // Computes average and global clustering coefficients (need to check this for method)
  printf("Computing global / average clustering coefficients...");
  GlClustCf = TSnap::GetGlClustCf(Graph);
  AvClustCf = TSnap::GetAvClustCf(Graph);
  AvDirClustCoeff = TSnap::GetAvDirLocalClustCoeff(Graph, NIdV, NIdClustCoeffH);
  StatsV.AddDat("GlClustCf", GlClustCf);
  StatsV.AddDat("AvClustCf", AvClustCf);
  StatsV.AddDat("AvDirClustCoeff", AvDirClustCoeff);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  printf("\nStatistics summary\n------------------\n");
  printf("Nodes: %d\n", nodes);
  printf("Edges: %d\n", edges);
  printf("AvDeg: %f\n", AvDeg);
  printf("Density: %f\n", density);
  printf("MxInDeg: %d\n", MxInDeg);
  printf("MxOutDeg: %d\n", MxOutDeg);
  printf("MxDeg: %d\n", MxDeg);
  printf("EffDiam: %f\n", EffDiam);
  printf("AppDiam: %f\n", AppDiam);
  printf("Radius: %f\n", Radius);
  printf("GlClustCf: %f\n", GlClustCf);
  printf("AvClustCf: %f\n", AvClustCf);
  printf("AvDirClustCoeff: %f\n", AvDirClustCoeff);
  
  Name = TStr::Fmt("%s.StatsV", OutFNm.CStr());
  printf("\nSaving %s...", Name.CStr());
  TSnap::SaveTxt(StatsV, Name.CStr(), "Graph statistics summary", "Stat", "Value");
  printf(" DONE\n");
  
  Name = TStr::Fmt("%s.NIdClustCoeffH", OutFNm.CStr());
  printf("\nSaving %s...", Name.CStr());
  TSnap::SaveTxt(NIdClustCoeffH, Name.CStr(), "Weighted local clustering coefficients", "NId", "WDirLocalClustCoeff");
  printf(" DONE\n");
  
  if (exact) {
    
    Name = TStr::Fmt("%s.NF", OutFNm.CStr());
    printf("\nSaving %s...", Name.CStr());
    TSnap::SaveTxt(NF, Name.CStr(), "Exact neighbourhood function / shortest path cumulative density (hop)");
    printf(" DONE\n");
    
    TSnap::printDataV(NF, true, "\nNF\n--");
    
  } else {
  
    Name = TStr::Fmt("%s.ANF", OutFNm.CStr());
    printf("\nSaving %s...", Name.CStr());
    TSnap::SaveTxtTIntFltKdV(ANF, Name.CStr(), "Approximate neighbourhood function / shortest path cumulative density (hop)");
    printf(" DONE\n");
    
    TSnap::printDataV(NF, true, "\nANF\n---");  
  }
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}