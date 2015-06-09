// Headers (?)

#include "stdafx.h"
#include "deg.h"

// Returns k deep degrees for node CtrNId (in / out / undirected)
int TSnap::GetkInDeg(const PNGraph& Graph, const int CtrNId, const int k) {
  PNGraph Ego = TNGraph::New(); Ego->AddNode(CtrNId);
  return(GetBDeg(Graph, Ego, k));
}
int TSnap::GetkOutDeg(const PNGraph& Graph, const int CtrNId, const int k) {
  PNGraph Ego = TNGraph::New(); Ego->AddNode(CtrNId);
  return(GetFDeg(Graph, Ego, k));
}
int TSnap::GetkDeg(const PNGraph& Graph, const int CtrNId, const int k) {
  PNGraph Ego = TNGraph::New(); Ego->AddNode(CtrNId);
  return(GetUDeg(Graph, Ego, k));
}

// Returns k deep degrees for the graph as DegVH (in / out / undirected)
void TSnap::GetkInDegSeqH(const PNGraph& Graph, TIntIntVH& DegVH, const int k) {
  typename PNGraph::TObj::TNodeI NI;
  TIntV DegV;
  DegVH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    DegV.Clr();
    for (int i = 1; i <= k; i++) {
      DegV.Add(TSnap::GetkInDeg(Graph, NI.GetId(), i));
    }
    DegVH.AddDat(NI.GetId(), DegV);
  }
}
void TSnap::GetkOutDegSeqH(const PNGraph& Graph, TIntIntVH& DegVH, const int k) {
  typename PNGraph::TObj::TNodeI NI;
  TIntV DegV;
  DegVH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    DegV.Clr();
    for (int i = 1; i <= k; i++) {
      DegV.Add(TSnap::GetkOutDeg(Graph, NI.GetId(), i));
    }
    DegVH.AddDat(NI.GetId(), DegV);
  }
}
void TSnap::GetkDegSeqH(const PNGraph& Graph, TIntIntVH& DegVH, const int k) {
  typename PNGraph::TObj::TNodeI NI;
  TIntV DegV;
  DegVH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    DegV.Clr();
    for (int i = 1; i <= k; i++) {
      DegV.Add(TSnap::GetkDeg(Graph, NI.GetId(), i));
    }
    DegVH.AddDat(NI.GetId(), DegV);
  }
}
