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
    TEdgeDir Dir;
    // Counted during BFS
    int Nodes;
    int Edges;
    THash<TInt, TEdgeW> WDegH;
  public:
    TkWEgoVisitor(const TPt<TGraph<TEdgeW> >& Graph) : Graph(Graph), Dir(edUnDirected), WDegH(Graph->GetNodes()) { }
    // Supplementary methods
    void SetEdgeDir(const TEdgeDir& DirArg) { Dir = DirArg; }
    void AddW(const int& SrcNId, const int& edge, const int& DstNId, const bool& discovered) {
      switch(Dir) {
        case edInDirected: { 
          TEdgeW& WDeg = discovered ? WDegH.GetDat(SrcNId) : WDegH.AddDat(SrcNId);
          WDeg += Graph->GetNI(SrcNId).GetInEW(edge);
          break;
        }
        case edOutDirected: {
          TEdgeW& WDeg = discovered ? WDegH.GetDat(SrcNId) : WDegH.AddDat(SrcNId);
          WDeg += Graph->GetNI(SrcNId).GetOutEW(edge);
          break;
        }
        case edUnDirected: {  // add edge weight to source node (OUT)
          TEdgeW& WSrcDeg = discovered ? WDegH.GetDat(SrcNId) : WDegH.AddDat(SrcNId);
          TEdgeW& WDstDeg = discovered ? WDegH.GetDat(DstNId) : WDegH.AddDat(DstNId);
          const TEdgeW W = Graph->GetNI(SrcNId).GetNbrEW(edge);
          WSrcDeg += W;
          WDstDeg += W;
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
      AddW(SrcNId, edge, DstNId, false);
    }
    void BackEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) {
      Edges += 1;
      AddW(SrcNId, edge, DstNId, true);
    }
    void ForwardEdge(const int& SrcNId, const int& depth, const int& edge, const int& DstNId) {
      Edges += 1;
      AddW(SrcNId, edge, DstNId, true);
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
  TFixedMemorykWEgo(const TPt<TGraph<TEdgeW> >& Graph, const int& k) : TFixedMemoryBFS<TPt<TGraph<TEdgeW> > >(Graph), Visitor(TkWEgoVisitor(Graph)), k(k) { }
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
