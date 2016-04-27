#ifndef core_wstats
#define core_wstats

// Headers (?)

// #include "Snap.h"
// #include "wgraph.h"

//#//////////////////////////////////////////////
/// Clustering coefficients

namespace TSnap {

// Computes local clustering coefficient for each node in NIdV (returns average)
template <class TEdgeW, template <class> class TGraph > double GetWAvLocalClustCoeff(const TPt<TGraph<TEdgeW> >& Graph, const TIntV& NIdV, TIntFltH& NIdClustCoeffH);
// Computes average local clustering coefficient (entire graph)
template <class TEdgeW, template <class> class TGraph > double GetWAvLocalClustCoeff(const TPt<TGraph<TEdgeW> >& Graph);

// Compute directed binary local clustering coefficients as defined in Clustering in Complex Directed Networks [Fagiolo, 2007]
template <class TEdgeW, template <class> class TGraph > double GetWDirLocalClustCoeff(const TPt<TGraph<TEdgeW> >& Graph, const int& NId, double& CycleCoeff, double& MidCoeff, double& InCoeff, double& OutCoeff);
// Should work for undirected graphs by overcounting 
template <class TEdgeW, template <class> class TGraph > double GetWDirLocalClustCoeff(const TPt<TGraph<TEdgeW> >& Graph, const int& NId);

// Computes local clustering coefficient for each node in NIdV (returns average)
template <class TEdgeW, template <class> class TGraph >
double GetWAvLocalClustCoeff(const TPt<TGraph<TEdgeW> >& Graph, const TIntV& NIdV, TIntFltH& NIdClustCoeffH) {
  // Variables
  // PGraph::TObj::TNodeI NI;
  TIntV::TIter NI;
  double Coeff, AvCoeff = 0;
  // Check empty
  NIdClustCoeffH.Clr();
  // For each node in NIdV
  for (NI = NIdV.BegI(); NI < NIdV.EndI(); NI++) {
    const int& NId = NI->Val;
    Coeff = GetWDirLocalClustCoeff(Graph, NId);
    NIdClustCoeffH.AddDat(NId, Coeff);
    AvCoeff += Coeff;
  }
  return AvCoeff / ((double) NIdV.Len());
}

// Computes average local clustering coefficient (entire graph)
template <class TEdgeW, template <class> class TGraph >
double GetWAvLocalClustCoeff(const TPt<TGraph<TEdgeW> >& Graph) {
  // Variables
  TIntV NIdV; Graph->GetNIdV(NIdV);
  TIntFltH NIdClustCoeffH;
  return GetWAvLocalClustCoeff(Graph, NIdV, NIdClustCoeffH);
}

// Compute directed binary local clustering coefficients as defined in Clustering in Complex Directed Networks [Fagiolo, 2007]
template <class TEdgeW, template <class> class TGraph >
double GetWDirLocalClustCoeff(const TPt<TGraph<TEdgeW> >& Graph, const int& NId, double& CycleCoeff, double& MidCoeff, double& InCoeff, double& OutCoeff) {
  
  typename TGraph<TEdgeW>::TNodeI NI = Graph->GetNI(NId);
  // Variables
  TIntSet OutS, InS;  // Hash backed
  double CycleW = 0, MidW = 0, InW = 0, OutW = 0;
  double w1, w2, w3;
  int OutNId, InNId;
  int OutDeg = NI.GetOutDeg(), InDeg = NI.GetInDeg(), Deg = OutDeg + InDeg, BiDeg = 0;
  TEdgeW MxW = Graph->GetMxW();
  // Collect nodes in OUT
  for (int e = 0; e < OutDeg; e++) {
    OutS.AddKey(NI.GetOutNId(e));
  }
  // Collect nodes in IN
  for (int e = 0; e < InDeg; e++) {
    InNId = NI.GetInNId(e);
    InS.AddKey(InNId);
    if (OutS.IsKey(InNId)) {
      BiDeg++;
    }
  }
  // Edge cases
  if (Deg < 2 || InDeg*OutDeg*BiDeg == 1) {
    CycleCoeff = MidCoeff = InCoeff = OutCoeff = 0;
    return 0;
  }
  // OUT
  for (int j = 0; j < OutS.Len(); j++) {
    NI = Graph->GetNI(OutS.GetKey(j));
    // OUT x OUT (unique)
    for (int k = j + 1; k < OutS.Len(); k++) {
      OutNId = OutS.GetKey(k);
      if (NI.IsOutNId(OutNId)) {
        // i --> j, i --> k, j --> k,
        // i = Graph->GetNI(NId)
        // j = Graph->GetNI(OutS.GetKey(j))
        // k = Graph->GetNI(OutS.GetKey(k))
        w1 = ((double) Graph->GetEW(NId, NI.GetId())) / ((double) MxW);
        w2 = ((double) Graph->GetEW(NId, OutNId)) / ((double) MxW);
        w3 = ((double) Graph->GetEW(NI.GetId(), OutNId)) / ((double) MxW);
        OutW += pow(w1*w2*w3, 1.0 / 3.0);
      }
      if (NI.IsInNId(OutNId)) {
        // i --> j, i --> k, j <-- k,
        // i = Graph->GetNI(NId)
        // j = Graph->GetNI(OutS.GetKey(j))
        // k = Graph->GetNI(OutS.GetKey(k))
        w1 = ((double) Graph->GetEW(NId, NI.GetId())) / ((double) MxW);
        w2 = ((double) Graph->GetEW(NId, OutNId)) / ((double) MxW);
        w3 = ((double) Graph->GetEW(OutNId, NI.GetId())) / ((double) MxW);
        OutW += pow(w1*w2*w3, 1.0 / 3.0);
      }
    }
    // OUT x IN (complete)
    for (int k = 0; k < InS.Len(); k++) {
      InNId = InS.GetKey(k);
      if (NI.IsOutNId(InNId)) {
        // i --> j, i <-- k, j --> k,
        // i = Graph->GetNI(NId)
        // j = Graph->GetNI(OutS.GetKey(j))
        // k = Graph->GetNI(InS.GetKey(k))
        w1 = ((double) Graph->GetEW(NId, NI.GetId())) / ((double) MxW);
        w2 = ((double) Graph->GetEW(InNId, NId)) / ((double) MxW);
        w3 = ((double) Graph->GetEW(NI.GetId(), InNId)) / ((double) MxW);
        CycleW += pow(w1*w2*w3, 1.0 / 3.0);
      }
      if (NI.IsInNId(InNId)) {
        // i --> j, i <-- k, j <-- k,
        // i = Graph->GetNI(NId)
        // j = Graph->GetNI(OutS.GetKey(j))
        // k = Graph->GetNI(InS.GetKey(k))
        w1 = ((double) Graph->GetEW(NId, NI.GetId())) / ((double) MxW);
        w2 = ((double) Graph->GetEW(InNId, NId)) / ((double) MxW);
        w3 = ((double) Graph->GetEW(InNId, NI.GetId())) / ((double) MxW);
        MidW += pow(w1*w2*w3, 1.0 / 3.0);
      }
    }
  }
  // IN
  for (int j = 0; j < InS.Len(); j++) {
    NI = Graph->GetNI(InS.GetKey(j));
    // IN x IN (unique)
    for (int k = j + 1; k < InS.Len(); k++) {
      InNId = InS.GetKey(k);
      if (NI.IsOutNId(InNId)) {
        // i <-- j, i <-- k, j --> k,
        // i = Graph->GetNI(NId)
        // j = Graph->GetNI(InS.GetKey(j))
        // k = Graph->GetNI(InS.GetKey(k))
        w1 = ((double) Graph->GetEW(NI.GetId(), NId)) / ((double) MxW);
        w2 = ((double) Graph->GetEW(InNId, NId)) / ((double) MxW);
        w3 = ((double) Graph->GetEW(NI.GetId(), InNId)) / ((double) MxW);
        InW += pow(w1*w2*w3, 1.0 / 3.0);
      }
      if (NI.IsInNId(InNId)) {
        // i <-- j, i <-- k, j <-- k,
        // i = Graph->GetNI(NId)
        // j = Graph->GetNI(InS.GetKey(j))
        // k = Graph->GetNI(InS.GetKey(k))
        w1 = ((double) Graph->GetEW(NI.GetId(), NId)) / ((double) MxW);
        w2 = ((double) Graph->GetEW(InNId, NId)) / ((double) MxW);
        w3 = ((double) Graph->GetEW(InNId, NI.GetId())) / ((double) MxW);
        InW += pow(w1*w2*w3, 1.0 / 3.0);
      }
    }
  }
  
  // Compute directed coefficients
  CycleCoeff = InDeg*OutDeg == 0 ? 0 : ((double) CycleW) / ((double) InDeg*OutDeg - BiDeg);
  MidCoeff = InDeg*OutDeg == 0 ? 0 : ((double) MidW) / ((double) InDeg*OutDeg - BiDeg);
  InCoeff = InDeg < 2 ? 0 : ((double) InW) / ((double) InDeg*(InDeg - 1));
  OutCoeff = OutDeg < 2 ? 0 : ((double) OutW) / ((double) OutDeg*(OutDeg - 1));
  // Return undirected coefficient for overloading
  return ((double) CycleW + MidW + InW + OutW) /
    ((double) Deg*(Deg - 1) - 2*BiDeg);
  
}

// Should work for undirected graphs by overcounting 
template <class TEdgeW, template <class> class TGraph >
double GetWDirLocalClustCoeff(const TPt<TGraph<TEdgeW> >& Graph, const int& NId) {
  
  double CycleCoeff, MidCoeff, InCoeff, OutCoeff;
  return GetWDirLocalClustCoeff(Graph, NId, CycleCoeff, MidCoeff, InCoeff, OutCoeff);
  
}

} // namespace TSnap

//#//////////////////////////////////////////////
/// Degrees

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

} // namespace TSnap

//#//////////////////////////////////////////////
/// GINI

namespace TSnap {

// Computes GINI coefficient of egonet as a child of the parent graph (edges into and out of the egonet ARE considered)
double GetGiniCoefficient(const TIntFltH DegH, const TIntV NIdV);

} // namespace TSnap

#endif