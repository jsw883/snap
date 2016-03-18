#ifndef yins_wgen
#define yins_wgen

// Headers (?)

// #include "Snap.h"
// #include "wgraph.h"

//#//////////////////////////////////////////////
/// Weighted random graph generators

namespace TSnap {

/// Weighted Erdos Renyi model (geometric distribution, discrete weights)
template <class TEdgeW, template <class> class TGraph > TPt<TGraph<TEdgeW> > GenGeoErdosRenyi(const int& Nodes, const TEdgeW& TotalW, TRnd& Rnd = TInt::Rnd);
/// Weighted Erdos Renyi model (geometric distribution, discrete weights)
template <class TEdgeW, template <class> class TGraph > TPt<TGraph<TEdgeW> > GenExpErdosRenyi(const int& Nodes, const TEdgeW& TotalW, const TEdgeW& Threshold = 0, TRnd& Rnd = TInt::Rnd);
/// Weighted Barabasi-Albert model (hacky)
template <class TEdgeW, template <class> class TGraph > TPt<TGraph<TEdgeW> > GenWPrefAttach(const int& Nodes, const int& PrefOutDeg, const TEdgeDir& prefDir, const double& Scale, const double& Shape, TRnd& Rnd = TInt::Rnd);

/// Weighted Erdos Renyi model (geometric distribution, discrete weights)
/// 
/// Maximally random ensemble of weighted networks with uniformly distributed
/// strengths, assuming geometric weight distribution q(w) = (p)^w * (1 - p),
/// where p = W / (n * (n - 1) + W) is the MLE to reproduce the same total W.
/// 
///   URL: http://arxiv.org/abs/0902.0897
template <class TEdgeW, template <class> class TGraph >
TPt<TGraph<TEdgeW> > GenGeoErdosRenyi(const int& Nodes, const TEdgeW& TotalW, TRnd& Rnd) {
  TPt<TGraph<TEdgeW> > GraphPt = TPt<TGraph<TEdgeW> >::New();
  TGraph<TEdgeW>& Graph = *GraphPt;
  double p = (double) TotalW / ((double) Nodes * (Nodes - 1) + TotalW);
  int Edges = (double) p * Nodes * (Nodes - 1) / 2;
  Graph.Reserve(Nodes, Edges);
  IAssertR((double) (Graph.HasFlag(gfDirected) ? 2 : 1) * Nodes * (Nodes - 1) / 2 >= (double) Edges, TStr::Fmt("Not enough nodes (%d), for edges (%d).", Nodes, Edges));
  for (int node = 0; node < Nodes; node++) {
    IAssert(Graph.AddNode(node) == node);
  }
  for (int SrcNId = 0; SrcNId < Nodes; SrcNId++) {
    for (int DstNId = 0; DstNId < Nodes; DstNId++) {
      if (SrcNId != DstNId && !Graph.IsEdge(SrcNId, DstNId)) { // don't want to increment edge weight accidentally
        TEdgeW W = Rnd.GetGeoDev(1 - p) - 1; // allow W = 0
        if (W > 0) { Graph.AddEdge(SrcNId, DstNId, W); }
      }
    }
  }
  return GraphPt;
}

/// Weighted Erdos Renyi model (geometric distribution, discrete weights)
/// 
/// Maximally random ensemble of weighted networks with uniformly distributed
/// strengths, assuming exponential weight distribution q(w) = a * exp(-a*x),
/// where a = (1 - p) / p and p = W / (n * (n - 1) + W).
/// 
///   URL: http://arxiv.org/abs/0902.0897
template <class TEdgeW, template <class> class TGraph >
TPt<TGraph<TEdgeW> > GenExpErdosRenyi(const int& Nodes, const TEdgeW& TotalW, const TEdgeW& Threshold, TRnd& Rnd) {
  TPt<TGraph<TEdgeW> > GraphPt = TPt<TGraph<TEdgeW> >::New();
  TGraph<TEdgeW>& Graph = *GraphPt;
  double p = (double) TotalW / ((double) Nodes * (Nodes - 1) + TotalW);
  int Edges = (double) p * Nodes * (Nodes - 1) / 2;
  Graph.Reserve(Nodes, Edges);
  IAssertR((double) (Graph.HasFlag(gfDirected) ? 2 : 1) * Nodes * (Nodes - 1) / 2 >= (double) Edges, TStr::Fmt("Not enough nodes (%d), for edges (%d).", Nodes, Edges));
  for (int node = 0; node < Nodes; node++) {
    IAssert(Graph.AddNode(node) == node);
  }
  for (int SrcNId = 0; SrcNId < Nodes; SrcNId++) {
    for (int DstNId = 0; DstNId < Nodes; DstNId++) {
      if (SrcNId != DstNId && !Graph.IsEdge(SrcNId, DstNId)) { // don't want to increment edge weight accidentally
        TEdgeW W = Rnd.GetExpDev((1 - p) / p);
        if (W > Threshold) { Graph.AddEdge(SrcNId, DstNId, W); } // TODO: actually model a better threshold
      }
    }
  }
  return GraphPt;
}

/// Pareto Weighted Barabasi-Albert model (hacky)
/// This is currently preferential attachment with Pareto weights applied
/// agnostically to the resulting preferential attachment topology along
/// the way.
///
///   URL: http://journals.aps.org/pre/pdf/10.1103/PhysRevE.86.026117
template <class TEdgeW, template <class> class TGraph >
TPt<TGraph<TEdgeW> > GenWPrefAttach(const int& Nodes, const int& PrefOutDeg, const TEdgeDir& prefDir, const double& Scale, const double& Shape, TRnd& Rnd) {
  typedef TPair<TEdgeW, TInt> TEdgeWIntPr;
  // graph
  TPt<TGraph<TEdgeW> > GraphPt = TPt<TGraph<TEdgeW> >::New();
  TGraph<TEdgeW>& Graph = *GraphPt;
  Graph.Reserve(Nodes, PrefOutDeg*Nodes);
  // variables
  TVec<TEdgeWIntPr> NodeDensity(PrefOutDeg*Nodes, 0);
  TEdgeW W, SetW, TotalW = 0;
  int i;
  double Rand, Temp;
  // edge
  Graph.AddNode(0);
  Graph.AddNode(1);
  W = Rnd.GetParetoDev(Scale, Shape); // Rnd.GetGeoDev(p); // edge must exist so no - 1
  if (W > 0) {
    Graph.AddEdge(1, 0, W);
    TotalW += (prefDir == edUnDirected ? 2 : 1) * W;
  }
  NodeDensity.AddSorted(TEdgeWIntPr(Graph.GetNI(0).GetWDeg(prefDir), 0), false);
  NodeDensity.AddSorted(TEdgeWIntPr(Graph.GetNI(1).GetWDeg(prefDir), 1), false);
  // other edges
  TIntSet Set;
  TIntV Vec;
  for (int nodes = 2; nodes < Nodes; nodes++) {
    Set.Clr(false);
    Vec.Clr(false);
    // limited by preferred out degree and available nodes
    SetW = 0;
    while (Set.Len() < PrefOutDeg && Set.Len() < nodes) {
      Rand = (TotalW - SetW) * Rnd.GetUniDev();
      Temp = 0;
      for (i = 0; i < NodeDensity.Len(); i++) {
        if (Set.IsKey(i)) {
          continue;
        }
        Temp += NodeDensity[i].Val1;
        if (Temp > Rand) { break; }
      }
      Vec.AddSorted(i);
      Set.AddKey(i);
      SetW += NodeDensity[i].Val1;
    }
    const int N = Graph.AddNode();
    for (i = 0; i < Set.Len(); i++) {
      // geometric random weights
      W = Rnd.GetParetoDev(Scale, Shape); // Rnd.GetGeoDev(p) - 1;
      if (W > 0) {
        // add edge
        Graph.AddEdge(N, NodeDensity[Set.GetKey(i)].Val2, W);
        TotalW += (prefDir == edUnDirected ? 2 : 1) * W;
        // update preferred node density (could use degree again to be sure)
        if (prefDir != edOutDirected) {
          NodeDensity[Set.GetKey(i)].Val1 += W;
        }
      }
    }
    // apply insertion sort to ensure order is maintained (order matters)
    for (int i = 0; i < Vec.Len(); i++) {
      int ValN1 = Vec[i];
      int ValN2 = ValN1;
      TEdgeWIntPr Val = NodeDensity[ValN1];
      while (ValN2 > 0 && NodeDensity[ValN2-1] < Val) {
        NodeDensity[ValN2] = NodeDensity[ValN2-1];
        ValN2--;
      }
      NodeDensity[ValN2] = Val;
    }
    // add sorted
    NodeDensity.AddSorted(TEdgeWIntPr(Graph.GetNI(N).GetWDeg(prefDir), N), false);
  }
  return GraphPt;
}

//#//////////////////////////////////////////////
/// Surrogate random graph algorithms

/// Weight reshuffling (degree and weight preserving)
template <class TEdgeW>
void WeightShuffling(TPt<TWNGraph<TEdgeW> >& Graph) {
  typename TWNGraph<TEdgeW>::TEdgeI EI;
  TVec<TEdgeW> EIW;
  int i;
  TRnd Rnd(0);
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    EIW.Add(EI.GetW());
  }
  EIW.Shuffle(Rnd); // shuffle randomly
  for (EI = Graph->BegEI(), i = 0; EI < Graph->EndEI(); EI++, i++) {
    Graph->SetEW(EI.GetSrcNId(), EI.GetDstNId(), EIW[i]);
  }
}

/// Weight reshuffling (degree and weight preserving)
template <class TEdgeW>
void WeightShuffling(TPt<TWNEGraph<TEdgeW> >& Graph) {
  typename TWNEGraph<TEdgeW>::TEdgeI EI;
  TVec<TEdgeW> EIW;
  int i;
  TRnd Rnd(0);
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    EIW.Add(EI.GetW());
  }
  EIW.Shuffle(Rnd);
  // Iterate through entire graph and shuffle randomly
  for (EI = Graph->BegEI(), i = 0; EI < Graph->EndEI(); EI++, i++) {
    Graph->SetEW(EI.GetId(), EIW[i]);
  }
}

} // namespace TSnap

/// Random graph generators (TODO)
/// ==============================

/// Generators
/// ----------
/// Weighted Erdos Renyi model (geometric distribution, discrete weights)
///   URL: http://arxiv.org/abs/0902.0897
/// Weighted Barbasi-Albert model (hacky)
///   URL: http://journals.aps.org/pre/pdf/10.1103/PhysRevE.86.026117

/// Surrogate random graph algorithms
/// =================================

/// Weight shuffling (topology preserving)
/// --------------------------------------
/// Local weight reshuffling (degree, strength, and weight preserving)
/// Weight reshuffling (degree and weight preserving)

/// Edge shuffling
/// --------------
/// Binary edge rewiring (degree, weight preserving)
/// Tetragonal weight redistributing (continuous, topology preserving)
///   URL: http://journals.aps.org/pre/pdf/10.1103/PhysRevE.84.026103

//#//////////////////////////////////////////////
/// Graph threshold

namespace TSnap {

/// Restricts graph to have edge weights greater than or equal to threshold
template <class TEdgeW, template <class> class TGraph > static TPt<TGraph<TEdgeW> > ThresholdGraph(const TPt<TGraph<TEdgeW> >& Graph, const TEdgeW& Threshold);

/// Restricts graph to have edge weights greater than or equal to threshold
template <class TEdgeW, template <class> class TGraph >
static TPt<TGraph<TEdgeW> > ThresholdGraph(const TPt<TGraph<TEdgeW> >& Graph, const TEdgeW& Threshold) {
  typename TGraph<TEdgeW>::TEdgeI EI;
  TPt<TGraph<TEdgeW> > GraphCopy = TGraph<TEdgeW>::New();
  *GraphCopy = *Graph;
  // Iterate through the edges, delete based on threshold
  for (EI = GraphCopy->BegEI(); EI < GraphCopy->EndEI(); ) {
    if (EI.GetW() < Threshold) {
      GraphCopy->DelEdge(EI);
    } else {
      EI++;
    }
  }
  return GraphCopy;
}

} // namespace TSnap

#endif