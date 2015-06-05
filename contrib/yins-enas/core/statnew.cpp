// Headers (?)

#include "stdafx.h"
#include "statnew.h"

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
