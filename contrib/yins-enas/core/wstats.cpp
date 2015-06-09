// Headers (?)

#include "stdafx.h"
#include "wstats.h"

// Computes GINI coefficient of egonet as a subset of the parent graph (edges into and out of the egonet ARE considered)
double TSnap::GetGiniCoefficient(const TIntFltH DegH, const TIntV NIdV) {
  typename TIntV::TIter VI;
  typename TFltV::TIter DI;
  TFltV DegV;
  const int n = NIdV.Len();
  // DegV.Gen(n); // NOTE: don't use Gen() and Sort() on the same object (!)
  for (VI = NIdV.BegI(); VI < NIdV.EndI(); VI++) {
    DegV.Add(DegH.GetDat(VI->Val)); // might need to change this (in / out / undirected)
  }
  DegV.Sort();
  int i = 0;
  double numerator = 0.0, denominator = 0.0;
  for (DI = DegV.BegI(); DI < DegV.EndI(); DI++, i++) {
    numerator += (i + 1)*DegV[i];
    denominator += DegV[i];
  }
  return(double(2*numerator) / double(n*denominator) - double(n + 1) / double(n));
}
