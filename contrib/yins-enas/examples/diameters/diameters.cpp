#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Graph connectivity. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input graph (tab separated list of edges)");
  const TStr SubsetNIdVFNm = Env.GetIfArgPrefixStr("-j:", "", "subset nodes (column of nodes)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const bool collate = Env.GetIfArgPrefixBool("--collate:", false, "collate centralities into matrix (T / F)");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", Graph->GetNodes());
  printf("  edges: %d\n", Graph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Load subset nodes
  TIntV SubsetNIdV = TSnap::LoadTxtIntV(SubsetNIdVFNm);
  
  // Declare variables
  TIntIntH InNodesH, InDiameterH, OutNodesH, OutDiameterH, NodesH, DiameterH;
  TNGraph::TNodeI NI;
  
  // SUBSET DIAMETER AND NODE COUNTS
  
  printf("Computing subset diameter and node counts\n");
  TSnap::TFixedMemorySubsetDiameter<PNGraph> FixedMemorySubsetDiameter(Graph);
  printf("  ...");
  FixedMemorySubsetDiameter.ComputeInSubsetDiameter(SubsetNIdV, InNodesH, InDiameterH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  ...");
  FixedMemorySubsetDiameter.ComputeOutSubsetDiameter(SubsetNIdV, OutNodesH, OutDiameterH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  ...");
  FixedMemorySubsetDiameter.ComputeSubsetDiameter(SubsetNIdV, NodesH, DiameterH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
 
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  if (collate) {
    
    printf("\nSaving %s.diameters.combined...", BseFNm.CStr());
    const TStr CombinedFNm = TStr::Fmt("%s.diameters.combined", OutFNm.CStr());
    FILE *F = fopen(CombinedFNm.CStr(), "wt");
    fprintf(F,"# Subset diameters and node counts (in / out / undirected)\n");
    fprintf(F,"# Nodes: %d\tEdges: %d\t Subset size: %d\n", Graph->GetNodes(), Graph->GetEdges(), SubsetNIdV.Len());
    fprintf(F,"# SubsetNodeId\tInDiameter\tInNodes\tOutDiameter\tOutNodes\tDiameter\tNodes\n");
    for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
      const int NId = NI.GetId(); fprintf(F, "%d", NId);
      fprintf(F, "\t%d\t%d", int(InDiameterH.GetDat(NId)), int(InNodesH.GetDat(NId)));
      fprintf(F, "\t%d\t%d", int(OutDiameterH.GetDat(NId)), int(OutNodesH.GetDat(NId)));
      fprintf(F, "\t%d\t%d", int(DiameterH.GetDat(NId)), int(NodesH.GetDat(NId)));
      fprintf(F, "\n");
    }
    printf(" DONE\n");
    
  } else {
    
    printf("\nSaving %s.nodes.IN...", BseFNm.CStr());
    TSnap::SaveTxt(InNodesH, TStr::Fmt("%s.nodes.IN", OutFNm.CStr()), "Number of nodes in neighborhood (in) ", "Node", "Number");
    printf(" DONE");
    
    printf("\nSaving %s.nodes.OUT...", BseFNm.CStr());
    TSnap::SaveTxt(OutNodesH, TStr::Fmt("%s.nodes.OUT", OutFNm.CStr()), "Number of nodes in neighborhood (out) ", "Node", "Number");
    printf(" DONE");
    
    printf("\nSaving %s.nodes...", BseFNm.CStr());
    TSnap::SaveTxt(NodesH, TStr::Fmt("%s.nodes", OutFNm.CStr()), "Number of nodes in neighborhood (undirected) ", "Node", "Number");
    printf(" DONE\n");
   
    printf("\nSaving %s.diameter.IN...", BseFNm.CStr());
    TSnap::SaveTxt(InDiameterH, TStr::Fmt("%s.diameter.IN", OutFNm.CStr()), "Diameter of neighborhood (in) ", "Node", "Diameter");
    printf(" DONE");
    
    printf("\nSaving %s.diameter.OUT...", BseFNm.CStr());
    TSnap::SaveTxt(OutDiameterH, TStr::Fmt("%s.diameter.OUT", OutFNm.CStr()), "Diameter of neighborhood (out) ", "Node", "Diameter");
    printf(" DONE");
    
    printf("\nSaving %s.diameter...", BseFNm.CStr());
    TSnap::SaveTxt(DiameterH, TStr::Fmt("%s.diameter", OutFNm.CStr()), "Diameter of neighborhood (undirected) ", "Node", "Diameter");
    printf(" DONE\n");
 
  }
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}