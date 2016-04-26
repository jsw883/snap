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
  const double r = Env.GetIfArgPrefixFlt("-r:", 1, "ratio of endogenous parameter to dominant eigenvalue for alpha centrality");
  const double eps = Env.GetIfArgPrefixFlt("--eps:", 1.0e-4, "precision for power method convergence");
  const int iters = Env.GetIfArgPrefixInt("--iters:", 1.0e+3, "maximum number of iterations");
  const bool collate = Env.GetIfArgPrefixBool("--collate:", false, "collate properties into matrix: T / F");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", Graph->GetNodes());
  printf("  edges: %d\n", Graph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Declare variables
  TIntIntVH FirstDegVH;
  TIntIntVH kInDegVH, kOutDegVH, kDegVH;
  TIntFltH ExoH;
  TIntFltVH DegCentrVH, EigCentrVH, AlphaCentrVH;
  TFltV EigDiffV, EigV, AlphaDiffV;
  TIntFltH PgRH;
  double PgRDiff;
  TNGraph::TNodeI NI;
  TFltV::TIter VI;
  
  // Initialize exogenous source of centrality
  if (ExoFNm.Empty()) { // might need to check if length == 1 instead
    for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
      ExoH.AddDat(NI.GetId(), 1);
    }
  } else {
    ExoH = TSnap::LoadTxtIntFltH(ExoFNm);
  }
  
  // CENTRALITY (computations)
  
  // First degree distributions
  
  printf("\nComputing degree distributions...");
  TSnap::GetDegVH(Graph, FirstDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printCategoryVHSummary(FirstDegVH, "FirstDegVH\n----------");
  
  // 1:k degree distributions
  
  printf("\nComputing egonet degrees for k = 1 to %d (in / out / undirected)\n", k);
  TSnap::TFixedMemorykDeg<PNGraph> FixedMemorykDeg(Graph, k);
  printf("  ...");
  FixedMemorykDeg.GetkInDegH(kInDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  ...");
  FixedMemorykDeg.GetkOutDegH(kOutDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  ...");
  FixedMemorykDeg.GetkDegH(kDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printCategoryVHSummary(kInDegVH, "kInDegVH\n--------");
  TSnap::printCategoryVHSummary(kOutDegVH, "kOutDegVH\n---------");
  TSnap::printCategoryVHSummary(kDegVH, "kDegVH\n------");
  
  // Degree centrality
  
  printf("\nComputing degree centrality...");
  TSnap::GetDegreeCentrVH(Graph, DegCentrVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printCategoryVHSummary(DegCentrVH, "DegCentrVH\n----------");
  
  // Eigenvector centrality
  
  printf("\nComputing eigenvector centrality...");
  EigDiffV = TSnap::GetEigenVectorCentrVH(Graph, EigCentrVH, EigV, eps, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  printf("\nConvergence (in / out / undirected)\n");
  printf("%e%s\n", (double) EigDiffV[0], EigDiffV[0] < eps ? "" : " DID NOT CONVERGE");
  printf("%e%s\n", (double) EigDiffV[1], EigDiffV[1] < eps ? "" : " DID NOT CONVERGE");
  printf("%e%s\n", (double) EigDiffV[2], EigDiffV[2] < eps ? "" : " DID NOT CONVERGE");
  
  printf("\nLeading eigenvalues (in / out / undirected)\n");
  printf("%e\n", (double) EigV[0]);
  printf("%e\n", (double) EigV[1]);
  printf("%e\n", (double) EigV[2]);
  
  printf("\nInverse eigenvalues (in / out / undirected)\n");
  printf("%e\n", 1.0 / EigV[0]);
  printf("%e\n", 1.0 / EigV[1]);
  printf("%e\n", 1.0 / EigV[2]);
  
  TSnap::printCategoryVHSummary(EigCentrVH, "EigCentrVH\n----------");
  
  // Alpha centrality
  
  double alpha = r / EigV[0];
  
  printf("\nComputing alpha centrality (alpha: %e)...", alpha);
  AlphaDiffV = TSnap::GetAlphaCentrVH(Graph, ExoH, AlphaCentrVH, alpha, eps, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  printf("\nConvergence (in / out / undirected)\n");
  printf("%e%s\n", (double) AlphaDiffV[0], AlphaDiffV[0] < eps ? "" : " DID NOT CONVERGE");
  printf("%e%s\n", (double) AlphaDiffV[1], AlphaDiffV[1] < eps ? "" : " DID NOT CONVERGE");
  printf("%e%s\n", (double) AlphaDiffV[2], AlphaDiffV[2] < eps ? "" : " DID NOT CONVERGE");
  
  if (AlphaDiffV[0] > eps || AlphaDiffV[1] > eps || AlphaDiffV[2] > eps) {
    printf("\nNOTE: for alpha centrality to converge, alpha must be less than the inverse leading eigenvalue for the direction specified.\n");
  }
  
  TSnap::printCategoryVHSummary(AlphaCentrVH, "AlphaCentrVH\n----------");
  
  // PageRank centrality
  
  printf("\nComputing PageRank centrality...");
  PgRDiff = TSnap::GetPageRankNew(Graph, PgRH, c, eps, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  printf("\nConvergence: %e%s\n", double(PgRDiff), PgRDiff < eps ? "" : " DID NOT CONVERGE");
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  if (collate) {
    
    printf("\nSaving %s.centr.combined...", BseFNm.CStr());
    const TStr CombinedFNm = TStr::Fmt("%s.centr.combined", OutFNm.CStr());
    FILE *F = fopen(CombinedFNm.CStr(), "wt");
    fprintf(F, "# Node centrality distributions on the directed / undirected graph (as applicable)\n");
    fprintf(F, "# Nodes: %d\tEdges: %d\n", Graph->GetNodes(), Graph->GetEdges());
    fprintf(F, "# NodeId\tInDegCentr\tOutDegCentr\tDegCentr\tInEigCentr\tOutEigCentr\tEigCentr\tInAlphaCentr\tOutAlphaCentr\tAlphaCentr\tPgRCentr\n");
    for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
      const int NId = NI.GetId(); fprintf(F, "%d", NId);
      const TFltV DegCentrV = DegCentrVH.GetDat(NId);
      for (VI = DegCentrV.BegI(); VI < DegCentrV.EndI(); VI++) { fprintf(F, "\t%e", VI->Val); }
      const TFltV EigCentrV = EigCentrVH.GetDat(NId);
      for (VI = EigCentrV.BegI(); VI < EigCentrV.EndI(); VI++) { fprintf(F, "\t%e", VI->Val); }
      const TFltV AlphaCentrV = AlphaCentrVH.GetDat(NId);
      for (VI = AlphaCentrV.BegI(); VI < AlphaCentrV.EndI(); VI++) { fprintf(F, "\t%e", VI->Val); }
      const double PgRCentr = PgRH.GetDat(NId); fprintf(F, "\t%e", PgRCentr);
      fprintf(F, "\n");
    }
    printf(" DONE\n");
    
  } else {
    
    printf("\nSaving %s.deg.centr...", BseFNm.CStr());
    TSnap::SaveTxt(DegCentrVH, TStr::Fmt("%s.deg.centr", OutFNm.CStr()), "Degree centrality (in / out / undirected)", "NodeId", "InDegCentr\tOutDegCentr\tDegCentr");
    printf(" DONE\n");
    
    printf("Saving %s.eig...", BseFNm.CStr());
    TSnap::SaveTxt(EigCentrVH, TStr::Fmt("%s.eig", OutFNm.CStr()), "Eigenvector centrality (in / out / undirected)", "NodeId", "InEigCentr\tOutEigCentr\tEigCentr");
    printf(" DONE\n");
    
    printf("Saving %s.alpha.centrality...", BseFNm.CStr());
    TSnap::SaveTxt(AlphaCentrVH, TStr::Fmt("%s.alpha.centrality", OutFNm.CStr()), TStr::Fmt("Alpha centrality (in / out / undirected) with r = %f, a = %e", r, alpha), "NodeId", "InAlphaCentr\tOutAlphaCentr\tAlphaCentr");
    printf(" DONE\n");
    
    printf("Saving %s.pgr...", BseFNm.CStr());
    TSnap::SaveTxt(PgRH, TStr::Fmt("%s.pgr", OutFNm.CStr()), "PageRank centrality (pgr)", "NodeId", "PageRank");
    printf(" DONE\n");
    
  }
  
  printf("Saving %s.deg...", BseFNm.CStr());
  TSnap::SaveTxt(FirstDegVH, TStr::Fmt("%s.deg", OutFNm.CStr()), "First degree distributions (in / out / undirected)", "NodeId", "InDeg\tOutDeg\tDeg");
  printf(" DONE\n");
  
  printf("Saving %s.kdeg.IN...", BseFNm.CStr());
  TSnap::SaveTxt(kInDegVH, TStr::Fmt("%s.kdeg.IN", OutFNm.CStr()), TStr::Fmt("1 to %d in degree distributions (kdeg.IN)", k), "NodeId", "kInDegVH");
  printf(" DONE\n");
  
  printf("Saving %s.kdeg.OUT...", BseFNm.CStr());
  TSnap::SaveTxt(kOutDegVH, TStr::Fmt("%s.kdeg.OUT", OutFNm.CStr()), TStr::Fmt("1 to %d out degree distributions (kdeg.OUT)", k), "NodeId", "kOutDegVH");
  printf(" DONE\n");
  
  printf("Saving %s.kdeg...", BseFNm.CStr());
  TSnap::SaveTxt(kDegVH, TStr::Fmt("%s.kdeg", OutFNm.CStr()), TStr::Fmt("1 to %d degree distributions (kdeg)", k), "NodeId", "kOutDegVH");
  printf(" DONE\n");
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}