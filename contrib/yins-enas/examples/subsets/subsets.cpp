#include "stdafx.h"

void ComputeINFH(const PNGraph& Graph, const TIntV& SrcNIdV, const TIntV& DstNIdV, const TEdgeDir& dir, const TIntSet& SkipNIdS, const TStr& OutFNm, const TStr& BseFNm, const TStr& SrcNm, const TStr& DstNm, const bool& collate, const TExeTm& ExeTm) {
  
  // Declare variables
  TUInt64V NF;
  THash<TInt, TUInt64V> INFH;
  TIntIntVH ShortestPathVH;
  TIntIntH NodesH, DiameterH;
  TIntFltH RadiusH; // Radius is "median path length"
  TIntV::TIter VI;
  
  TStr Name;

  // SUBSET NEIGHBORHOOD FUNCTION
  
  printf("\nComputing %s subset diameter, node counts, and exact shortest paths...", SrcNm.CStr());
  TSnap::TFixedMemoryExhaustiveNeighborhood<PNGraph> FixedMemoryExhaustiveNeighborhood(Graph, DstNIdV);
  FixedMemoryExhaustiveNeighborhood.ComputeSubsetINFH(SrcNIdV, dir, SkipNIdS, INFH, ShortestPathVH);
  TSnap::ConvertSubsetINFHSubsetNF(INFH, NF);
  printf(" DONE: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // TSnap::printDataVHSummary(ShortestPathVH, "ShortestPathVH\n--------------");
  
  // Compute diameters as the maximum quantiles of the INF
  TSnap::GetNodesINFH(INFH, NodesH);
  TSnap::InterpolateINFH(INFH, RadiusH, 0.5);
  TSnap::GetDiameterINFH(INFH, DiameterH);
  
  TSnap::printDataHSummary(NodesH, "\nNodesH\n------");
  TSnap::printDataHSummary(RadiusH, "\nRadiusH\n-------");
  TSnap::printDataHSummary(DiameterH, "\nDiameterH\n---------");
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  Name = TStr::Fmt("%s.%s.INFH", OutFNm.CStr(), SrcNm.CStr());
  printf("\nSaving %s...", Name.CStr());
  TSnap::SaveTxt(INFH, Name.CStr(), TStr::Fmt("Exact individual neighborhood function (dir: %d)", dir), "Node", "INFVH");
  printf(" DONE\n");
  
  Name = TStr::Fmt("%s.%s.NF", OutFNm.CStr(), SrcNm.CStr());
  printf("\nSaving %s...", Name.CStr());
  TSnap::SaveTxt(NF, Name.CStr(), "Exact subset neighbourhood function / shortest path cumulative density (hop)");
  printf(" DONE\n");
  
  TSnap::printDataV(NF, true, "\nNF\n--");
  
  TIntIntVH::TIter HI;

  // printf("\nSaving %s.%s.%s.ShortestPathVH...", BseFNm.CStr(), SrcNm.CStr(), DstNm.CStr());
  // TSnap::SaveTxt(ShortestPathVH, TStr::Fmt("%s.%s.%s.ShortestPathVH", OutFNm.CStr(), SrcNm.CStr(),  DstNm.CStr()), "Exact shortest paths to another subset of the graph (possibly exhaustive)");
  // printf(" DONE\n");
    
  Name = TStr::Fmt("%s.%s.%s.ShortestPathVH", OutFNm.CStr(), SrcNm.CStr(), DstNm.CStr());
  printf("\nSaving %s...", Name.CStr());
  FILE *F = fopen(Name.CStr(), "wt");  
  fprintf(F, "# Exact shortest paths to another subset of the graph (possibly exhaustive)\n");
  fprintf(F, "# SrcNIdV.Len(): %d\tDstNIdV.Len(): %d\t\n", SrcNIdV.Len(), DstNIdV.Len());
  fprintf(F, "# SrcNId\tDstNId\tShortestPathLength\n");
  for (HI = ShortestPathVH.BegI(); HI < ShortestPathVH.EndI(); HI++) {
    const TIntV& ShortestPathV = HI.GetDat();
    for (int i = 0; i < DstNIdV.Len(); i++) {
      if (ShortestPathV[i] != -1) {
        fprintf(F, "%d\t%d\t%d\n", (int) HI.GetKey(), (int) DstNIdV[i], (int) ShortestPathV[i]);
      }
    }
  }
  printf(" DONE\n");

  if (collate) {
    
    Name = TStr::Fmt("%s.%s.SubsetsCombined", OutFNm.CStr(), SrcNm.CStr());
    printf("\nSaving %s...", Name.CStr());
    FILE *F = fopen(Name.CStr(), "wt");
    fprintf(F, "# Subset node counts, radius (median path length), and diameter (dir: %d)\n", dir);
    fprintf(F, "# Nodes: %d\tEdges: %d\t Subset size: %d\n", Graph->GetNodes(), Graph->GetEdges(), SrcNIdV.Len());
    fprintf(F, "# SubsetNodeId\tNodes\tRadius\tDiameter\n");
    for (VI = SrcNIdV.BegI(); VI < SrcNIdV.EndI(); VI++) {
      const int NId = VI->Val; fprintf(F, "%d", NId);
      fprintf(F, "\t%d\t%f\t%d", (int) NodesH.GetDat(NId), (double) RadiusH.GetDat(NId), (int) DiameterH.GetDat(NId));
      fprintf(F, "\n");
    }
    printf(" DONE\n");
    
  } else {
    
    Name = TStr::Fmt("%s.%s.NodesH", OutFNm.CStr(), SrcNm.CStr());
    printf("\nSaving %s...", Name.CStr());
    TSnap::SaveTxt(NodesH, Name.CStr(), TStr::Fmt("Node counts (dir: %d)", dir), "Node", "Nodes");
    printf(" DONE");
   
    Name = TStr::Fmt("%s.%s.DiameterH", OutFNm.CStr(), SrcNm.CStr());
    printf("\nSaving %s...", Name.CStr());
    TSnap::SaveTxt(DiameterH, Name.CStr(), TStr::Fmt("Diameter of neighborhood (dir: %d)", dir), "Node", "Diameter");
    printf(" DONE\n");
 
    Name = TStr::Fmt("%s.%s.RadiusH", OutFNm.CStr(), SrcNm.CStr());
    printf("\nSaving %s...", Name.CStr());
    TSnap::SaveTxt(RadiusH, Name.CStr(), TStr::Fmt("Radius (median path length) of neighborhood (dir: %d)", dir), "Node", "Radius");
    printf(" DONE\n");
 
  }
  
}

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Graph subset NF. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges)");
  const TStr SrcNIdVFNm = Env.GetIfArgPrefixStr("-s:", "", "source subset of nodes (new line separated)");
  const TStr SrcNm = SrcNIdVFNm.RightOfLast('/').LeftOfLast('.');
  const TStr DstNIdVFNm = Env.GetIfArgPrefixStr("-d:", "", "destination subset of nodes (new line separated)");
  const TStr DstNm = DstNIdVFNm.RightOfLast('/').LeftOfLast('.');
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const TEdgeDir dir = (TEdgeDir) Env.GetIfArgPrefixInt("--dir:", 3, "direction of traversal: in = 1, out = 2, undected = 3");
  const bool exclude = Env.GetIfArgPrefixBool("--exclude:", true, "exclude other source nodes from BFS");
  const bool exhaustive = Env.GetIfArgPrefixBool("--exhaustive:", false, "compute for every node (overrides -s, -d): T / F");
  const bool collate = Env.GetIfArgPrefixBool("--collate:", true, "collate properties into matrix: T / F");
  
  // Load graph and create dected and undected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printGraphSummary(Graph, "\nGraph\n-----");
  
  // Declare variables
  TIntV SrcNIdV, DstNIdV, NIdV, RndNIdV;
  TRnd Rnd(0);
  TIntSet SkipNIdS;
  
  // Load subset nodes and compute disjoint random subset of nodes (same size)
  SrcNIdV = TSnap::LoadTxtIntV(SrcNIdVFNm);
  DstNIdV = TSnap::LoadTxtIntV(DstNIdVFNm);
  
  if (exclude) {
    SkipNIdS.AddKeyV(SrcNIdV);
  }

  // SUBSET DIAMETER AND NODE COUNTS
  
  if (exhaustive) {
    // EXHAUSTIVE
    Graph->GetNIdV(NIdV);
    ComputeINFH(Graph, NIdV, NIdV, dir, SkipNIdS, OutFNm, BseFNm, TStr("exhaustive"), TStr("exhaustive"), collate, ExeTm);
  } else {
    // SUBSET
    ComputeINFH(Graph, SrcNIdV, DstNIdV, dir, SkipNIdS, OutFNm, BseFNm, SrcNm, DstNm, collate, ExeTm);
  }
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}
