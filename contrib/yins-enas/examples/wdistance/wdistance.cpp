#include "stdafx.h"

void ComputeWD(const PFltWNGraph& WGraph, const TIntV& SrcNIdV, const TIntV& DstNIdV, const TEdgeDir& d, const int& k, const double& tol, const TStr& OutFNm, const TStr& BseFNm, const TStr& SrcNm, const TStr& DstNm, const TExeTm& ExeTm) {
  
  // Declare variables
  TUInt64V NF;
  TIntFltVH WDVH;
  TIntV::TIter VI;
  
  TStr Name;
  
  // SUBSET NEIGHBORHOOD FUNCTION
  
  printf("\nComputing weighted distances from %s to %s ...", SrcNm.CStr(), DstNm.CStr());
  TSnap::TFixedMemoryWD<PFltWNGraph> FixedMemoryWD(WGraph, DstNIdV, k, tol);
  FixedMemoryWD.ComputeSubsetWDVH(SrcNIdV, d, WDVH);
  printf(" DONE: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printDataVHSummary(WDVH, "WDVH\n----");
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  TIntFltVH::TIter HI;
  
  Name = TStr::Fmt("%s.%s.%s.WDVH", OutFNm.CStr(), SrcNm.CStr(), DstNm.CStr());
  printf("\nSaving %s...", Name.CStr());
  FILE *F = fopen(Name.CStr(), "wt");  
  fprintf(F, "# Weighted distances from %s to %s (%d depth limited, %f tolerance limited)\n", SrcNm.CStr(), DstNm.CStr(), k, tol);
  fprintf(F, "# SrcNIdV.Len(): %d\tDstNIdV.Len(): %d\t\n", SrcNIdV.Len(), DstNIdV.Len());
  fprintf(F, "# SrcNId\tDstNId\tWD\n");
  for (HI = WDVH.BegI(); HI < WDVH.EndI(); HI++) {
    const TFltV& WDV = HI.GetDat();
    for (int i = 0; i < DstNIdV.Len(); i++) {
      fprintf(F, "%d\t%d\t%f\n", (int) HI.GetKey(), (int) DstNIdV[i], (double) WDV[i]);
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
  const TEdgeDir d = (TEdgeDir) Env.GetIfArgPrefixInt("--dir:", 3, "direction of traversal: in = 1, out = 2, undected = 3");
  const bool exhaustive = Env.GetIfArgPrefixBool("--exhaustive:", false, "compute for every node (overrides -s, -d): T / F");
  
  const TStr SrcNm = SrcNIdVFNm.RightOfLast('/').LeftOfLast('.');
  const TStr DstNm = DstNIdVFNm.RightOfLast('/').LeftOfLast('.');
  const TStr BseFNm = OutFNm.RightOfLast('/');
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", WGraph->GetNodes());
  printf("  edges: %d\n", WGraph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Declare variables
  TIntV SrcNIdV, DstNIdV, NIdV, RndNIdV;
  TRnd Rnd(0);
  
  // Load subset nodes and compute disjoint random subset of nodes (same size) 
  SrcNIdV = TSnap::LoadTxtIntV(SrcNIdVFNm);
  DstNIdV = TSnap::LoadTxtIntV(DstNIdVFNm);
  
  // SUBSET DIAMETER AND NODE COUNTS
  
  if (exhaustive) {
    // EXHAUSTIVE
    WGraph->GetNIdV(NIdV);
    ComputeWD(WGraph, NIdV, NIdV, d, k, tol, OutFNm, BseFNm, TStr("exhaustive"), TStr("exhaustive"), ExeTm);
  } else {
    // SUBSET
    ComputeWD(WGraph, SrcNIdV, DstNIdV, d, k, tol, OutFNm, BseFNm, SrcNm, DstNm, ExeTm);
  }
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}
