#include "stdafx.h"

void ComputeINFH(const PNGraph& Graph, const TIntV& NIdV, const TEdgeDir& d, const TStr& OutFNm, const TStr& BseFNm, const TStr& SubNm, const bool& collate, const TExeTm& ExeTm) {
  
  // Declare variables
  TIntIntVH INFH;
  TIntIntH NodesH, DiameterH;
  TIntV::TIter VI;
  
  // SUBSET NEIGHBORHOOD FUNCTION
  
  printf("\nComputing %s subset diameter and node counts...", SubNm.CStr());
  TSnap::TFixedMemoryExactNF<PNGraph> FixedMemoryExactNF(Graph);
  FixedMemoryExactNF.ComputeSubsetINFH(NIdV, d, INFH);
  printf(" DONE: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Compute diameters as the maximum quantiles of the INF
  TSnap::GetNodesINFH(INFH, NodesH);
  TSnap::GetDiameterINFH(INFH, DiameterH);
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  printf("\nSaving %s.%s.INFH...", BseFNm.CStr(), SubNm.CStr());
  TSnap::SaveTxt(INFH, TStr::Fmt("%s.%s.INFH", OutFNm.CStr(), SubNm.CStr()), TStr::Fmt("Exact individual neighborhood function with d = %d", d), "Node", "INFVH");
  printf(" DONE\n");   
  
  if (collate) {
    
    printf("\nSaving %s.%s.diameters.summary...", BseFNm.CStr(), SubNm.CStr());
    const TStr CombinedFNm = TStr::Fmt("%s.%s.diameters.summary", OutFNm.CStr(), SubNm.CStr());
    FILE *F = fopen(CombinedFNm.CStr(), "wt");
    fprintf(F, "# Subset diameters and node counts with d = %d\n", d);
    fprintf(F, "# Nodes: %d\tEdges: %d\t Subset size: %d\n", Graph->GetNodes(), Graph->GetEdges(), NIdV.Len());
    fprintf(F, "# SubsetNodeId\tDiameter\tNodes\n");
    for (VI = NIdV.BegI(); VI < NIdV.EndI(); VI++) {
      const int NId = VI->Val; fprintf(F, "%d", NId);
      fprintf(F, "\t%d\t%d", int(DiameterH.GetDat(NId)), int(NodesH.GetDat(NId)));
      fprintf(F, "\n");
    }
    printf(" DONE\n");
    
  } else {
    
    printf("\nSaving %s.%s.nodes...", BseFNm.CStr(), SubNm.CStr());
    TSnap::SaveTxt(NodesH, TStr::Fmt("%s.%s.nodes", OutFNm.CStr(), SubNm.CStr()), TStr::Fmt("Node counts with d = %d", d), "Node", "Number");
    printf(" DONE");
   
    printf("\nSaving %s.%s.diameter...", BseFNm.CStr(), SubNm.CStr());
    TSnap::SaveTxt(DiameterH, TStr::Fmt("%s.%s.diameter", OutFNm.CStr(), SubNm.CStr()), TStr::Fmt("Diameter of neighborhood with d = %d", d), "Node", "Diameter");
    printf(" DONE\n");
 
  }
  
}

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Graph connectivity. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges)");
  const TStr SubNIdVFNm = Env.GetIfArgPrefixStr("-s:", "", "subset nodes (column of nodes)");
  const TStr SubNm = SubNIdVFNm.RightOfLast('/').LeftOfLast('.');
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const TEdgeDir d = (TEdgeDir) Env.GetIfArgPrefixInt("-d:", 3, "direction of traversal: in = 1, out = 2, undected = 3");
  const bool compare = Env.GetIfArgPrefixBool("--compare:", true, "compare to a random subset of nodes: T / F");
  const bool collate = Env.GetIfArgPrefixBool("--collate:", true, "collate properties into matrix: T / F");
  
  // Load graph and create dected and undected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", Graph->GetNodes());
  printf("  edges: %d\n", Graph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Declare variables
  TIntV SubNIdV, NIdV, RndNIdV;
  TRnd Rnd;
  
  // Load subset nodes and compute disjoint random subset of nodes (same size) 
  SubNIdV = TSnap::LoadTxtIntV(SubNIdVFNm);
  Graph->GetNIdV(NIdV);
  NIdV.Diff(SubNIdV);
  RndNIdV = TSnap::GetRndSubV(NIdV, SubNIdV.Len(), Rnd);
  
  // SUBSET DIAMETER AND NODE COUNTS
  
  ComputeINFH(Graph, SubNIdV, d, OutFNm, BseFNm, SubNm, collate, ExeTm);
  if (compare) {
    ComputeINFH(Graph, RndNIdV, d, OutFNm, BseFNm, TStr("random"), collate, ExeTm);
  }
    
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}
