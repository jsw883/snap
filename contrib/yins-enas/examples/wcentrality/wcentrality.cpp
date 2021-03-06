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
  const double r = Env.GetIfArgPrefixFlt("-r:", 0.5, "ratio of endogenous parameter to dominant eigenvalue for alpha centrality");
  const double eps = Env.GetIfArgPrefixFlt("--eps:", 1.0e-4, "precision for power method convergence");
  const int iters = Env.GetIfArgPrefixInt("--iters:", 1.0e+3, "maximum number of iterations");
  const bool collate = Env.GetIfArgPrefixBool("--collate:", false, "collate properties into matrix: T / F");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  // Declare variables
  TIntFltVH FirstWDegVH;
  TIntFltVH kWInDegVH, kWOutDegVH, kWDegVH;
  TIntFltH ExoH;
  TIntFltVH WDegCentrVH, WEigCentrVH, WAlphaCentrVH;
  TFltV AlphaV;
  TFltV WEigDiffV, WEigV, WAlphaDiffV;
  TIntFltH WPgRH;
  double WPgRDiff;
  TFltWNGraph::TNodeI NI;
  TFltV::TIter VI;
  
  TStr Name;

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
  
  TSnap::printCategoryVHSummary(FirstWDegVH, "\nFirstWDegVH\n----------");
  
  // 1:k degree distributions
  
  if (k > 1) {

    printf("\nComputing weighted egonet degrees for k = 1 to %d (in / out / undirected)\n", k);
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
    
    TSnap::printCategoryVHSummary(kWInDegVH, "\nkWInDegVH\n--------");
    TSnap::printCategoryVHSummary(kWOutDegVH, "\nkWOutDegVH\n---------");
    TSnap::printCategoryVHSummary(kWDegVH, "\nkWDegVH\n------");
    
  }

  // Degree centrality
  
  printf("\nComputing weighted degree centrality...");
  TSnap::GetWDegreeCentrVH(WGraph, WDegCentrVH, 0.5);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printCategoryVHSummary(WDegCentrVH, "\nWDegCentrVH\n----------");
  
  // Eigenvector centrality
  
  printf("\nComputing weighted eigenvector centrality...");
  WEigDiffV = TSnap::GetWEigenVectorCentrVH<TFlt>(WGraph, WEigCentrVH, WEigV, eps, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());

  printf("\nConvergence (in / out / undirected)\n");
  printf("%e%s\n", (double) WEigDiffV[0], WEigDiffV[0] < eps ? "" : " DID NOT CONVERGE");
  printf("%e%s\n", (double) WEigDiffV[1], WEigDiffV[1] < eps ? "" : " DID NOT CONVERGE");
  printf("%e%s\n", (double) WEigDiffV[2], WEigDiffV[2] < eps ? "" : " DID NOT CONVERGE");
  
  printf("\nLeading eigenvalues (in / out / undirected)\n");
  printf("%e\n", (double) WEigV[0]);
  printf("%e\n", (double) WEigV[1]);
  printf("%e\n", (double) WEigV[2]);
  
  printf("\nInverse eigenvalues (in / out / undirected)\n");
  printf("%e\n", 1.0 / WEigV[0]);
  printf("%e\n", 1.0 / WEigV[1]);
  printf("%e\n", 1.0 / WEigV[2]);
  
  TSnap::printCategoryVHSummary(WEigCentrVH, "\nWEigCentrVH\n----------");
  
  // Alpha centrality
  
  AlphaV.Add(r / WEigV[0]);
  AlphaV.Add(r / WEigV[1]);
  AlphaV.Add(r / WEigV[2]);

  printf("\nComputing alpha centrality...");
  WAlphaDiffV = TSnap::GetWAlphaCentrVH<TFlt>(WGraph, ExoH, WAlphaCentrVH, AlphaV, eps, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());

  printf("\nAlpha parameters (r: %f)\n", r);
  printf("%e\n", (double) AlphaV[0]);
  printf("%e\n", (double) AlphaV[1]);
  printf("%e\n", (double) AlphaV[2]);
  
  printf("\nConvergence (in / out / undirected)\n");
  printf("%e%s\n", (double) WAlphaDiffV[0], WAlphaDiffV[0] < eps ? "" : " DID NOT CONVERGE");
  printf("%e%s\n", (double) WAlphaDiffV[1], WAlphaDiffV[1] < eps ? "" : " DID NOT CONVERGE");
  printf("%e%s\n", (double) WAlphaDiffV[2], WAlphaDiffV[2] < eps ? "" : " DID NOT CONVERGE");
  
  if (WAlphaDiffV[0] > eps || WAlphaDiffV[1] > eps || WAlphaDiffV[2] > eps) {
    printf("\nNOTE: for alpha centrality to converge, alpha must be less than the inverse leading eigenvalue for the direction specified.\n");
  }
  
  TSnap::printCategoryVHSummary(WAlphaCentrVH, "\nWAlphaCentrVH\n----------");
  
  // PageRank centrality
  
  printf("\nComputing weighted PageRank centrality...");
  WPgRDiff = TSnap::GetWPageRank<TFlt>(WGraph, WPgRH, c, eps, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
 
  printf("\nConvergence: %e%s\n", double(WPgRDiff), WPgRDiff < eps ? "" : " DID NOT CONVERGE");
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  if (collate) {
    
    Name = TStr::Fmt("%s.WCentrCombined", OutFNm.CStr());
    printf("\nSaving %s...", Name.CStr());
    FILE *F = fopen(Name.CStr(), "wt");
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
    
    Name = TStr::Fmt("%s.WDegCentrVH", OutFNm.CStr());
    printf("\nSaving %s...", Name.CStr());
    TSnap::SaveTxt(WDegCentrVH, Name.CStr(), "Weighted degree centrality (in / out / undirected)", "NodeId", "WInDegCentr\tWOutDegCentr\tWDegCentr");
    printf(" DONE\n");
    
    Name = TStr::Fmt("%s.WEigCentrVH", OutFNm.CStr());
    printf("Saving %s...", Name.CStr());
    TSnap::SaveTxt(WEigCentrVH, Name.CStr(), "Weighted eigenvector centrality (in / out / undirected)", "NodeId", "WInEigCentr\tWOutEigCentr\tWEigCentr");
    printf(" DONE\n");
    
    Name = TStr::Fmt("%s.WAlphaCentrVH", OutFNm.CStr());
    printf("Saving %s...", Name.CStr());
    TSnap::SaveTxt(WAlphaCentrVH, Name.CStr(), TStr::Fmt("Weighted alpha centrality (in / out / undirected) with r = %f,  a = (%e, %e, %e)", r, AlphaV[0], AlphaV[1], AlphaV[2]), "NodeId", "WInAlphaCentr\tWOutAlphaCentr\tWAlphaCentr");
    printf(" DONE\n");
    
    Name = TStr::Fmt("%s.WPgRH", OutFNm.CStr());
    printf("Saving %s...", Name.CStr());
    TSnap::SaveTxt(WPgRH, Name.CStr(), "Weighted PageRank centrality (wpgr)", "NodeId", "WPageRank");
    printf(" DONE\n");
    
  }
  
  Name = TStr::Fmt("%s.FirstWDegVH", OutFNm.CStr());
  printf("Saving %s...", Name.CStr());
  TSnap::SaveTxt(FirstWDegVH, Name.CStr(), "Weighted degree distributions (in / out / undirected)", "NodeId", "WInDeg\tWOutDeg\tWDeg");
  printf(" DONE\n");
  
  if (k > 1) {

    Name = TStr::Fmt("%s.kWInDegVH", OutFNm.CStr());
    printf("Saving %s...", Name.CStr());
    TSnap::SaveTxt(kWInDegVH, Name.CStr(), TStr::Fmt("1 to %d weighted in degree distributions (kdeg.IN)", k), "NodeId", "kWInDegH");
    printf(" DONE\n");
    
    Name = TStr::Fmt("%s.kWOutDegVH", OutFNm.CStr());
    printf("Saving %s...", Name.CStr());
    TSnap::SaveTxt(kWOutDegVH, Name.CStr(), TStr::Fmt("1 to %d weighted out degree distributions (kdeg.OUT)", k), "NodeId", "kWOutDegH");
    printf(" DONE\n");
    
    Name = TStr::Fmt("%s.kWDegVH", OutFNm.CStr());
    printf("Saving %s...", Name.CStr());
    TSnap::SaveTxt(kWDegVH, Name.CStr(), TStr::Fmt("1 to %d weighted degree distributions (kdeg)", k), "NodeId", "kWDegH");
    printf(" DONE\n");
  
  }
  
  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}