#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Graph connectivity. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const bool gccOnly = Env.GetIfArgPrefixBool("--gcc-only:", false, "compute giant weakly connected component only");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", Graph->GetNodes());
  printf("  edges: %d\n", Graph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Declare variables
  PNGraph WSubGraph, SSubGraph, BSubGraph, FSubGraph, TeSubGraph;
  TCnComV WCnComV, SCnComV, BCnComV, FCnComV, TCnComV;
  TCnCom BCnCom, FCnCom;
  TVec<TCnCom>::TIter CnComI;
  
  // CONNECTIVITY (computations)
  
  // Giant weakly and strongly connected components (GWCC / GSCC)
  
  WSubGraph = TSnap::GetMxWcc(Graph);
  SSubGraph = TSnap::GetMxScc(WSubGraph);
  
  // Weakly connected components and strongly connected cores (WCCS / SCCS)
  
  TSnap::GetWccs(Graph, WCnComV);
  TSnap::GetWccSccCores(Graph, WCnComV, SCnComV);
  
  TSnap::printCnComVSummary(WCnComV, "WCnComV\n-------");
  TSnap::printCnComVSummary(SCnComV, "SCnComV\n-------");
  
  // Components connecting in to the SCCS and out from the SCCS (IN / OUT / TE)
  
  TSnap::GetIns(Graph, WCnComV, BCnComV);
  TSnap::GetOuts(Graph, WCnComV, FCnComV);
  TSnap::GetTes(Graph, WCnComV, TCnComV);
  
  TSnap::printCnComVSummary(BCnComV, "BCnComV\n-------");
  TSnap::printCnComVSummary(FCnComV, "FCnComV\n-------");
  TSnap::printCnComVSummary(TCnComV, "TCnComV\n-------");
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  if (gccOnly) {
    
    // GWCC and GSCC
    
    printf("\nSaving %s.GWCC and %s.GSCC...", BseFNm.CStr(), BseFNm.CStr());
    TSnap::SaveTxt(WCnComV[0].NIdV, TStr::Fmt("%s.GWCC", OutFNm.CStr()), "Giant weakly connected component (GWCC)", "NodeId");
    TSnap::SaveTxt(SCnComV[0].NIdV, TStr::Fmt("%s.GSCC", OutFNm.CStr()), "Giant strongly connected core (GSCC)", "NodeId");
    // TSnap::SaveTxt(WSubGraph, TStr::Fmt("%s.GWCC", OutFNm.CStr()), "Giant weakly connected component (GWCC)");
    // TSnap::SaveTxt(SSubGraph, TStr::Fmt("%s.GSCC", OutFNm.CStr()), "Giant strongly connected component (GSCC)");
    printf(" DONE\n");
    
    printf("\nGWCC\n----\n");
    printf("Nodes: %d\n", WSubGraph->GetNodes());
    printf("Edges: %d\n", WSubGraph->GetEdges());
    
    printf("\nGSCC\n----\n");
    printf("Nodes: %d\n", SSubGraph->GetNodes());
    printf("Edges: %d\n", SSubGraph->GetEdges());
    
    // GIN and GOUT
    
    printf("\nSaving %s.GIN and %s.GOUT...", BseFNm.CStr(), BseFNm.CStr());
    TSnap::SaveTxt(BCnComV[0].NIdV, TStr::Fmt("%s.GIN", OutFNm.CStr()), "Components connecting in to the GSCC (GIN)", "NodeId");
    TSnap::SaveTxt(FCnComV[0].NIdV, TStr::Fmt("%s.GOUT", OutFNm.CStr()), "Components connected out from the GSCC (GOUT)", "NodeId");
    printf(" DONE\n");
    
    printf("\nGIN\n---\n");
    printf("Nodes: %d\n", BCnComV[0].NIdV.Len());
    printf("\nGOUT\n----\n");
    printf("Nodes: %d\n", FCnComV[0].NIdV.Len());
    
    // GTE
    
    printf("\nSaving %s.GTE...", BseFNm.CStr());
    TSnap::SaveTxt(TCnComV[0].NIdV, TStr::Fmt("%s.GTE", OutFNm.CStr()), "Tendrils from the IN and to the OUT (GTE)", "NodeId");
    printf(" DONE\n");
    
    printf("\nGTE\n---\n");
    printf("Nodes: %d\n", TCnComV[0].NIdV.Len());
    
  } else {
    
    printf("\nSaving %s.WCCS and %s.SCCS...", BseFNm.CStr(), BseFNm.CStr());
    TSnap::SaveTxt(WCnComV, TStr::Fmt("%s.WCCS", OutFNm.CStr()), "Weakly connected components (WCCS)");
    TSnap::SaveTxt(SCnComV, TStr::Fmt("%s.SCCS", OutFNm.CStr()), "Strongly connected cores (SCCS)");
    printf(" DONE\n");
    
    printf("\nSaving %s.INS and %s.OUTS...", BseFNm.CStr(), BseFNm.CStr());
    TSnap::SaveTxt(BCnComV, TStr::Fmt("%s.INS", OutFNm.CStr()), "Components connecting in to the SCCS  (INS)");
    TSnap::SaveTxt(FCnComV, TStr::Fmt("%s.OUTS", OutFNm.CStr()), "Components connected out from the SCCS (OUTS)");
    printf(" DONE\n");
    
    printf("\nSaving %s.TES...", BseFNm.CStr());
    TSnap::SaveTxt(TCnComV, TStr::Fmt("%s.TES", OutFNm.CStr()), "Tendrils from the INS and to the OUTS (TES)");
    printf(" DONE\n");
    
  }
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}