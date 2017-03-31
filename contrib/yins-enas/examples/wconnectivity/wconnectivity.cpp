#include "stdafx.h"

void ComputeWD(const PFltWNGraph& WGraph, const TIntV& SrcNIdV, const TIntV& DstNIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, const bool& preNormalized, const int& k, const double& tol, const TStr& OutFNm, const TStr& BseFNm, const TStr& SrcNm, const TStr& DstNm, const TExeTm& ExeTm) {
  
  // Declare variables
  TIntV::TIter VI;
  TStr Name;
  THash<TIntPr, TIntH> PathsHH;
  
  // SUBSET NEIGHBORHOOD FUNCTION
  
  printf("\nComputing weighted distances from %s to %s ...", SrcNm.CStr(), DstNm.CStr());
  TSnap::TFixedMemoryWeightedNeighborhood<PFltWNGraph> FixedMemoryWeightedNeighborhood(WGraph, DstNIdV, k, tol, preNormalized);
  FixedMemoryWeightedNeighborhood.ComputeSubsetConnectivity(SrcNIdV, dir, SkipNIdS, PathsHH);
  printf(" DONE: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  THash<TIntPr, TIntH>::TIter HI;
  TIntH::TIter PI;

  Name = TStr::Fmt("%s.%s.%s.PathsHH", OutFNm.CStr(), SrcNm.CStr(), DstNm.CStr());
  printf("\nSaving %s...", Name.CStr());
  FILE *F = fopen(Name.CStr(), "wt");
  fprintf(F, "# Weighted connectivity from %s to %s (dir: %d, k: %d, tol: %f)\n", SrcNm.CStr(), DstNm.CStr(), dir, k, tol);
  fprintf(F, "# SrcNIdV.Len(): %d\tDstNIdV.Len(): %d\t\n", SrcNIdV.Len(), DstNIdV.Len());
  fprintf(F, "# SrcNId\tDstNId\tk\tN\n");
  for (HI = PathsHH.BegI(); HI < PathsHH.EndI(); HI++) {
    const TIntH& PathsH = HI.GetDat();
    for (PI = PathsH.BegI(); PI < PathsH.EndI(); PI++) {
      fprintf(F, "%d\t%d\t%d\t%d\n", (int) HI.GetKey().Val1, (int) HI.GetKey().Val2, (int) PI.GetKey(), (int) PI.GetDat());
    }
  }
  printf(" DONE\n");

  // Name = TStr::Fmt("%s.%s.%s.WDVH...", BseFNm.CStr(), SrcNm.CStr(), DstNm.CStr());
  // printf("\nSaving %s...", Name.CStr());
  // TSnap::SaveTxt(WBVH, Name, TStr::Fmt("# Weighted distance from %s to %s (%d depth limited, %f tolerance limited)\n", SrcNm.CStr(), DstNm.CStr(), k, tol));
  // printf(" DONE\n");
  
}

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Weighted graph subset distances. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges)");
  const TStr SrcNIdVFNm = Env.GetIfArgPrefixStr("-s:", "", "source subset of nodes (new line separated)");
  const TStr DstNIdVFNm = Env.GetIfArgPrefixStr("-d:", "", "destination subset of nodes (new line separated)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const int k = Env.GetIfArgPrefixInt("-k:", 10, "depth limit for BFS");
  const double tol = Env.GetIfArgPrefixFlt("--tol:", 1e-3, "weighted distance limit for BFS");
  const TEdgeDir dir = (TEdgeDir) Env.GetIfArgPrefixInt("--dir:", 3, "direction of traversal: in = 1, out = 2, undected = 3");
  const bool exclude = Env.GetIfArgPrefixBool("--exclude:", true, "exclude other source nodes from BFS");
  const bool preNormalized = Env.GetIfArgPrefixBool("--normalized:", false, "edge weights are pre-normalized");
  const bool exhaustive = Env.GetIfArgPrefixBool("--exhaustive:", false, "compute for every node (overrides -s, -d): T / F");
  
  const TStr SrcNm = SrcNIdVFNm.RightOfLast('/').LeftOfLast('.');
  const TStr DstNm = DstNIdVFNm.RightOfLast('/').LeftOfLast('.');
  const TStr BseFNm = OutFNm.RightOfLast('/');
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  // Declare variables
  TIntV SrcNIdV, DstNIdV, NIdV, RndNIdV;
  TRnd Rnd(0);
  TIntSet SkipNIdS;

  // Load subset nodes and compute disjoint random subset of nodes (same size) 
  SrcNIdV = TSnap::LoadTxtIntV(SrcNIdVFNm);
  DstNIdV = TSnap::LoadTxtIntV(DstNIdVFNm);
  
  if (exclude) {
    SkipNIdS.AddKeyV(SrcNIdV);
  }

  // SUBSET DIAMETER AND NODE COUNTS
  
  if (exhaustive) {
    // EXHAUSTIVE
    WGraph->GetNIdV(NIdV);
    ComputeWD(WGraph, NIdV, NIdV, dir, SkipNIdS, preNormalized, k, tol, OutFNm, BseFNm, TStr("exhaustive"), TStr("exhaustive"), ExeTm);
  } else {
    // SUBSET
    ComputeWD(WGraph, SrcNIdV, DstNIdV, dir, SkipNIdS, preNormalized, k, tol, OutFNm, BseFNm, SrcNm, DstNm, ExeTm);
  }
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}
