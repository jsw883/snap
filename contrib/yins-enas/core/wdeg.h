#ifndef core_wdeg
#define core_wdeg

// Headers (?)

#include "galg.h"

namespace TSnap {

/// Computes weighted in degrees
template <class TEdgeW, template <class> class TGraph > void GetWInDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TEdgeW>& WDegH);
/// Computes weighted out degrees
template <class TEdgeW, template <class> class TGraph > void GetWOutDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TEdgeW>& WDegH);
/// Computes weighted degrees
template <class TEdgeW, template <class> class TGraph > void GetWDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TEdgeW>& WDegH);

/// Computes weighted degree distributions (in / out / undirected)
template <class TEdgeW, template <class> class TGraph > void GetWDegVH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH);


// Computes weighted in degrees
template <class TEdgeW, template <class> class TGraph >
void GetWInDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TEdgeW>& WDegH) {
  typename TGraph<TEdgeW>::TNodeI NI;
  WDegH.Gen(WGraph->GetNodes());
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    WDegH.AddDat(NI.GetId(), NI.GetWInDeg());
  }
}
// Computes weighted out degrees
template <class TEdgeW, template <class> class TGraph >
void GetWOutDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TEdgeW>& WDegH) {
  typename TGraph<TEdgeW>::TNodeI NI;
  WDegH.Gen(WGraph->GetNodes());
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    WDegH.AddDat(NI.GetId(), NI.GetWOutDeg());
  }
}
// Computes weighted degrees
template <class TEdgeW, template <class> class TGraph >
void GetWDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegH) {
  typename TGraph<TEdgeW>::TNodeI NI;
  WDegH.Gen(WGraph->GetNodes());
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    WDegH.AddDat(NI.GetId(), NI.GetWDeg());
  }
}

// Computes weighted first degrees (in / out / undirected)
template <class TEdgeW, template <class> class TGraph >
void GetWDegVH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TFltV WDegV;
  WDegVH.Gen(WGraph->GetNodes());
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    WDegV.Clr();
    WDegV.Add(NI.GetWInDeg());
    WDegV.Add(NI.GetWOutDeg());
    WDegV.Add(NI.GetWDeg());
    WDegVH.AddDat(NI.GetId(), WDegV);
  }
}

} // namespace TSnap

namespace TSnap {

/// Returns k in degree distribution using fixed memory BFS
template <class PGraph> void GetkWInDegSeqH(const PGraph& WGraph, TIntIntVH& WDegVH, const int k);
/// Returns k out degree distribution using fixed memory BFS
template <class PGraph> void GetkWOutDegSeqH(const PGraph& WGraph, TIntIntVH& WDegVH, const int k);
/// Returns k degree distribution using fixed memory BFS
template <class PGraph> void GetkWDegSeqH(const PGraph& WGraph, TIntIntVH& WDegVH, const int k);

template <class PGraph>
class TFixedMemorykWDeg : public TFixedMemoryBFS<PGraph> {
public:
  // Backward / forward visitor (degree only)
  class TkWDegVisitor {
  public:
    PGraph Graph;
    TEdgeDir Dir;
    int WDeg;
    TIntTrV EdgeV;
  public:
    TkWDegVisitor(const PGraph& GraphArg) : Graph(GraphArg), Dir(edUnDirected), WDeg(0) { }
    void SetEdgeDir(const TEdgeDir& DirArg) { Dir = DirArg; }
    void AddW(const int& SrcNId, const int& edge, const int& DstNId) {
      switch(Dir) {
        case edInDirected: WDeg += Graph->GetNI(SrcNId).GetInEW(edge); break;
        case edOutDirected: WDeg += Graph->GetNI(SrcNId).GetOutEW(edge); break;
        case edUnDirected: WDeg += Graph->GetNI(SrcNId).GetNbrEW(edge); break;
      }
    }
    void DiscoverNode(int NId, int depth) { }
    void FinishNode(const int& NId, int depth) { }
    void ExamineEdge(const int& SrcNId, const int& edge, const int& DstNId) { }
    void TreeEdge(const int& SrcNId, const int& edge, const int& DstNId) {
      AddW(SrcNId, edge, DstNId);
    }
    void BackEdge(const int& SrcNId, const int& edge, const int& DstNId) {
      AddW(SrcNId, edge, DstNId);
    }
    void ForwardEdge(const int& SrcNId, const int& edge, const int& DstNId) {
      AddW(SrcNId, edge, DstNId);
    }
    void Clr() {
      WDeg = 0;
      Dir = edUnDirected;
    }
  };
private:
  TkWDegVisitor Visitor;
public:
  TFixedMemorykWDeg(const PGraph& GraphArg) : TFixedMemoryBFS<PGraph>(GraphArg), Visitor(TkWDegVisitor(GraphArg)) { }
  int GetkWInDeg(const int NId, const int k);
  int GetkWOutDeg(const int NId, const int k);
  int GetkWDeg(const int NId, const int k);
  void GetkWInDegSeqH(TIntIntVH& WDegVH, const int k);
  void GetkWOutDegSeqH(TIntIntVH& WDegVH, const int k);
  void GetkWDegSeqH(TIntIntVH& WDegVH, const int k);
  void Clr(const bool& DoDel = false);
};

template <class PGraph>
int TFixedMemorykWDeg<PGraph>::GetkWInDeg(const int NId, const int k) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial 0
  Visitor.SetEdgeDir(edInDirected);
  this->GetBfsVisitor<TkWDegVisitor>(Ego, Visitor, edInDirected, k);
  return(Visitor.WDeg);
}
template <class PGraph>
int TFixedMemorykWDeg<PGraph>::GetkWOutDeg(const int NId, const int k) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial 0
  Visitor.SetEdgeDir(edOutDirected);
  this->GetBfsVisitor<TkWDegVisitor>(Ego, Visitor, edOutDirected, k);
  return(Visitor.WDeg);
}
template <class PGraph>
int TFixedMemorykWDeg<PGraph>::GetkWDeg(const int NId, const int k) {
  PGraph Ego = PGraph::TObj::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial 0
  Visitor.SetEdgeDir(edUnDirected);
  this->GetBfsVisitor<TkWDegVisitor>(Ego, Visitor, edUnDirected, k);
  return(Visitor.WDeg);
}

template <class PGraph>
void TFixedMemorykWDeg<PGraph>::GetkWInDegSeqH(TIntIntVH& WDegVH, const int k) {
  typename PGraph::TObj::TNodeI NI;
  TIntV WDegV;
  WDegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    WDegV.Clr();
    for (int i = 1; i <= k; i++) {
      WDegV.Add(GetkWInDeg(NI.GetId(), i));
    }
    WDegVH.AddDat(NI.GetId(), WDegV);
  }
}
template <class PGraph>
void TFixedMemorykWDeg<PGraph>::GetkWOutDegSeqH(TIntIntVH& WDegVH, const int k) {
  typename PGraph::TObj::TNodeI NI;
  TIntV WDegV;
  WDegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    WDegV.Clr();
    for (int i = 1; i <= k; i++) {
      WDegV.Add(GetkWOutDeg(NI.GetId(), i));
    }
    WDegVH.AddDat(NI.GetId(), WDegV);
  }
}
template <class PGraph>
void TFixedMemorykWDeg<PGraph>::GetkWDegSeqH(TIntIntVH& WDegVH, const int k) {
  typename PGraph::TObj::TNodeI NI;
  TIntV WDegV;
  WDegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    WDegV.Clr();
    for (int i = 1; i <= k; i++) {
      WDegV.Add(GetkWDeg(NI.GetId(), i));
    }
    WDegVH.AddDat(NI.GetId(), WDegV);
  }
}

template <class PGraph>
void TFixedMemorykWDeg<PGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<PGraph>::Clr(DoDel);
  Visitor.Clr(); // resets the degree visitor to the initial -1
}

template <class PGraph>
void GetkWInDegSeqH(const PGraph& WGraph, TIntIntVH& WDegVH, const int k) {
  TFixedMemorykWDeg<PGraph> FixedMemorykWDeg(WGraph);
  FixedMemorykWDeg.GetkWInDegSeqH(WDegVH, k);
}
template <class PGraph>
void GetkWOutDegSeqH(const PGraph& WGraph, TIntIntVH& WDegVH, const int k) {
  TFixedMemorykWDeg<PGraph> FixedMemorykWDeg(WGraph);
  FixedMemorykWDeg.GetkWOutDegSeqH(WDegVH, k);
}
template <class PGraph>
void GetkWDegSeqH(const PGraph& WGraph, TIntIntVH& WDegVH, const int k) {
  TFixedMemorykWDeg<PGraph> FixedMemorykWDeg(WGraph);
  FixedMemorykWDeg.GetkWDegSeqH(WDegVH, k);
}

} // namespace TSnap

#endif