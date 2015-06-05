#ifndef core_statnew
#define core_statnew

// Headers (?)

// #include "Snap.h"

namespace TSnap {

// Computes global clustering coefficient (need to check this for method)
template<class PGraph> double GetGlobClustCf(PGraph& Graph, int SampleNodes = -1);
// Computes average clustering coefficient (need to check this for method)
template<class PGraph> double GetAvClustCf(PGraph& Graph, int SampleNodes = -1);

// Computes maximum degree (undirected)
template <class PGraph>
int GetMxDeg(PGraph& Graph) {
  typename PGraph::TObj::TNodeI NI;
  int MxDeg = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    if (MxDeg < NI.GetDeg()) {
      MxDeg = NI.GetDeg();
    }
  }
  return MxDeg;
}

// Computes given percentiles of the shortest path cumulative density (multiple runs of ANF)
template<class PGraph>
void GetAnfEffDiam(const PGraph& Graph, const bool& IsDir, const TFltV& Percentiles, TFltV& PValues, const int NRuns = 1, int NApprox = -1) {
  typename TFltV::TIter PI;
  typename TMomV::TIter MomI;
  TMomV MomV;
  TMom::NewV(MomV, Percentiles.Len());
  if (NApprox == -1) {
    if (Graph->GetNodes() < 100000) { NApprox = 64; }
    else if (Graph->GetNodes() < 1000000) { NApprox = 32; }
    else { NApprox = 16; }
  }
  for (int r = 0; r < NRuns; r++) {
    for (MomI = MomV.BegI(), PI = Percentiles.BegI(); MomI < MomV.EndI() || PI < Percentiles.EndI(); MomI++, PI++) {
      (*MomI)->Add(TSnap::GetAnfEffDiam(Graph, IsDir, PI->Val, NApprox));
    }
  }
  TMom::DefV(MomV);
  for (MomI = MomV.BegI(); MomI < MomV.EndI(); MomI++) {
    PValues.Add((*MomI)->GetMean());
  }
}

// Computes global clustering coefficient (need to check this for method)
template<class PGraph>
double GetGlobClustCf(PGraph& Graph, int SampleNodes) {
  int64 ClosedTriadsX, OpenTriadsX;
  GetTriads(Graph, ClosedTriadsX, OpenTriadsX, SampleNodes);
  return double(ClosedTriadsX) / double(ClosedTriadsX + OpenTriadsX);
}

// Computes average clustering coefficient (need to check this for method)
template<class PGraph> double GetAvClustCf(PGraph& Graph, int SampleNodes = -1) {
  return GetClustCf(Graph, SampleNodes = -1);
}

} // namespace TSnap

namespace TSnap {

// Computes maximum degree (in / out)
int GetMxInDeg(PNGraph& Graph);
int GetMxOutDeg(PNGraph& Graph);

}

#endif
