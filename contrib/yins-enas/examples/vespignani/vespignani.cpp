#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
   
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Backbone extractor (Vespignani). build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (alpha value and filename extensions added)");
  const float alpha = Env.GetIfArgPrefixFlt("-a:", 0.01, "alpha significance level threshold");

  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", WGraph->GetNodes());
  printf("  edges: %d\n", WGraph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());

  // VESPIGNANI METHOD

  // Apply the disparity filter Vespignani method
  printf("\nApplying the disparity filter vespignani method...");
  TSnap::FilterEdgesVespignani<TFlt, TWNGraph>(WGraph, alpha);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("\nPruned graph:");
  printf("\n  nodes: %d", WGraph->GetNodes());
  printf("\n  edges: %d\n", WGraph->GetEdges());

  // OUTPUTTING 
  
  printf("\nSaving %s-%f.snap...", OutFNm.CStr(), alpha);
  TSnap::SaveFltWEdgeList(WGraph, TStr::Fmt("%s-%f.snap", OutFNm.CStr(), alpha), "");
  printf(" DONE\n");

  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}