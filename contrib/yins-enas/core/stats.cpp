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
  int Diff, depth;
  if (p == 0 || NF.Len() == 0 || NF.Len() == 1) {
    return 0;
  }
  if (p == 1) {
    return NF.Len() - 1;
  }
  double quantileThreshold = NF[0] + p * (NF.Last() - NF[0]);
  for (depth = 0; depth < NF.Len() - 1; depth++) {
    if (NF[depth + 1] >= quantileThreshold) {
      Diff = NF[depth + 1] - NF[depth];
      if (Diff > 0) {
        return depth + (quantileThreshold - NF[depth]) / ((double) Diff); // linearly interpolated
      } else {
        return depth;
      }
    }
  }
  return -1;
}

void TSnap::InterpolateINFH(const TIntIntVH& INFH, TIntIntH& QuantileH, const double& p) {
  TIntIntVH::TIter HI;
  for (HI = INFH.BegI(); HI < INFH.EndI(); HI++) {
    QuantileH.AddDat(HI.GetKey(), InterpolateNF(HI.GetDat(), p));
  }
}

void TSnap::GetDiameterINFH(const TIntIntVH& INFH, TIntIntH& DiameterH) {
  // InterpolateINFH(INFH, DiameterH, 1.0);
  TIntIntVH::TIter HI;
  for (HI = INFH.BegI(); HI < INFH.EndI(); HI++) {
    DiameterH.AddDat(HI.GetKey(), HI.GetDat().Len() - 1);
  }
}
