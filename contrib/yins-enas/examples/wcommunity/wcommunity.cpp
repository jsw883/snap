#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
   
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Node centrality. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges with edge weights)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const double eps = Env.GetIfArgPrefixFlt("--eps:", 1.0e-5, "minimum quality improvement threshold");
  const double moves = Env.GetIfArgPrefixFlt("--moves:", 1.0e-2, "minimum number of moves (relative)");
  const int iters = Env.GetIfArgPrefixInt("--iters:", 1.0e+4, "maximum number of iterations");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  // Declare variables
  TIntIntVH NIdCmtyVH;
  double LouvainQ;
  
  // COMMUNITY
  
  // Louvain method (modularity objective)
  
  printf("\nLouvain method (weighted)...");
  LouvainQ = TSnap::LouvainMethod<TSnap::ModularityCommunity<TFlt>, TFlt>(WGraph, NIdCmtyVH, edUnDirected, eps, moves, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  quality: %f\n", LouvainQ);
  
  TSnap::CmtyHierarchySummary(NIdCmtyVH, 1, -1, "\nLouvain hierarchy\n-----------------");
  
  printf("Saving %s.louvain.modularity...", BseFNm.CStr());
  TSnap::SaveTxt(NIdCmtyVH, TStr::Fmt("%s.louvain.modularity", OutFNm.CStr()), "Louvain modularity community hierarchy (weighted)", "NodeId", "CmtyV");
  printf(" DONE\n");
  
  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}