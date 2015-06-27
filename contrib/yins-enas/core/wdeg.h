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
template <class TEdgeW, template <class> class TGraph > void GetkWInDegSeqH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int k);
/// Returns k out degree distribution using fixed memory BFS
template <class TEdgeW, template <class> class TGraph > void GetkWOutDegSeqH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int k);
/// Returns k degree distribution using fixed memory BFS
template <class TEdgeW, template <class> class TGraph > void GetkWDegSeqH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int k);

template <class TEdgeW, template <class> class TGraph >
class TFixedMemorykWDeg : public TFixedMemoryBFS<TPt<TGraph<TEdgeW> > > {
public:
  // Backward / forward visitor (degree only)
  class TkWDegVisitor {
  public:
    TPt<TGraph<TEdgeW> > Graph;
    TEdgeDir Dir;
    int WDeg;
  public:
    TkWDegVisitor(const TPt<TGraph<TEdgeW> >& GraphArg) : Graph(GraphArg), Dir(edUnDirected), WDeg(0) { }
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
  TFixedMemorykWDeg(const TPt<TGraph<TEdgeW> >& GraphArg) : TFixedMemoryBFS<TPt<TGraph<TEdgeW> > >(GraphArg), Visitor(TkWDegVisitor(GraphArg)) { }
  int GetkWInDeg(const int NId, const int k);
  int GetkWOutDeg(const int NId, const int k);
  int GetkWDeg(const int NId, const int k);
  void GetkWInDegSeqH(THash<TInt, TVec<TEdgeW> >& WDegVH, const int k);
  void GetkWOutDegSeqH(THash<TInt, TVec<TEdgeW> >& WDegVH, const int k);
  void GetkWDegSeqH(THash<TInt, TVec<TEdgeW> >& WDegVH, const int k);
  void Clr(const bool& DoDel = false);
};

template <class TEdgeW, template <class> class TGraph >
int TFixedMemorykWDeg<TEdgeW, TGraph>::GetkWInDeg(const int NId, const int k) {
  TPt<TGraph<TEdgeW> > Ego = TGraph<TEdgeW>::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial 0
  Visitor.SetEdgeDir(edInDirected);
  this->GetBfsVisitor<TkWDegVisitor>(Ego, Visitor, edInDirected, k);
  return(Visitor.WDeg);
}
template <class TEdgeW, template <class> class TGraph >
int TFixedMemorykWDeg<TEdgeW, TGraph>::GetkWOutDeg(const int NId, const int k) {
  TPt<TGraph<TEdgeW> > Ego = TGraph<TEdgeW>::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial 0
  Visitor.SetEdgeDir(edOutDirected);
  this->GetBfsVisitor<TkWDegVisitor>(Ego, Visitor, edOutDirected, k);
  return(Visitor.WDeg);
}
template <class TEdgeW, template <class> class TGraph >
int TFixedMemorykWDeg<TEdgeW, TGraph>::GetkWDeg(const int NId, const int k) {
  TPt<TGraph<TEdgeW> > Ego = TGraph<TEdgeW>::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial 0
  Visitor.SetEdgeDir(edUnDirected);
  this->GetBfsVisitor<TkWDegVisitor>(Ego, Visitor, edUnDirected, k);
  return(Visitor.WDeg);
}

template <class TEdgeW, template <class> class TGraph >
void TFixedMemorykWDeg<TEdgeW, TGraph>::GetkWInDegSeqH(THash<TInt, TVec<TEdgeW> >& WDegVH, const int k) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TVec<TEdgeW> WDegV;
  WDegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    WDegV.Clr();
    for (int i = 1; i <= k; i++) {
      WDegV.Add(GetkWInDeg(NI.GetId(), i));
    }
    WDegVH.AddDat(NI.GetId(), WDegV);
  }
}
template <class TEdgeW, template <class> class TGraph >
void TFixedMemorykWDeg<TEdgeW, TGraph>::GetkWOutDegSeqH(THash<TInt, TVec<TEdgeW> >& WDegVH, const int k) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TVec<TEdgeW> WDegV;
  WDegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    WDegV.Clr();
    for (int i = 1; i <= k; i++) {
      WDegV.Add(GetkWOutDeg(NI.GetId(), i));
    }
    WDegVH.AddDat(NI.GetId(), WDegV);
  }
}
template <class TEdgeW, template <class> class TGraph >
void TFixedMemorykWDeg<TEdgeW, TGraph>::GetkWDegSeqH(THash<TInt, TVec<TEdgeW> >& WDegVH, const int k) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TVec<TEdgeW> WDegV;
  WDegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    WDegV.Clr();
    for (int i = 1; i <= k; i++) {
      WDegV.Add(GetkWDeg(NI.GetId(), i));
    }
    WDegVH.AddDat(NI.GetId(), WDegV);
  }
}

template <class TEdgeW, template <class> class TGraph >
void TFixedMemorykWDeg<TEdgeW, TGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<TPt<TGraph<TEdgeW> > >::Clr(DoDel);
  Visitor.Clr(); // resets the degree visitor to the initial -1
}

template <class TEdgeW, template <class> class TGraph > 
void GetkWInDegSeqH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int k) {
  TFixedMemorykWDeg<TEdgeW, TGraph> FixedMemorykWDeg(WGraph);
  FixedMemorykWDeg.GetkWInDegSeqH(WDegVH, k);
}
template <class TEdgeW, template <class> class TGraph > 
void GetkWOutDegSeqH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int k) {
  TFixedMemorykWDeg<TEdgeW, TGraph> FixedMemorykWDeg(WGraph);
  FixedMemorykWDeg.GetkWOutDegSeqH(WDegVH, k);
}
template <class TEdgeW, template <class> class TGraph > 
void GetkWDegSeqH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int k) {
  TFixedMemorykWDeg<TEdgeW, TGraph> FixedMemorykWDeg(WGraph);
  FixedMemorykWDeg.GetkWDegSeqH(WDegVH, k);
}

} // namespace TSnap

#endif