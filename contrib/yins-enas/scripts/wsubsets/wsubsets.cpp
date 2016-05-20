#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Graph subsets. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "./graph", "output prefix (filename extensions added)");
  const int nodes = Env.GetIfArgPrefixInt("-n:", 100, "size of subset (default: 100, and -p is ignored)");
  const int k = Env.GetIfArgPrefixInt("-k:", 1, "number of disjoint subsets (default: 1)");
  
  TStr Name;
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  TIntV NIdV, RndNIdV;
  TRnd Rnd(0);

  WGraph->GetNIdV(NIdV);
  NIdV.Sort();
  
  for (int i = 0; i < k; i++) {
    RndNIdV.Clr();
    RndNIdV = TSnap::GetRndSubV(NIdV, nodes, Rnd);
    NIdV.Diff(RndNIdV);
    TSnap::SaveTxt(RndNIdV, TStr::Fmt("%sRndNIdV%d.TIntV", OutFNm.CStr(), i), TStr::Fmt("Random subset %d (n: %d)", i, nodes), "NodeId");
  }

  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}