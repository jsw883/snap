#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Egonet characteristics. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
   
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges with edge weights)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr OutDir = OutFNm.LeftOfLast('/');
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const int n = Env.GetIfArgPrefixInt("-n:", 10, "number of egos to traverse");
  const int k = Env.GetIfArgPrefixInt("-k:", 1, "depth of ego traversal");
  const TEdgeDir d = (TEdgeDir) Env.GetIfArgPrefixInt("-d:", 3, "direction of ego traversal: in = 1, out = 2, undirected = 3");
  
  const TStr EgoNIdVFNm = Env.GetIfArgPrefixStr("--egov:", "", "subset of nodes to compute egonets (overrides -n)");

  const TStr measure = Env.GetIfArgPrefixStr("--measure:", "degree", "measure used to order nodes");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  // Declare variables

  PFltWNGraph WSubGraph;

  TIntIntH NodesH, EdgesH;
  TIntFltH TotalWH, DensityH, GiniH;
  
  TIntIntH DegH;
  TIntFltH WDegH;

  TIntV NIdV, EgoNIdV, SubNIdV;
  TIntV::TIter NI;
  
  TStr Name;

  // EGONETS

  if (!EgoNIdVFNm.Empty()) {
    Name = EgoNIdVFNm.RightOfLast('/');
    NIdV = TSnap::LoadTxtIntV(EgoNIdVFNm);
  } else {
    if (measure == "degree") {
      TSnap::GetDegH(WGraph, DegH);
      DegH.SortByDat(false);
      for (int i = 0; i < n; i++) {
        NIdV.Add(DegH.GetKey(i));
      }
      Name = "degree";
    } else if (measure == "wdegree") {
      TSnap::GetWDegH(WGraph, WDegH);
      WDegH.SortByDat(false);
      for (int i = 0; i < n; i++) {
        NIdV.Add(WDegH.GetKey(i));
      }
      Name = "degree";
    } else {
      IAssertR(false, "Measure must be one of degree or wdegree.");
    }
  }

  // Loop over egonets (node iterator)
  
  Progress progress(ExeTm, NIdV.Len(), 5, "Computing egonets");
  progress.start();
  for (NI = NIdV.BegI(); NI < NIdV.EndI(); NI++) {
    const int NId = NI->Val;
    
    // Compute egonet (computationally expensive)
    
    TSnap::TFixedMemorykEgo<PFltWNGraph> FixedMemorykEgo(WGraph, k);
    FixedMemorykEgo.ComputeEgonet(NId, d);
    
    EgoNIdV = FixedMemorykEgo.GetNIdV();
    EgoNIdV.Sort();
    SubNIdV.Union(EgoNIdV);

    progress++;
  }
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  WSubGraph = TSnap::GetSubGraph(WGraph, SubNIdV);
  TSnap::SaveFltWEdgeList(WSubGraph, TStr::Fmt("%s/data.snap", OutDir.CStr()), TStr::Fmt("Egonets: %s", Name));

  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}