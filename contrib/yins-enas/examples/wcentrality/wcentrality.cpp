#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
   
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Node centrality. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const int k = Env.GetIfArgPrefixInt("-k:", 1, "depth of weighted degree distributions (1 / 2 / ...)");
  const bool c = Env.GetIfArgPrefixBool("-c:", false, "collate centralities into matrix (T / F)");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", WGraph->GetNodes());
  printf("  edges: %d\n", WGraph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Declare variables
  TIntFltVH FirstWDegVH;
  TIntFltVH kWInDegVH, kWOutDegVH, kWDegVH;
  TIntFltVH WDegCentrVH, WEigCentrVH;
  TFltV WEigDiffV;
  TIntFltH WPgRH;
  double WPgRDiff;
  TFltWNGraph::TNodeI NI;
  TFltV::TIter VI;
  
  // CENTRALITY (computations)
  
  // Weighted first degree distributions
  
  printf("\nComputing weighted degree distributions...");
  TSnap::GetWDegVH(WGraph, FirstWDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // 1:k degree distributions
  
  printf("Computing egonet degrees for k = 1 to %d (in / out / undirected)\n", k);
  TSnap::TFixedMemorykWDeg<TFlt, TWNGraph> FixedMemorykWDeg(WGraph, k);
  printf("  ...");
  FixedMemorykWDeg.GetkWInDegSeqH(kWInDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  ...");
  FixedMemorykWDeg.GetkWOutDegSeqH(kWOutDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  ...");
  FixedMemorykWDeg.GetkWDegSeqH(kWDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Centrality measures
  
  printf("Computing weighted degree centrality...");
  TSnap::GetWDegreeCentrVH(WGraph, WDegCentrVH, 0.5);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  printf("Computing weighted eigenvector centrality...");
  WEigDiffV = TSnap::GetWEigenVectorCentrVH<TFlt>(WGraph, WEigCentrVH, 1e-4, 1000);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  convergence differences (in / out / undirected)\n");
  printf("    %f\n", double(WEigDiffV[0]));
  printf("    %f\n", double(WEigDiffV[1]));
  printf("    %f\n", double(WEigDiffV[2]));
  
  printf("Computing weighted PageRank centrality...");
  WPgRDiff = TSnap::GetWPageRank<TFlt>(WGraph, WPgRH, 0.85, 1e-4, 1000);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  convergence difference: %f\n", double(WPgRDiff));
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  if (c) {
    
    printf("\nSaving %s.wcentr...", BseFNm.CStr());
    const TStr AggFNm = TStr::Fmt("%s.wcentr", OutFNm.CStr());
    FILE *F = fopen(AggFNm.CStr(), "wt");
    fprintf(F,"# Node centrality distributions on the directed / undirected graph (as applicable)\n");
    fprintf(F,"# Nodes: %d\tEdges: %d\n", WGraph->GetNodes(), WGraph->GetEdges());
    fprintf(F,"# NodeId\tWInDegCentr\tWOutDegCentr\tWDegCentr\tWInEigCentr\tWOutEigCentr\tWEigCentr\tWPgRCentr\n");
    for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
      const int NId = NI.GetId(); fprintf(F, "%d", NId);
      const TFltV WDegCentrV = WDegCentrVH.GetDat(NId);
      for (VI = WDegCentrV.BegI(); VI < WDegCentrV.EndI(); VI++) { fprintf(F, "\t%f", VI->Val); }
      const TFltV WEigCentrV = WEigCentrVH.GetDat(NId);
      for (VI = WEigCentrV.BegI(); VI < WEigCentrV.EndI(); VI++) { fprintf(F, "\t%f", VI->Val); }
      const double WPgRCentr = WPgRH.GetDat(NId); fprintf(F, "\t%f", WPgRCentr);
      fprintf(F, "\n");
    }
    printf(" DONE\n");
    
  } else {
    
    printf("\nSaving %s.wdeg.centr...", BseFNm.CStr());
    TSnap::SaveTxt(WDegCentrVH, TStr::Fmt("%s.wdeg.centr", OutFNm.CStr()), "Weighted degree centrality (in / out / undirected)", "NodeId", "WInDegCentr\tWOutDegCentr\tWDegCentr");
    printf(" DONE\n");
    
    printf("Saving %s.weig...", BseFNm.CStr());
    TSnap::SaveTxt(WEigCentrVH, TStr::Fmt("%s.weig", OutFNm.CStr()), "Weighted eigenvector centrality (in / out / undirected)", "NodeId", "WInEigCentr\tWOutEigCentr\tWEigCentr");
    printf(" DONE\n");
    
    printf("Saving %s.wpgr...", BseFNm.CStr());
    TSnap::SaveTxt(WPgRH, TStr::Fmt("%s.wpgr", OutFNm.CStr()), "Weighted PageRank centrality (wpgr)", "NodeId", "WPageRank");
    printf(" DONE\n");
    
  }
  
  printf("Saving %s.wdeg...", BseFNm.CStr());
  TSnap::SaveTxt(FirstWDegVH, TStr::Fmt("%s.wdeg", OutFNm.CStr()), "Weighted degree distributions (in / out / undirected)", "NodeId", "WInDeg\tWOutDeg\tWDeg");
  printf(" DONE\n");
  
  printf("Saving %s.kwdeg.IN...", BseFNm.CStr());
  TSnap::SaveTxt(kWInDegVH, TStr::Fmt("%s.kwdeg.IN", OutFNm.CStr()), TStr::Fmt("1 to %d weighted in degree distributions (kdeg.IN)", k), "NodeId", "kWInDegH");
  printf(" DONE\n");
  
  printf("Saving %s.kwdeg.OUT...", BseFNm.CStr());
  TSnap::SaveTxt(kWOutDegVH, TStr::Fmt("%s.kwdeg.OUT", OutFNm.CStr()), TStr::Fmt("1 to %d weighted out degree distributions (kdeg.OUT)", k), "NodeId", "kWOutDegH");
  printf(" DONE\n");
  
  printf("Saving %s.kwdeg...", BseFNm.CStr());
  TSnap::SaveTxt(kWDegVH, TStr::Fmt("%s.kwdeg", OutFNm.CStr()), TStr::Fmt("1 to %d weighted degree distributions (kdeg)", k), "NodeId", "kWDegH");
  printf(" DONE\n");
  
  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}