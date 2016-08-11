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
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printGraphSummary(Graph, "\nGraph\n-----");
  
  // Declare variables
  TIntIntVH FirstDegVH;
  TIntIntVH kInDegVH, kOutDegVH, kDegVH;
  TIntFltH ExoH;
  TIntFltVH DegCentrVH, EigCentrVH, AlphaCentrVH;
  TFltV AlphaV;
  TFltV EigDiffV, EigV, AlphaDiffV;
  TIntFltH PgRH;
  double PgRDiff;
  TNGraph::TNodeI NI;
  TFltV::TIter VI;
  
  TStr Name;
  
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
  
  TSnap::printCategoryVHSummary(FirstDegVH, "\nFirstDegVH\n----------");
  
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
  
  TSnap::printCategoryVHSummary(kInDegVH, "\nkInDegVH\n--------");
  TSnap::printCategoryVHSummary(kOutDegVH, "\nkOutDegVH\n---------");
  TSnap::printCategoryVHSummary(kDegVH, "\nkDegVH\n------");
  
  // Degree centrality
  
  printf("\nComputing degree centrality...");
  TSnap::GetDegreeCentrVH(Graph, DegCentrVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printCategoryVHSummary(DegCentrVH, "\nDegCentrVH\n----------");
  
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
  
  TSnap::printCategoryVHSummary(EigCentrVH, "\nEigCentrVH\n----------");
  
  // Alpha centrality
  
  AlphaV.Add(r / EigV[0]);
  AlphaV.Add(r / EigV[1]);
  AlphaV.Add(r / EigV[2]);

  printf("\nComputing alpha centrality...");
  AlphaDiffV = TSnap::GetAlphaCentrVH(Graph, ExoH, AlphaCentrVH, AlphaV, eps, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  printf("\nAlpha parameters (r: %f)\n", r);
  printf("%e\n", (double) AlphaV[0]);
  printf("%e\n", (double) AlphaV[1]);
  printf("%e\n", (double) AlphaV[2]);
  
  printf("\nConvergence (in / out / undirected)\n");
  printf("%e%s\n", (double) AlphaDiffV[0], AlphaDiffV[0] < eps ? "" : " DID NOT CONVERGE");
  printf("%e%s\n", (double) AlphaDiffV[1], AlphaDiffV[1] < eps ? "" : " DID NOT CONVERGE");
  printf("%e%s\n", (double) AlphaDiffV[2], AlphaDiffV[2] < eps ? "" : " DID NOT CONVERGE");
  
  if (AlphaDiffV[0] > eps || AlphaDiffV[1] > eps || AlphaDiffV[2] > eps) {
    printf("\nNOTE: for alpha centrality to converge, alpha must be less than the inverse leading eigenvalue for the direction specified.\n");
  }
  
  TSnap::printCategoryVHSummary(AlphaCentrVH, "\nAlphaCentrVH\n----------");
  
  // PageRank centrality
  
  printf("\nComputing PageRank centrality...");
  PgRDiff = TSnap::GetPageRankNew(Graph, PgRH, c, eps, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  printf("\nConvergence: %e%s\n", double(PgRDiff), PgRDiff < eps ? "" : " DID NOT CONVERGE");
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  if (collate) {
    
    Name = TStr::Fmt("%s.CentrCombined", OutFNm.CStr());
    printf("\nSaving %s...", Name.CStr());
    FILE *F = fopen(Name.CStr(), "wt");
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
    
    Name = TStr::Fmt("%s.DegCentrVH", OutFNm.CStr());
    printf("\nSaving %s...", Name.CStr());
    TSnap::SaveTxt(DegCentrVH, Name.CStr(), "Degree centrality (in / out / undirected)", "NodeId", "InDegCentr\tOutDegCentr\tDegCentr");
    printf(" DONE\n");
    
    Name = TStr::Fmt("%s.EigCentrVH", OutFNm.CStr());
    printf("Saving %s...", Name.CStr());
    TSnap::SaveTxt(EigCentrVH, Name.CStr(), "Eigenvector centrality (in / out / undirected)", "NodeId", "InEigCentr\tOutEigCentr\tEigCentr");
    printf(" DONE\n");
    
    Name = TStr::Fmt("%s.AlphaCentrVH", OutFNm.CStr());
    printf("Saving %s...", Name.CStr());
    TSnap::SaveTxt(AlphaCentrVH, Name.CStr(), TStr::Fmt("Alpha centrality (in / out / undirected) with r = %f, a = (%e, %e, %e)", r, AlphaV[0], AlphaV[1], AlphaV[2]), "NodeId", "InAlphaCentr\tOutAlphaCentr\tAlphaCentr");
    printf(" DONE\n");
    
    Name = TStr::Fmt("%s.PgRH", OutFNm.CStr());
    printf("Saving %s...", Name.CStr());
    TSnap::SaveTxt(PgRH, Name.CStr(), "PageRank centrality (pgr)", "NodeId", "PageRank");
    printf(" DONE\n");
    
  }
  
  Name = TStr::Fmt("%s.FirstDegVH", OutFNm.CStr());
  printf("Saving %s...", Name.CStr());
  TSnap::SaveTxt(FirstDegVH, Name.CStr(), "First degree distributions (in / out / undirected)", "NodeId", "InDeg\tOutDeg\tDeg");
  printf(" DONE\n");
  
  Name = TStr::Fmt("%s.kInDegVH", OutFNm.CStr());
  printf("Saving %s...", Name.CStr());
  TSnap::SaveTxt(kInDegVH, Name.CStr(), TStr::Fmt("1 to %d in degree distributions (kdeg.IN)", k), "NodeId", "kInDegVH");
  printf(" DONE\n");
  
  Name = TStr::Fmt("%s.kOutDegVH", OutFNm.CStr());
  printf("Saving %s...", Name.CStr());
  TSnap::SaveTxt(kOutDegVH, Name.CStr(), TStr::Fmt("1 to %d out degree distributions (kdeg.OUT)", k), "NodeId", "kOutDegVH");
  printf(" DONE\n");
  
  Name = TStr::Fmt("%s.kDegVH", OutFNm.CStr());
  printf("Saving %s...", Name.CStr());
  TSnap::SaveTxt(kDegVH, Name.CStr(), TStr::Fmt("1 to %d degree distributions (kdeg)", k), "NodeId", "kOutDegVH");
  printf(" DONE\n");
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}