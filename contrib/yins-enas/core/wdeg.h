#ifndef core_wdeg
#define core_wdeg

// Headers (?)

#include "galg.h"
#include "galgfixedmemory.h"

//#//////////////////////////////////////////////
/// Wrappers for weighted first degrees

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
void GetWDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TEdgeW>& WDegH) {
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
  TVec<TEdgeW> WDegV;
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

//#//////////////////////////////////////////////
/// Weighted k degrees using fixed memory BFS

namespace TSnap {

// /// Returns k in degree distribution using fixed memory BFS
// template <class TEdgeW, template <class> class TGraph > void GetkWInDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int& k);
// /// Returns k out degree distribution using fixed memory BFS
// template <class TEdgeW, template <class> class TGraph > void GetkWOutDeqH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int& k);
// /// Returns k degree distribution using fixed memory BFS
// template <class TEdgeW, template <class> class TGraph > void GetkWDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int& k);

template <class TEdgeW, template <class> class TGraph>
class TFixedMemorykWDeg : public TFixedMemoryBFS<TPt<TGraph<TEdgeW> > > {
public:
  // Backward / forward visitor (weighted degree only)
  class TkWDegVisitor {
  public:
    // Parameters for BFS
    TPt<TGraph<TEdgeW> > Graph;
    TEdgeDir Dir;
    TVec<TEdgeW> WDegV;
  public:
    TkWDegVisitor(const TPt<TGraph<TEdgeW> >& Graph, const int& k) : Graph(Graph), Dir(edUnDirected), WDegV(k) { }
    // Supplementary methods
    void SetEdgeDir(const TEdgeDir& DirArg) { Dir = DirArg; }
    void AddW(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) {
      TEdgeW& WDeg = WDegV[depth];
      switch(Dir) {
        case edInDirected: WDeg += Graph->GetNI(SrcNId).GetInEW(edge); break;
        case edOutDirected: WDeg += Graph->GetNI(SrcNId).GetOutEW(edge); break;
        case edUnDirected: WDeg += Graph->GetNI(SrcNId).GetNbrEW(edge); break;
      }
    }
    // BFS methods (standard signatures)
    void Start() { }
    void DiscoverNode(const int& NId, const int& depth) { }
    void FinishNode(const int& NId, const int& depth) { }
    void ExamineEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) { }
    void TreeEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) {
      AddW(SrcNId, depth, edge, DstNId);
    }
    void BackEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) {
      AddW(SrcNId, depth, edge, DstNId);
    }
    void ForwardEdge(const int& SrcNId, const int&  depth, const int& edge, const int& DstNId) {
      AddW(SrcNId, depth, edge, DstNId);
    }
    void Finish() {
      for (int i = 1; i < WDegV.Len(); i++) {
        WDegV[i] += WDegV[i - 1];
      };
    }
    void Clr() {
      for (int i = 0; i < WDegV.Len(); i++) {
        WDegV[i] = 0;
      }
      Dir = edUnDirected;
    }
  };
private:
  TkWDegVisitor Visitor;
  int k;
public:
  TFixedMemorykWDeg(const TPt<TGraph<TEdgeW> >& Graph, const int& k) : TFixedMemoryBFS<TPt<TGraph<TEdgeW> > >(Graph), Visitor(TkWDegVisitor(Graph, k)), k(k) { }
  // Get k weighted degree for a single node (in / out / undirected)
  void GetkWInDegV(const int& NId, TVec<TEdgeW>& WDegV) {
    GetkWDegV(NId, WDegV, edInDirected);
  }
  void GetkWOutDegV(const int& NId, TVec<TEdgeW>& WDegV) {
    GetkWDegV(NId, WDegV, edOutDirected);
  }
  void GetkWDegV(const int& NId, TVec<TEdgeW>& WDegV) {
    GetkWDegV(NId, WDegV, edUnDirected);
  }
  // Get k weighted degree for a single node according to direction specified
  void GetkWDegV(const int& NId, TVec<TEdgeW>& WDegV, const TEdgeDir& Dir);
  // Get k weighted degree for all nodes (in / out / undirected)
  void GetkWInDegH(THash<TInt, TVec<TEdgeW> >& WDegVH) {
    GetkWDegH(WDegVH, edInDirected);
  }
  void GetkWOutDegH(THash<TInt, TVec<TEdgeW> >& WDegVH) {
    GetkWDegH(WDegVH, edOutDirected);
  }
  void GetkWDegH(THash<TInt, TVec<TEdgeW> >& WDegVH) {
    GetkWDegH(WDegVH, edUnDirected);
  }
  // Get k weighted degree for all nodes according to direction specified
  void GetkWDegH(THash<TInt, TVec<TEdgeW> >& WDegVH, const TEdgeDir& Dir);
  void Clr(const bool& DoDel = false);
};

// Get k weighted degree for a single node according to direction specified
template <class TEdgeW, template <class> class TGraph>
void TFixedMemorykWDeg<TEdgeW, TGraph>::GetkWDegV(const int& NId, TVec<TEdgeW>& WDegV, const TEdgeDir& Dir) {
  TPt<TGraph<TEdgeW> > Ego = TGraph<TEdgeW>::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr(); // resets the degree visitor to the initial 0
  Visitor.SetEdgeDir(Dir);
  this->GetBfsVisitor<TkWDegVisitor>(Ego, Visitor, Dir, k);
  WDegV = Visitor.WDegV;
}  

// Get k weighted degree for all nodes according to direction specified
template <class TEdgeW, template <class> class TGraph>
void TFixedMemorykWDeg<TEdgeW, TGraph>::GetkWDegH(THash<TInt, TVec<TEdgeW> >& WDegVH, const TEdgeDir& Dir) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TVec<TEdgeW> WDegV;
  WDegVH.Gen(this->Graph->GetNodes());
  for (NI = this->Graph->BegNI(); NI < this->Graph->EndNI(); NI++) {
    WDegV.Clr();
    GetkWDegV(NI.GetId(), WDegV, Dir);
    WDegVH.AddDat(NI.GetId(), WDegV);
  }
}

template <class TEdgeW, template <class> class TGraph>
void TFixedMemorykWDeg<TEdgeW, TGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<TPt<TGraph<TEdgeW> > >::Clr(DoDel);
  Visitor.Clr(); // resets the degree visitor
}

// template <class TEdgeW, template <class> class TGraph > 
// void GetkWInDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int& k) {
//   TFixedMemorykWDeg<TEdgeW, TGraph> FixedMemorykWDeg(WGraph, k);
//   FixedMemorykWDeg.GetkWInDegH(WDegVH, k);
// }
// template <class TEdgeW, template <class> class TGraph > 
// void GetkWOutDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int& k) {
//   TFixedMemorykWDeg<TEdgeW, TGraph> FixedMemorykWDeg(WGraph, k);
//   FixedMemorykWDeg.GetkWOutDegH(WDegVH, k);
// }
// template <class TEdgeW, template <class> class TGraph > 
// void GetkWDegH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& WDegVH, const int& k) {
//   TFixedMemorykWDeg<TEdgeW, TGraph> FixedMemorykWDeg(WGraph, k);
//   FixedMemorykWDeg.GetkWDegH(WDegVH, k);
// }

} // namespace TSnap

#endif