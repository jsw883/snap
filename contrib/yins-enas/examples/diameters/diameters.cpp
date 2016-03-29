#include "stdafx.h"

void ComputeINFH(const PNGraph& Graph, const TIntV& SrcNIdV, const TIntV& DstNIdV, const TEdgeDir& d, const TStr& OutFNm, const TStr& BseFNm, const TStr& SrcNm, const TStr& DstNm, const bool& collate, const TExeTm& ExeTm) {
  
  // Declare variables
  TUInt64V NF;
  THash<TInt, TUInt64V> INFH;
  TIntIntVH DstNIdShortestPathVH;
  TIntIntH NodesH, DiameterH;
  TIntFltH RadiusH; // Radius is "median path length"
  TIntV::TIter VI;
  
  // SUBSET NEIGHBORHOOD FUNCTION
  
  printf("\nComputing %s subset diameter, node counts, and exact shortest paths...", SrcNm.CStr());
  TSnap::TCustomFixedMemoryNeighborhood<PNGraph> CustomFixedMemoryNeighborhood(Graph, DstNIdV);
  CustomFixedMemoryNeighborhood.ComputeCustomSubsetINFH(SrcNIdV, d, INFH, DstNIdShortestPathVH);
  TSnap::ConvertSubsetINFHSubsetNF(INFH, NF);
  printf(" DONE: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Compute diameters as the maximum quantiles of the INF
  TSnap::GetNodesINFH(INFH, NodesH);
  TSnap::InterpolateINFH(INFH, RadiusH, 0.5);
  TSnap::GetDiameterINFH(INFH, DiameterH);
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  printf("\nSaving %s.%s.INFH...", BseFNm.CStr(), SrcNm.CStr());
  TSnap::SaveTxt(INFH, TStr::Fmt("%s.%s.INFH", OutFNm.CStr(), SrcNm.CStr()), TStr::Fmt("Exact individual neighborhood function with d = %d", d), "Node", "INFVH");
  printf(" DONE\n");   
  
  printf("\nSaving %s.%s.hop.NF...", BseFNm.CStr(), SrcNm.CStr());
  TSnap::SaveTxt(NF, TStr::Fmt("%s.%s.hop.NF", OutFNm.CStr(), SrcNm.CStr()), "Exact subset neighbourhood function / shortest path cumulative density (hop)");
  printf(" DONE\n");
  
  printf("\nSaving %s.%s.%s.ShortestPathVH...", BseFNm.CStr(), SrcNm.CStr(), DstNm.CStr());
  TSnap::SaveTxt(DstNIdShortestPathVH, TStr::Fmt("%s.%s.%s.ShortestPathVH", OutFNm.CStr(), SrcNm.CStr(),  DstNm.CStr()), "Exact shortest paths to another subset of the graph (possibly exhaustive)");
  printf(" DONE\n");
  
  if (collate) {
    
    printf("\nSaving %s.%s.diameters.summary...", BseFNm.CStr(), SrcNm.CStr());
    const TStr CombinedFNm = TStr::Fmt("%s.%s.diameters.summary", OutFNm.CStr(), SrcNm.CStr());
    FILE *F = fopen(CombinedFNm.CStr(), "wt");
    fprintf(F, "# Subset node counts, radius (median path length), and diameter with d = %d\n", d);
    fprintf(F, "# Nodes: %d\tEdges: %d\t Subset size: %d\n", Graph->GetNodes(), Graph->GetEdges(), SrcNIdV.Len());
    fprintf(F, "# SubsetNodeId\tNodes\tRadius\tDiameter\n");
    for (VI = SrcNIdV.BegI(); VI < SrcNIdV.EndI(); VI++) {
      const int NId = VI->Val; fprintf(F, "%d", NId);
      fprintf(F, "\t%d\t%f\t%d", (int) NodesH.GetDat(NId), (double) RadiusH.GetDat(NId), (int) DiameterH.GetDat(NId));
      fprintf(F, "\n");
    }
    printf(" DONE\n");
    
  } else {
    
    printf("\nSaving %s.%s.nodes...", BseFNm.CStr(), SrcNm.CStr());
    TSnap::SaveTxt(NodesH, TStr::Fmt("%s.%s.nodes", OutFNm.CStr(), SrcNm.CStr()), TStr::Fmt("Node counts with d = %d", d), "Node", "Nodes");
    printf(" DONE");
   
    printf("\nSaving %s.%s.diameter...", BseFNm.CStr(), SrcNm.CStr());
    TSnap::SaveTxt(DiameterH, TStr::Fmt("%s.%s.diameter", OutFNm.CStr(), SrcNm.CStr()), TStr::Fmt("Diameter of neighborhood with d = %d", d), "Node", "Diameter");
    printf(" DONE\n");
 
    printf("\nSaving %s.%s.radius...", BseFNm.CStr(), SrcNm.CStr());
    TSnap::SaveTxt(RadiusH, TStr::Fmt("%s.%s.radius", OutFNm.CStr(), SrcNm.CStr()), TStr::Fmt("Radius (median path length) of neighborhood with d = %d", d), "Node", "Radius");
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
  const TStr SrcNIdVFNm = Env.GetIfArgPrefixStr("-s:", "", "source subset of nodes (new line separated)");
  const TStr SrcNm = SrcNIdVFNm.RightOfLast('/').LeftOfLast('.');
  const TStr DstNIdVFNm = Env.GetIfArgPrefixStr("-d:", "", "destination subset of nodes (new line separated)");
  const TStr DstNm = DstNIdVFNm.RightOfLast('/').LeftOfLast('.');
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const TEdgeDir d = (TEdgeDir) Env.GetIfArgPrefixInt("--dir:", 3, "direction of traversal: in = 1, out = 2, undected = 3");
  const bool exhaustive = Env.GetIfArgPrefixBool("--exhaustive:", false, "compute for every node (overrides -s, -d): T / F");
  const bool collate = Env.GetIfArgPrefixBool("--collate:", true, "collate properties into matrix: T / F");
  
  // Load graph and create dected and undected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", Graph->GetNodes());
  printf("  edges: %d\n", Graph->GetEdges());
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
    Graph->GetNIdV(NIdV);
    ComputeINFH(Graph, NIdV, NIdV, d, OutFNm, BseFNm, TStr("exhaustive"), TStr("exhaustive"), collate, ExeTm);
  } else {
    // SUBSET
    ComputeINFH(Graph, SrcNIdV, DstNIdV, d, OutFNm, BseFNm, SrcNm, DstNm, collate, ExeTm);
  }
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}
