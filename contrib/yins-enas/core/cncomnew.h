#ifndef core_cncomnew
#define core_cncomnew

// Headers (?)

#include "galg.h"

//#//////////////////////////////////////////////
/// Bowtie connectivity

namespace TSnap {

// Returns the strongly connected cores (SCC) pertaining to the weakly connected
// components specified.
template <class PGraph>
void GetWccSccCores(PGraph& Graph, TCnComV& WCnComV, TCnComV& SCnComV) {
  PNGraph WSubGraph, SSubGraph;
  TCnComV::TIter WCnComI;
  TIntV SNIdV;
  SCnComV.Clr();
  for (WCnComI = WCnComV.BegI(); WCnComI < WCnComV.EndI(); WCnComI++) {
    WSubGraph = TSnap::GetSubGraph(Graph, WCnComI->NIdV);
    SSubGraph = TSnap::GetMxScc(WSubGraph);
    SSubGraph->GetNIdV(SNIdV);
    SCnComV.Add(TCnCom(SNIdV));
  }
}

// Returns the components connecting in to the strongly connected cores (IN)
// pertaining to the weakly connected components specified.
template <class PGraph>
void GetIns(const PGraph& Graph, const TCnComV& WCnComV, TCnComV& BCnComV) {
  PNGraph WSubGraph, SSubGraph;
  TCnComV::TIter WCnComI;
  TCnCom BCnCom;
  TIntV SNIdV;
  BCnComV.Clr();
  for (WCnComI = WCnComV.BegI(); WCnComI < WCnComV.EndI(); WCnComI++) {
    WSubGraph = TSnap::GetSubGraph(Graph, WCnComI->NIdV);
    SSubGraph = TSnap::GetMxScc(WSubGraph);
    TSnap::GetBcc(WSubGraph, SSubGraph, BCnCom); BCnCom.Sort();
    SSubGraph->GetNIdV(SNIdV); SNIdV.Sort();
    BCnCom.NIdV.Diff(SNIdV);
    BCnComV.Add(BCnCom);
  }  
}

// Returns the components connected out from the strongly connected cores (OUT)
// pertaining to the weakly connected components specified.
template <class PGraph>
void GetOuts(const PGraph& Graph, const TCnComV& WCnComV, TCnComV& FCnComV) {
  PNGraph WSubGraph, SSubGraph;
  TCnComV::TIter WCnComI;
  TCnCom FCnCom;
  TIntV SNIdV;
  FCnComV.Clr();
  for (WCnComI = WCnComV.BegI(); WCnComI < WCnComV.EndI(); WCnComI++) {
    WSubGraph = TSnap::GetSubGraph(Graph, WCnComI->NIdV);
    SSubGraph = TSnap::GetMxScc(WSubGraph);
    TSnap::GetFcc(WSubGraph, SSubGraph, FCnCom); FCnCom.Sort();
    SSubGraph->GetNIdV(SNIdV); SNIdV.Sort();
    FCnCom.NIdV.Diff(SNIdV);
    FCnComV.Add(FCnCom);
  }  
}

// Removes nodes in the subgraph from the graph (silent missing id handling).
// template <class PGraph>
// void DelSubGraphChecked(PGraph& Graph, const PGraph& SubGraph) {
//   PNGraph::TObj::TNodeI NI;
//   for (NI = SubGraph->BegNI(); NI < SubGraph->EndNI(); NI++) {
//     if (Graph->IsNode(NI.GetId())) { Graph->DelNode(NI.GetId()); }
//   }
// }

// Removes nodes with ids in NIdV from the graph (silent missing id handling).
// template <class PGraph>
// void DelNodesChecked(PGraph& Graph, const TIntV& NIdV) {
//   for (int n = 0; n < NIdV.Len(); n++) {
//      if (Graph->IsNode(NIdV[n])) { Graph->DelNode(NIdV[n]); }
//   }
// }

// Returns the tendrils from the IN and to the OUT (TE) or remaining components
// not connected to the strongly connected cores pertaining to the weakly
// connected components specified.
template <class PGraph>
void GetTes(const PGraph& Graph, const TCnComV& WCnComV, TCnComV& TECnComV) {
  PNGraph WSubGraph, SSubGraph;
  TVec<TCnCom>::TIter WCnComI;
  TCnCom BCnCom, FCnCom;
  TIntV WNIdV, SNIdV, TENIdV;
  TECnComV.Clr();
  for (WCnComI = WCnComV.BegI(); WCnComI < WCnComV.EndI(); WCnComI++) {
    WSubGraph = TSnap::GetSubGraph(Graph, WCnComI->NIdV);
    SSubGraph = TSnap::GetMxScc(WSubGraph);
    TSnap::GetBcc(WSubGraph, SSubGraph, BCnCom); BCnCom.Sort();
    TSnap::GetFcc(WSubGraph, SSubGraph, FCnCom); FCnCom.Sort();
    WSubGraph->GetNIdV(WNIdV); WNIdV.Sort();
    SSubGraph->GetNIdV(SNIdV); SNIdV.Sort();
    WNIdV.Diff(SNIdV, TENIdV);
    TENIdV.Diff(BCnCom.NIdV);
    TENIdV.Diff(FCnCom.NIdV);
    TECnComV.Add(TCnCom(TENIdV));
  }  
}

} // namespace TSnap

// Removes nodes with ids in ValV from SrcV (applies sorting as required).
// void DiffSorted(TIntV& SrcV, TIntV& ValV) {
//   if (!SrcV.IsSorted()) SrcV.Sort();
//   if (!ValV.IsSorted()) ValV.Sort();
//   SrcV.Diff(ValV);
// }

// Removes nodes with ids in ValV from SrcV (applies sorting as required).
// void DiffSorted(TIntV& SrcV, TIntV& ValV, TIntV& DstV) {
//   if (!SrcV.IsSorted()) SrcV.Sort();
//   if (!ValV.IsSorted()) ValV.Sort();
//   SrcV.Diff(ValV, DstV);
// }

//#//////////////////////////////////////////////
/// Connectivity summary

namespace TSnap {

void printCnComVSummary(const TCnComV& CnComV, const TStr& Desc = "CnComV Summary\n--------------");

} // namespace TSnap

#endif
