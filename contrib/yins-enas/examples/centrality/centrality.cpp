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
  const int k = Env.GetIfArgPrefixInt("-k:", 1, "depth of degree distributions (1 / 2 / ...)");
  const bool c = Env.GetIfArgPrefixBool("-c:", false, "collate centralities into matrix (T / F)");
  
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
  TIntFltVH DegCentrVH, EigCentrVH;
  TFltV EigDiffV;
  TIntFltH PgRH;
  double PgRDiff;
  TNGraph::TNodeI NI;
  TFltV::TIter VI;
  
  // CENTRALITY (computations)
  
  // First degree distributions
  
  printf("\nComputing degree distributions...");
  TSnap::GetDegVH(Graph, FirstDegVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // 1:k degree distributions
  
  printf("Computing egonet degrees for k = 1 to %d (in / out / undirected)\n", k);
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
  
  // Centrality measures
  
  printf("Computing degree centrality...");
  TSnap::GetDegreeCentrVH(Graph, DegCentrVH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  printf("Computing eigenvector centrality...");
  EigDiffV = TSnap::GetEigenVectorCentrVH(Graph, EigCentrVH, 1e-4, 1000);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  convergence differences (in / out / undirected)\n");
  printf("    %f\n", double(EigDiffV[0]));
  printf("    %f\n", double(EigDiffV[1]));
  printf("    %f\n", double(EigDiffV[2]));
  
  printf("Computing PageRank centrality...");
  PgRDiff = TSnap::GetPageRankNew(Graph, PgRH, 0.85, 1e-4, 1000);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  convergence difference: %f\n", double(PgRDiff));
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  if (c) {
    
    printf("\nSaving %s.centr.combined...", BseFNm.CStr());
    const TStr CombinedFNm = TStr::Fmt("%s.centr.combined", OutFNm.CStr());
    FILE *F = fopen(CombinedFNm.CStr(), "wt");
    fprintf(F,"# Node centrality distributions on the directed / undirected graph (as applicable)\n");
    fprintf(F,"# Nodes: %d\tEdges: %d\n", Graph->GetNodes(), Graph->GetEdges());
    fprintf(F,"# NodeId\tInDegCentr\tOutDegCentr\tDegCentr\tInEigCentr\tOutEigCentr\tEigCentr\tPgRCentr\n");
    for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
      const int NId = NI.GetId(); fprintf(F, "%d", NId);
      const TFltV DegCentrV = DegCentrVH.GetDat(NId);
      for (VI = DegCentrV.BegI(); VI < DegCentrV.EndI(); VI++) { fprintf(F, "\t%f", VI->Val); }
      const TFltV EigCentrV = EigCentrVH.GetDat(NId);
      for (VI = EigCentrV.BegI(); VI < EigCentrV.EndI(); VI++) { fprintf(F, "\t%f", VI->Val); }
      const double PgRCentr = PgRH.GetDat(NId); fprintf(F, "\t%f", PgRCentr);
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