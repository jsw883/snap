#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
   
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Node centrality. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges)");
  const TStr ExoFNm = Env.GetIfArgPrefixStr("-e:", "", "exogenous source of centrality (tab separated node mapping)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const int k = Env.GetIfArgPrefixInt("-k:", 1, "depth of degree traversal");
  const double c = Env.GetIfArgPrefixFlt("-c:", 0.85, "personalization parameter for PageRank centrality");
  const double a = Env.GetIfArgPrefixFlt("-a:", 1.0e-8, "endogenous parameter for alpha centrality");
  const double eps = Env.GetIfArgPrefixFlt("--eps:", 1.0e-4, "precision for power method convergence");
  const int iters = Env.GetIfArgPrefixInt("--iters:", 1.0e+3, "maximum number of iterations");
  const bool collate = Env.GetIfArgPrefixBool("--collate:", false, "collate properties into matrix: T / F");
  
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
  TIntFltH ExoH;
  TIntFltVH WDegCentrVH, WEigCentrVH, WAlphaCentrVH;
  TFltV WEigDiffV, WEigV, WAlphaDiffV;
  TIntFltH WPgRH;
  double WPgRDiff;
  TFltWNGraph::TNodeI NI;
  TFltV::TIter VI;
  
  // Initialize exogenous source of centrality
  if (ExoFNm.Empty()) { // might need to check if length == 1 instead
    for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
      ExoH.AddDat(NI.GetId(), 1);
    }
  } else {
    ExoH = TSnap::LoadTxtIntFltH(ExoFNm);
  }
  
  // CENTRALITY (computations)
  
  // Weighted first degree distributions
  
  printf("\nComputing weighted degree distributions...");
  TSnap::GetWDegVH(WGraph, FirstWDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // 1:k degree distributions
  
  printf("Computing weighted egonet degrees for k = 1 to %d (in / out / undirected)\n", k);
  TSnap::TFixedMemorykWDeg<TFlt, TWNGraph> FixedMemorykWDeg(WGraph, k);
  printf("  ...");
  FixedMemorykWDeg.GetkWInDegH(kWInDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  ...");
  FixedMemorykWDeg.GetkWOutDegH(kWOutDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  ...");
  FixedMemorykWDeg.GetkWDegH(kWDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Centrality measures
  
  printf("Computing weighted degree centrality...");
  TSnap::GetWDegreeCentrVH(WGraph, WDegCentrVH, 0.5);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  printf("\nComputing weighted eigenvector centrality...");
  WEigDiffV = TSnap::GetWEigenVectorCentrVH<TFlt>(WGraph, WEigCentrVH, WEigV, eps, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  Convergence differences, inverse eigenvalues (in / out / undirected)\n");
  printf("  %e, %e, %e%s\n", (double) WEigDiffV[0], (double) WEigV[0], 1.0 / WEigV[0], WEigDiffV[0] < eps ? "" : " DID NOT CONVERGE");
  printf("  %e, %e, %e%s\n", (double) WEigDiffV[1], (double) WEigV[1], 1.0 / WEigV[1], WEigDiffV[1] < eps ? "" : " DID NOT CONVERGE");
  printf("  %e, %e, %e%s\n", (double) WEigDiffV[2], (double) WEigV[2], 1.0 / WEigV[2], WEigDiffV[2] < eps ? "" : " DID NOT CONVERGE");
  
  printf("\nComputing weighted alpha centrality...");
  WAlphaDiffV = TSnap::GetWAlphaCentrVH<TFlt>(WGraph, ExoH, WAlphaCentrVH, a, eps, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  Convergence differences (in / out / undirected)\n");
  printf("  %e%s\n", (double) WAlphaDiffV[0], WAlphaDiffV[0] < eps ? "" : " DID NOT CONVERGE");
  printf("  %e%s\n", (double) WAlphaDiffV[1], WAlphaDiffV[1] < eps ? "" : " DID NOT CONVERGE");
  printf("  %e%s\n", (double) WAlphaDiffV[2], WAlphaDiffV[2] < eps ? "" : " DID NOT CONVERGE");
  printf("NOTE: for alpha centrality to converge, alpha must be less than the inverse leading eigenvalue!\n");
  
  printf("\nComputing weighted PageRank centrality...");
  WPgRDiff = TSnap::GetWPageRank<TFlt>(WGraph, WPgRH, c, eps, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  Convergence difference: %e%s\n", double(WPgRDiff), WPgRDiff < eps ? "" : " DID NOT CONVERGE");
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  if (collate) {
    
    printf("\nSaving %s.wcentr.combined...", BseFNm.CStr());
    const TStr CombinedFNm = TStr::Fmt("%s.wcentr.combined", OutFNm.CStr());
    FILE *F = fopen(CombinedFNm.CStr(), "wt");
    fprintf(F, "# Node centrality distributions on the directed / undirected graph (as applicable)\n");
    fprintf(F, "# Nodes: %d\tEdges: %d\n", WGraph->GetNodes(), WGraph->GetEdges());
    fprintf(F, "# NodeId\tWInDegCentr\tWOutDegCentr\tWDegCentr\tWInEigCentr\tWOutEigCentr\tWEigCentr\tWInAlphaCentr\tWOutAlphaCentr\tWAlphaCentr\tWPgRCentr\n");
    for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
      const int NId = NI.GetId(); fprintf(F, "%d", NId);
      const TFltV WDegCentrV = WDegCentrVH.GetDat(NId);
      for (VI = WDegCentrV.BegI(); VI < WDegCentrV.EndI(); VI++) { fprintf(F, "\t%e", VI->Val); }
      const TFltV WEigCentrV = WEigCentrVH.GetDat(NId);
      for (VI = WEigCentrV.BegI(); VI < WEigCentrV.EndI(); VI++) { fprintf(F, "\t%e", VI->Val); }
      const TFltV WAlphaCentrV = WAlphaCentrVH.GetDat(NId);
      for (VI = WAlphaCentrV.BegI(); VI < WAlphaCentrV.EndI(); VI++) { fprintf(F, "\t%e", VI->Val); }
      const double WPgRCentr = WPgRH.GetDat(NId); fprintf(F, "\t%e", WPgRCentr);
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
    
    printf("Saving %s.alpha.wcentrality...", BseFNm.CStr());
    TSnap::SaveTxt(WAlphaCentrVH, TStr::Fmt("%s.alpha.wcentrality", OutFNm.CStr()), TStr::Fmt("Weighted alpha centrality (in / out / undirected) with a = %f", a), "NodeId", "WInAlphaCentr\tWOutAlphaCentr\tWAlphaCentr");
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