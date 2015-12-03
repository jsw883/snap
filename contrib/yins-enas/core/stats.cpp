// Headers (?)

#include "stdafx.h"
#include "stats.h"

//#//////////////////////////////////////////////
/// Degrees

// Computes maximum degree (in / out)
int TSnap::GetMxInDeg(PNGraph& Graph) {
  typename PNGraph::TObj::TNodeI NI;
  int MxDeg = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    if (MxDeg < NI.GetInDeg()) {
      MxDeg = NI.GetInDeg();
    }
  }
  return MxDeg;
}
int TSnap::GetMxOutDeg(PNGraph& Graph) {
  typename PNGraph::TObj::TNodeI NI;
  int MxDeg = 0;
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    if (MxDeg < NI.GetOutDeg()) {
      MxDeg = NI.GetOutDeg();
    }
  }
  return MxDeg;
}

//#//////////////////////////////////////////////
/// Exact neighborhood function using BFS

double TSnap::InterpolateNF(const TIntV& NF, const double& p) {
  TIntV::TIter VI;
  int CumSum, depth;
  if (p == 0 || NF.Len() == 0 || NF.Len() == 1) {
    return 0;
  }
  CumSum = 0;
  for (VI = NF.BegI(); VI < NF.EndI(); VI++) {
    CumSum += VI->Val;
  }
  if (p == 1) {
    return CumSum;
  }
  double quantileThreshold = p * CumSum;
  CumSum = 0;
  for (VI = NF.BegI(), depth = 0; VI < NF.EndI(); VI++, depth++) {
    int Diff = VI-> Val;
    int LSum = CumSum;
    int RSum = CumSum + VI->Val;
    if (RSum >= quantileThreshold) {
      if (Diff > 0) {
        return depth + (quantileThreshold - LSum) / ((double) Diff); // linearly interpolated
      } else {
        return depth;
      }
    }
    CumSum += VI->Val;
  }
  return -1;
}

void TSnap::InterpolateINFH(const TIntIntVH& INFH, TIntIntH& QuantileH, const double& p) {
  TIntIntVH::TIter HI;
  for (HI = INFH.BegI(); HI < INFH.EndI(); HI++) {
    QuantileH.AddDat(HI.GetKey(), InterpolateNF(HI.GetDat(), p));
  }
}

void TSnap::GetNodesINFH(const TIntIntVH& INFH, TIntIntH& NodesH) {
  TIntIntVH::TIter HI;
  TIntV::TIter VI;
  int Nodes;
  for (HI = INFH.BegI(); HI < INFH.EndI(); HI++) {
    Nodes = 0;
    for (VI = HI.GetDat().BegI(); VI < HI.GetDat().EndI(); VI++) {
      Nodes += VI->Val;
    }
    NodesH.AddDat(HI.GetKey(), Nodes);
  }
}

void TSnap::GetDiameterINFH(const TIntIntVH& INFH, TIntIntH& DiameterH) {
  // InterpolateINFH(INFH, DiameterH, 1.0);
  TIntIntVH::TIter HI;
  for (HI = INFH.BegI(); HI < INFH.EndI(); HI++) {
    DiameterH.AddDat(HI.GetKey(), HI.GetDat().Len() - 1);
  }
}
