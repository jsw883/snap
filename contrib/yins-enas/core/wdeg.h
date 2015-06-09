#ifndef core_wdeg
#define core_wdeg

// Headers (?)

// #include "Snap.h"

// TODO: generalise all weighted methods to THash<TInt, TEdgeW> (?)
namespace TSnap {

/// Computes weighted in degrees
template <class TEdgeW, template <class> class TGraph > void GetWInDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TEdgeW>& WInDegH);
/// Computes weighted out degrees
template <class TEdgeW, template <class> class TGraph > void GetWOutDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TEdgeW>& WOutDegH);
/// Computes weighted degrees
template <class TEdgeW, template <class> class TGraph > void GetWDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TEdgeW>& WDegH);

/// Computes weighted degree distributions (in / out / undirected)
template <class TEdgeW, template <class> class TGraph > void GetWDegVH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& DegVH);


// Computes weighted in degrees
template <class TEdgeW, template <class> class TGraph >
void GetWInDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TEdgeW>& DegH) {
  typename TGraph<TEdgeW>::TNodeI NI;
  DegH.Gen(WGraph->GetNodes());
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    DegH.AddDat(NI.GetId(), NI.GetWInDeg());
  }
}
// Computes weighted out degrees
template <class TEdgeW, template <class> class TGraph >
void GetWOutDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TEdgeW>& DegH) {
  typename TGraph<TEdgeW>::TNodeI NI;
  DegH.Gen(WGraph->GetNodes());
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    DegH.AddDat(NI.GetId(), NI.GetWOutDeg());
  }
}
// Computes weighted degrees
template <class TEdgeW, template <class> class TGraph >
void GetWDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& DegH) {
  typename TGraph<TEdgeW>::TNodeI NI;
  DegH.Gen(WGraph->GetNodes());
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    DegH.AddDat(NI.GetId(), NI.GetWDeg());
  }
}

// Computes weighted first degrees (in / out / undirected)
template <class TEdgeW, template <class> class TGraph >
void GetWDegVH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& DegVH) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TFltV DegV;
  DegVH.Gen(WGraph->GetNodes());
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    DegV.Clr();
    DegV.Add(NI.GetWInDeg());
    DegV.Add(NI.GetWOutDeg());
    DegV.Add(NI.GetWDeg());
    DegVH.AddDat(NI.GetId(), DegV);
  }
}

} // namespace TSnap

#endif