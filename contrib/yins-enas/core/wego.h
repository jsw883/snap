#ifndef core_wego
#define core_wego

// Headers (?)

#include "galg.h"

namespace TSnap {

template <class TEdgeW, template <class> class TGraph>
class TFixedMemorykWEgo : public TFixedMemoryBFS<TPt<TGraph<TEdgeW> > > {
public:
// Backward / forward visitor (degree only)
  class TkWEgoVisitor {
  public:
    // Parameters for BFS
    TPt<TGraph<TEdgeW> > Graph;
    int k;
    TEdgeDir Dir;
    // Counted during BFS
    int Nodes;
    int Edges;
    THash<TInt, TEdgeW> WDegH;
  public:
    TkWEgoVisitor(const TPt<TGraph<TEdgeW> >& Graph, const int& k) : Graph(Graph), k(k), Dir(edUnDirected), WDegH(Graph->GetNodes()) { }
    // Supplementary methods
    void SetEdgeDir(const TEdgeDir& DirArg) { Dir = DirArg; }
    void AddW(const int& NId, const int& depth, const TEdgeW& W) {
      if (!WDegH.IsKey(NId) && depth < k) {
        WDegH.AddDat(NId, W);
      } else {
        TEdgeW& WDeg = WDegH.GetDat(NId);
        WDeg += W;
      }
    }
    void AddW(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) {
      switch(Dir) {
        case edInDirected: {
          const TEdgeW W = Graph->GetNI(SrcNId).GetInEW(edge);
          AddW(SrcNId, depth, W);
          break;
        }
        case edOutDirected: {
          const TEdgeW W = Graph->GetNI(SrcNId).GetOutEW(edge);
          AddW(SrcNId, depth, W);
          break;
        }
        case edUnDirected: {  // add edge weight to source node (OUT)
          const TEdgeW W = Graph->GetNI(SrcNId).GetNbrEW(edge);
          AddW(SrcNId, depth, W);
          AddW(DstNId, depth, W);
          break;
        }
      }
    }
    // BFS methods (standard signatures)
    void Start() { }
    void DiscoverNode(int NId, int depth) {
      Nodes += 1;
    }
    void FinishNode(const int& NId, int depth) { }
    void ExamineEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) { }
    void TreeEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) {
      Edges += 1;
      AddW(SrcNId, depth, edge, DstNId);
    }
    void BackEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) {
      Edges += 1;
      AddW(SrcNId, depth, edge, DstNId);
    }
    void ForwardEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) {
      Edges += 1;
      AddW(SrcNId, depth, edge, DstNId);
    }
    void Finish() { }
    // Clear method (somewhat standard)
    void Clr() {
      Dir = edUnDirected;
      Nodes = 0;
      Edges = 0;
      WDegH.Clr(false);
    }
  };
  
private:
  TkWEgoVisitor Visitor;
  int k;
  bool computed;
public:
  TFixedMemorykWEgo(const TPt<TGraph<TEdgeW> >& Graph, const int& k) : TFixedMemoryBFS<TPt<TGraph<TEdgeW> > >(Graph), Visitor(TkWEgoVisitor(Graph, k)), k(k) { }
  // Direction specific compute egonet statistics
  void ComputeInEgonetStatistics(const int& NId) {
    ComputeEgonetStatistics(NId, edInDirected);
  }
  void ComputeOutEgonetStatistics(const int& NId) {
    ComputeEgonetStatistics(NId, edOutDirected);
  }
  void ComputeEgonetStatistics(const int& NId) {
    ComputeEgonetStatistics(NId, edUnDirected);
  }
  // Compute egonet statistics using the direction specified
  void ComputeEgonetStatistics(const int& NId, const TEdgeDir& Dir);
  // Get results from the egonet traversal (only valid after compute method called)
  int GetNodes() { return Visitor.Nodes; }
  int GetEdges() { return Visitor.Edges; }
  double GetDensity();
  TEdgeW GetTotalW();
  double GetGiniCoefficient();
  void GetWDegH(THash<TInt, TEdgeW> WDegH) { WDegH = Visitor.WDegH; }
  void Clr(const bool& DoDel = false);
};

template <class TEdgeW, template <class> class TGraph>
void TFixedMemorykWEgo<TEdgeW, TGraph>::ComputeEgonetStatistics(const int& NId, const TEdgeDir& Dir) {
  TPt<TGraph<TEdgeW> > Ego = TGraph<TEdgeW>::New(); Ego->AddNode(NId); // this might be inefficient (?)
  Visitor.Clr();
  Visitor.SetEdgeDir(Dir);
  this->GetBfsVisitor<TkWEgoVisitor>(Ego, Visitor, Dir, k);
}

template <class TEdgeW, template <class> class TGraph>
double TFixedMemorykWEgo<TEdgeW, TGraph>::GetDensity() {
  return(double(Visitor.Edges) / (double(Visitor.Nodes) * (double(Visitor.Nodes) - 1)));
}

template <class TEdgeW, template <class> class TGraph>
TEdgeW TFixedMemorykWEgo<TEdgeW, TGraph>::GetTotalW() {
  TEdgeW TotalW = 0.0;
  for (int i = 0; i < Visitor.WDegH.Len(); i++) {
    TotalW += Visitor.WDegH[i];
  }
  if (Visitor.Dir == edUnDirected) { TotalW /= 2; }
  return(TotalW);
}

template <class TEdgeW, template <class> class TGraph>
double TFixedMemorykWEgo<TEdgeW, TGraph>::GetGiniCoefficient() {
  typename TVec<TEdgeW>::TIter DI;
  TVec<TEdgeW> WDegV;
  Visitor.WDegH.GetDatV(WDegV);
  WDegV.Sort();
  const int n = WDegV.Len();
  int i = 0;
  double numerator = 0.0, denominator = 0.0;
  for (DI = WDegV.BegI(); DI < WDegV.EndI(); DI++, i++) {
    numerator += (i + 1)*WDegV[i];
    denominator += WDegV[i];
  }
  return(double(2*numerator) / double(n*denominator) - double(n + 1) / double(n));
}

template <class TEdgeW, template <class> class TGraph>
void TFixedMemorykWEgo<TEdgeW, TGraph>::Clr(const bool& DoDel) {
  TFixedMemoryBFS<TPt<TGraph<TEdgeW> > >::Clr(DoDel);
  Visitor.Clr(); // resets the degree visitor
}

} // namespace TSnap

#endif
