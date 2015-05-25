#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Node centrality. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const bool c = Env.GetIfArgPrefixBool("-c:", false, "collate centralities into matrix (T / F)");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", Graph->GetNodes());
  printf("  edges: %d\n", Graph->GetEdges());
  
  // Declare variables
  TIntIntVH FirstDegVH;
  TIntFltVH DegCentrVH, EigCentrVH;
  TFltV EigDiffV;
  TIntFltH PgRH;
  double PgRDiff;
  TNGraph::TNodeI NI;
  TFltV::TIter VI;
  
  // CENTRALITY (computations)
  
  // First degree distributions
  printf("\nComputing first degree distributions...");
  TSnap::GetDegVH(Graph, FirstDegVH);
  printf(" DONE\n");
  
  // Centrality measures
  printf("Computing centrality measures...");
  TSnap::GetDegreeCentrVH(Graph, DegCentrVH);
  EigDiffV = TSnap::GetEigenVectorCentrVH(Graph, EigCentrVH, 1e-4, 100);
  PgRDiff = TSnap::GetPageRankNew(Graph, PgRH, 0.85, 1e-4, 100);
  printf(" DONE\n");
  printf("Eigenvector centrality convergence differences (in / out / undirected)\n");
  printf("  in: %f\n", double(EigDiffV[0]));
  printf("  out: %f\n", double(EigDiffV[1]));
  printf("  undirected: %f\n", double(EigDiffV[2]));
  printf("PageRank centrality convergence difference: %f\n", double(PgRDiff));
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  if (c) {
    
    printf("\nSaving %s.centr...", BseFNm.CStr());
    const TStr AggFNm = TStr::Fmt("%s.centr", OutFNm.CStr());
    FILE *F = fopen(AggFNm.CStr(), "wt");
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
    TSnap::SaveTxt(DegCentrVH, TStr::Fmt("%s.deg.centr", OutFNm.CStr()), "First degree centrality (in / out / undirected)", "NodeId", "InDegCentr\tOutDegCentr\tDegCentr");
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
  
  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}