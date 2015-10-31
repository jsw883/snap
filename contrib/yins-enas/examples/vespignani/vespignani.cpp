#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
   
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Backbone extractor (Vespignani). build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output network name (-backbone-(alpha).snap added)");
  const float alpha = Env.GetIfArgPrefixFlt("-a:", 0.01, "level of significance alpha");

  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", WGraph->GetNodes());
  printf("  edges: %d\n", WGraph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());

  // Declare variables

  // Hashtables for source and destination weight sums
  TIntFltH OutWDegH, InWDegH;

  // Hashtables for binary source and destination degrees
  TIntIntH OutDegH, InDegH;

  
  // Part A: Get degrees, weighted and unweighted

  printf("\nGetting weighted source and destination degrees...");
  TSnap::GetWInDegH(WGraph, InWDegH);
  TSnap::GetWOutDegH(WGraph, OutWDegH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());

  printf("\nGetting binary source and destination degrees...");
  TSnap::GetInDegSeqH(WGraph, InDegH);
  TSnap::GetOutDegSeqH(WGraph, OutDegH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Part B: Disparity Filter

  printf("\nApplying the disparity filter...");
  TSnap::GetVespignaniPruned(WGraph, InWDegH, OutWDegH, InDegH, OutDegH, alpha);
  printf(" DONE")
  printf("\nPruned graph:\n");
  printf("\n  nodes: %d", WGraph->GetNodes());
  printf("\n  edges: %d", WGraph->GetEdges());

  printf("\n  DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  // OUTPUTTING 

  
  printf("\nSaving...");
  TSnap::SaveFltWEdgeList(WGraph, TStr::Fmt("%s-backbone-%f.snap", OutFNm.CStr(), alpha), "");
  printf(" DONE\n");

  
  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}