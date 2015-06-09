#ifndef core_wstats
#define core_wstats

// Headers (?)

// #include "Snap.h"
// #include "wgraph.h"

namespace TSnap {

// Computes maximum degree (in / out / undirected)
template <class TEdgeW, template <class> class TGraph >
TEdgeW GetMxWInDeg(const TPt<TGraph<TEdgeW> >& Graph) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TEdgeW WDeg = 0, MxWDeg = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    WDeg = NI.GetWInDeg();
    if (MxWDeg < WDeg) {
      MxWDeg = WDeg;
    }
  }
  return MxWDeg;
}
template <class TEdgeW, template <class> class TGraph >
TEdgeW GetMxWOutDeg(const TPt<TGraph<TEdgeW> >& Graph) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TEdgeW WDeg = 0, MxWDeg = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    WDeg = NI.GetWOutDeg();
    if (MxWDeg < WDeg) {
      MxWDeg = WDeg;
    }
  }
  return MxWDeg;
}
template <class TEdgeW, template <class> class TGraph >
TEdgeW GetMxWDeg(const TPt<TGraph<TEdgeW> >& Graph) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TEdgeW WDeg = 0, MxWDeg = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    WDeg = NI.GetWDeg();
    if (MxWDeg < WDeg) {
      MxWDeg = WDeg;
    }
  }
  return MxWDeg;
}

// Computes GINI coefficient on egonet only (edges into and out of the egonet are NOT considered)
template <class TEdgeW, template <class> class TGraph >
double GetGiniCoefficient(const TPt<TGraph<TEdgeW> >& Graph) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TFltV DegV;
  const int n = Graph->GetNodes();
  // DegV.Gen(n); // NOTE: don't use Gen() and Sort() on the same object (!)
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    DegV.Add(NI.GetWInDeg()); // might need to change this (in / out / undirected)
  }
  DegV.Sort();
  int i = 0;
  double numerator = 0, denominator = 0;
  for (i = 0; i < n; i++) {
    numerator += (i + 1)*DegV[i];
    denominator += DegV[i];
  }
  return(double(2*numerator) / double(n*denominator) - double(n + 1) / double(n));
}

// Computes GINI coefficient of egonet as a child of the parent graph (edges into and out of the egonet ARE considered)
double GetGiniCoefficient(const TIntFltH DegH, const TIntV NIdV);

} // namespace TSnap

#endif