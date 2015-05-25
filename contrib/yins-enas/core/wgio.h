#ifndef core_wgio
#define core_wgio

// Headers (?)

// #include "Snap.h"
// #include "wgraph.h"

namespace TSnap {

// template <class TEdgeW> TPt<TWNEGraph<TEdgeW> > LoadWEdgeList(const TStr& InFNm, const int& SrcColId = 0, const int& DstColId = 1, const int& WColId = 2);

// template <class TEdgeW>
// TPt<TWNEGraph<TEdgeW> > LoadWEdgeList(const TStr& InFNm, const int& SrcColId, const int& DstColId, const int& WColId) {
//   TSsParser Ss(InFNm, ssfWhiteSep, true, true, true);
//   TPt<TWNEGraph<TEdgeW> > WGraph = TPt<TWNEGraph<TEdgeW> >::TObj::New();
//   int SrcNId, DstNId;
//   TEdgeW EdgeW;
//   while (Ss.Next()) {
//     if (! Ss.GetInt(SrcColId, SrcNId) || ! Ss.GetInt(DstColId, DstNId) || ! Ss.GetInt(WColId, EdgeW)) { continue; } // can't template TInt / TFlt (?)
//     if (! WGraph->IsNode(SrcNId)) { WGraph->AddNode(SrcNId); }
//     if (! WGraph->IsNode(DstNId)) { WGraph->AddNode(DstNId); }
//     WGraph->AddEdge(SrcNId, DstNId, EdgeW);
//   }
//   WGraph->Defrag();
//   return WGraph;
// }

// Loads the format saved by TSnap::SaveWEdgeList()
PIntWNEGraph LoadIntWEdgeList(const TStr& InFNm, const int& SrcColId = 0, const int& DstColId = 1, const int& WColId = 2);
PFltWNEGraph LoadFltWEdgeList(const TStr& InFNm, const int& SrcColId = 0, const int& DstColId = 1, const int& WColId = 2);

// Saves a weighted graph into a text file. Each line contains two columns and encodes a single edge: <source node id><tab><destination node id><tab><edge weight>
void SaveIntWEdgeList(const PIntWNEGraph& Graph, const TStr& OutFNm, const TStr& Desc);
void SaveFltWEdgeList(const PFltWNEGraph& Graph, const TStr& OutFNm, const TStr& Desc);

} // namespace TSnap

#endif
