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
/// Convert NF

// Aggregate neighborhood depth counts to NF (in place)
void TSnap::ConvertNeighborhoodNF(TUInt64V& Neighborhood) {
  // Variables
  int depth;
  // Aggregate
  for (depth = 1; depth < Neighborhood.Len(); depth++) {
    Neighborhood[depth] += Neighborhood[depth - 1];
  }
}

// Reverse aggregation from NF to neighborhood (in place)
void TSnap::ConvertNFNeighborhood(TUInt64V& NF) {
  // Variables
  int depth;
  // Reverse
  for (depth = NF.Len() - 1; depth > 0; depth--) {
    NF[depth] -= NF[depth - 1];
  }
}

// Convert neighborhoods to exact NF (aggregate and cumulate)
void TSnap::ConvertSubsetNeighborhoodHSubsetINFH(THash<TInt, TUInt64V>& NeighborhoodH) {
  // Variables
  THash<TInt, TUInt64V>::TIter HI;
  // For each node in INFH (equivalently, NIdV)
  for (HI = NeighborhoodH.BegI(); HI < NeighborhoodH.EndI(); HI++) {
    TUInt64V& Neighborhood = HI.GetDat();
    // Convert each neighborhood to NF (in place)
    ConvertNeighborhoodNF(Neighborhood);
  }
}

// Convert neighborhoods to exact NF (aggregate and cumulate)
void TSnap::ConvertSubsetINFHSubsetNeighborhoodH(THash<TInt, TUInt64V>& INFH) {
  // Variables
  THash<TInt, TUInt64V>::TIter HI;
  // For each node in INFH (equivalently, NIdV)
  for (HI = INFH.BegI(); HI < INFH.EndI(); HI++) {
    TUInt64V& INF = HI.GetDat();
    // Convert each neighborhood to NF (in place)
    ConvertNFNeighborhood(INF);
  }
}

// Convert neighborhoods to exact NF (aggregate and cumulate)
void TSnap::ConvertSubsetNeighborhoodHSubsetNF(const THash<TInt, TUInt64V>& NeighborhoodH, TUInt64V& NF) {
  // Variables
  THash<TInt, TUInt64V>::TIter HI;
  // Clear NF
  NF.Clr();
  // For each node in NeighborhoodH (equivalently, NIdV)
  for (HI = NeighborhoodH.BegI(); HI < NeighborhoodH.EndI(); HI++) {
    const TUInt64V& Neighborhood = HI.GetDat();
    // Ensure that all depths exist in the NFH
    if (Neighborhood.Len() > NF.Len()) {
      for (int depth = NF.Len(); depth < Neighborhood.Len(); depth++) {
        NF.Add(0);
      }
    }
    // Increment depth counts
    for (int depth = 0; depth < Neighborhood.Len(); depth++) {
      NF[depth] += (TUInt64) Neighborhood[depth];
    }
  }
  // Aggregate
  ConvertNeighborhoodNF(NF);
}

// Convert neighborhoods to exact NF (aggregate and cumulate)
void TSnap::ConvertSubsetINFHSubsetNF(THash<TInt, TUInt64V>& INFH, TUInt64V& NF) {
  ConvertSubsetINFHSubsetNeighborhoodH(INFH);
  ConvertSubsetNeighborhoodHSubsetNF(INFH, NF);
  ConvertSubsetNeighborhoodHSubsetINFH(INFH);
}

//#//////////////////////////////////////////////
/// Interpolate NF

double TSnap::InterpolateNF(const TUInt64V& NF, const double& p) {
  TIntV::TIter VI;
  TUInt64 Diff;
  int depth;
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

void TSnap::InterpolateINFH(const THash<TInt, TUInt64V>& INFH, TIntFltH& QuantileH, const double& p) {
  THash<TInt, TUInt64V>::TIter HI;
  for (HI = INFH.BegI(); HI < INFH.EndI(); HI++) {
    QuantileH.AddDat(HI.GetKey(), InterpolateNF(HI.GetDat(), p));
  }
}

void TSnap::GetNodesINFH(const THash<TInt, TUInt64V>& INFH, TIntIntH& NodesH) {
  // InterpolateINFH(INFH, NodesH, 0.0);
  THash<TInt, TUInt64V>::TIter HI;
  for (HI = INFH.BegI(); HI < INFH.EndI(); HI++) {
    NodesH.AddDat(HI.GetKey(), (int) HI.GetDat().Last());
  }
}

void TSnap::GetDiameterINFH(const THash<TInt, TUInt64V>& INFH, TIntIntH& DiameterH) {
  // InterpolateINFH(INFH, DiameterH, 1.0);
  THash<TInt, TUInt64V>::TIter HI;
  for (HI = INFH.BegI(); HI < INFH.EndI(); HI++) {
    DiameterH.AddDat(HI.GetKey(), (int) HI.GetDat().Len() - 1);
  }
}
