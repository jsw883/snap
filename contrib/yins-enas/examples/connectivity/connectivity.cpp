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
  const bool giantOnly = Env.GetIfArgPrefixBool("--giant-only:", false, "compute giant weakly connected component only");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", Graph->GetNodes());
  printf("  edges: %d\n", Graph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Declare variables
  PNGraph WSubGraph, SSubGraph, BSubGraph, FSubGraph, TeSubGraph;
  TCnComV WCnComV, SCnComV, BCnComV, FCnComV, TeCnComV;
  TCnCom BCnCom, FCnCom;
  TVec<TCnCom>::TIter CnComI;
  
  // CONNECTIVITY (computations)
  
  // Giant weakly and strongly connected components (GWCC / GSCC)
  WSubGraph = TSnap::GetMxWcc(Graph);
  SSubGraph = TSnap::GetMxScc(WSubGraph);
  // Weakly connected components and strongly connected cores (WCCS / SCCS)
  TSnap::GetWccs(Graph, WCnComV);
  TSnap::GetWccSccCores(Graph, WCnComV, SCnComV);
  // Components connecting in to the SCCS and out from the SCCS (IN / OUT / TE)
  TSnap::GetIns(Graph, WCnComV, BCnComV);
  TSnap::GetOuts(Graph, WCnComV, FCnComV);
  TSnap::GetTes(Graph, WCnComV, TeCnComV);
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  if (giantOnly) {
    
    printf("\nSaving %s.GWCC and %s.GWCC...", BseFNm.CStr(), BseFNm.CStr());
    TSnap::SaveTxt(WCnComV[0].NIdV, TStr::Fmt("%s.GWCC", OutFNm.CStr()), "Giant weakly connected component (GWCC)", "NodeId");
    TSnap::SaveTxt(SCnComV[0].NIdV, TStr::Fmt("%s.GSCC", OutFNm.CStr()), "Giant strongly connected core (GSCC)", "NodeId");
    // TSnap::SaveTxt(WSubGraph, TStr::Fmt("%s.GWCC", OutFNm.CStr()), "Giant weakly connected component (GWCC)");
    // TSnap::SaveTxt(SSubGraph, TStr::Fmt("%s.GSCC", OutFNm.CStr()), "Giant strongly connected component (GSCC)");
    printf(" DONE\n");
    printf("  (GWCC) nodes: %d edges: %d\n", WSubGraph->GetNodes(), WSubGraph->GetEdges());
    printf("  (GSCC) nodes: %d edges: %d\n", SSubGraph->GetNodes(), SSubGraph->GetEdges());
    
    printf("\nSaving %s.IN and %s.OUT...", BseFNm.CStr(), BseFNm.CStr());
    TSnap::SaveTxt(BCnComV[0].NIdV, TStr::Fmt("%s.IN", OutFNm.CStr()), "Components connecting in to the GSCC (IN)", "NodeId");
    TSnap::SaveTxt(FCnComV[0].NIdV, TStr::Fmt("%s.OUT", OutFNm.CStr()), "Components connected out from the GSCC (OUT)", "NodeId");
    printf(" DONE\n");
    printf("  (IN) nodes: %d\n",BCnComV[0].NIdV.Len());
    printf("  (OUT) nodes: %d\n",FCnComV[0].NIdV.Len());
    
    printf("\nSaving %s.TE...", BseFNm.CStr());
    TSnap::SaveTxt(TeCnComV[0].NIdV, TStr::Fmt("%s.TE", OutFNm.CStr()), "Tendrils from the IN and to the OUT (TE)", "NodeId");
    printf(" DONE\n");
    printf("  (TE) nodes: %d\n",TeCnComV[0].NIdV.Len());
    
  } else {
    
    printf("\nSaving %s.WCCS and %s.SCCS...", BseFNm.CStr(), BseFNm.CStr());
    TSnap::SaveTxt(WCnComV, TStr::Fmt("%s.WCCS", OutFNm.CStr()), "Weakly connected components (WCCS)");
    TSnap::SaveTxt(SCnComV, TStr::Fmt("%s.SCCS", OutFNm.CStr()), "Strongly connected cores (SCCS)");
    printf(" DONE\n");
    printf("(WCCS)\n  comps: %d\n  nodes:\n", WCnComV.Len());
    for (CnComI = WCnComV.BegI(); CnComI < WCnComV.EndI(); CnComI++) {
      printf("    %d\n", CnComI->NIdV.Len());
    }
    printf("(SCCS)\n  cores: %d\n  nodes:\n", SCnComV.Len());
    for (CnComI = SCnComV.BegI(); CnComI < SCnComV.EndI(); CnComI++) {
      printf("    %d\n", CnComI->NIdV.Len());
    }
    
    printf("\nSaving %s.INS and %s.OUTS...", BseFNm.CStr(), BseFNm.CStr());
    TSnap::SaveTxt(BCnComV, TStr::Fmt("%s.INS", OutFNm.CStr()), "Components connecting in to the SCCS  (INS)");
    TSnap::SaveTxt(FCnComV, TStr::Fmt("%s.OUTS", OutFNm.CStr()), "Components connected out from the SCCS (OUTS)");
    printf(" DONE\n");
    printf("(INS)\n  number: %d\n  nodes:\n",BCnComV.Len());
    for (CnComI = BCnComV.BegI(); CnComI < BCnComV.EndI(); CnComI++) {
      printf("    %d\n", CnComI->NIdV.Len());
    }
    printf("(OUTS)\n  number: %d\n  nodes:\n",FCnComV.Len());
    for (CnComI = FCnComV.BegI(); CnComI < FCnComV.EndI(); CnComI++) {
      printf("    %d\n", CnComI->NIdV.Len());
    }
    
    printf("\nSaving %s.TES...", BseFNm.CStr());
    TSnap::SaveTxt(TeCnComV, TStr::Fmt("%s.TES", OutFNm.CStr()), "Tendrils from the INS and to the OUTS (TES)");
    printf(" DONE\n");
    printf("(TES)\n  number: %d\n  nodes:\n",BCnComV.Len());
    for (CnComI = TeCnComV.BegI(); CnComI < TeCnComV.EndI(); CnComI++) {
      printf("    %d\n", CnComI->NIdV.Len());
    }
    
  }
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}