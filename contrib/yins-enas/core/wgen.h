#ifndef yins_wgen
#define yins_wgen

// Headers (?)

// #include "Snap.h"
// #include "wgraph.h"

//#//////////////////////////////////////////////
/// Weighted graph generators

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
  
  TPt<TGraph<TEdgeW> > GraphPt = TPt<TGraph<TEdgeW> >::New();
  TGraph<TEdgeW>& Graph = *GraphPt;
  Graph.Reserve(Nodes, PrefOutDeg*Nodes);
  
  // double p = (double) PrefW / ((double) Nodes * (Nodes - 1) + PrefW);
  
  TVec<TEdgeWIntPr> NodeDensity(PrefOutDeg*Nodes, 0);
  TEdgeW W, SetW, TotalW = 0;
  int i;
  double Rand, Temp;
  
  // first edge
  Graph.AddNode(0);
  Graph.AddNode(1);
  W = Rnd.GetParetoDev(Scale, Shape); // Rnd.GetGeoDev(p); // edge must exist so no - 1
  if (W > 0) {
    Graph.AddEdge(1, 0, W);
    TotalW += (prefDir == edUnDirected ? 2 : 1) * W;
    // printf("Nodes = %d\n", Graph.GetNodes());
    // printf("Edges = %d\n", Graph.GetEdges());
    // printf("Graph.TotalW = %f\n", (double) Graph.GetTotalW());
    // printf("TotalW = %f\n", (double) TotalW);
  }
  NodeDensity.AddSorted(TEdgeWIntPr(Graph.GetNI(0).GetWDeg(prefDir), 0), false);
  NodeDensity.AddSorted(TEdgeWIntPr(Graph.GetNI(1).GetWDeg(prefDir), 1), false);
  // printf("NodeDensity =\n");
  // for (typename TVec<TEdgeWIntPr>::TIter VI = NodeDensity.BegI(); VI < NodeDensity.EndI(); VI++) {
  //   printf("  (%f, %f)\n", (double) VI->Val1, (double) VI->Val2);
  // }
  
  TIntSet Set;
  TIntV Vec;
  for (int nodes = 2; nodes < Nodes; nodes++) {
    Set.Clr(false);
    Vec.Clr(false);
    
    // printf("---------\n");
    // printf("nodes = %d\n", nodes);
    
    // limited by preferred out degree and available nodes
    // printf("Graph.TotalW = %f\n", (double) Graph.GetTotalW());
    // printf("TotalW = %f\n", (double) TotalW);
    // printf("NodeDensity =\n");
    // for (typename TVec<TEdgeWIntPr>::TIter VI = NodeDensity.BegI(); VI < NodeDensity.EndI(); VI++) {
    //   printf("  (%f, %f)\n", (double) VI->Val1, (double) VI->Val2);
    // }
    SetW = 0;
    while (Set.Len() < PrefOutDeg && Set.Len() < nodes) {
      Rand = (TotalW - SetW) * Rnd.GetUniDev();
      Temp = 0;
      // printf("TotalW - SetW = %f\n", (double) TotalW - SetW);
      // printf("Rand = %f\n", Rand);
      // printf("Temp = %f\n", Temp);
      for (i = 0; i < NodeDensity.Len(); i++) {
        if (Set.IsKey(i)) {
          // printf("(%f, %d)\n", (double) NodeDensity[i].Val1, (int) NodeDensity[i].Val2);
          continue;
        }
        Temp += NodeDensity[i].Val1;
        // printf("(%f, %d) Temp = %f\n", (double) NodeDensity[i].Val1, (int) NodeDensity[i].Val2, Temp);
        if (Temp > Rand) { break; }
      }
      // i--;
      // printf("> Set.Addkey(%d)\n", i);
      Vec.AddSorted(i);
      Set.AddKey(i);
      SetW += NodeDensity[i].Val1;
      // printf("Vec =\n");
      // for (typename TIntV::TIter VI = Vec.BegI(); VI < Vec.EndI(); VI++) {
      //   printf("  %f\n", (double) VI->Val);
      // }
      // printf("Set =\n");
      // for (typename TIntSet::TIter SI = Set.BegI(); SI < Set.EndI(); SI++) {
      //   printf("  %f\n", (double) SI.GetKey());
      // }
    }
    const int N = Graph.AddNode();
    for (i = 0; i < Set.Len(); i++) {
      // geometric random weights
      W = Rnd.GetParetoDev(Scale, Shape); // Rnd.GetGeoDev(p) - 1;
      // printf("W = %f\n", (double) W);
      if (W > 0) {
        // add edge
        Graph.AddEdge(N, NodeDensity[Set.GetKey(i)].Val2, W);
        TotalW += (prefDir == edUnDirected ? 2 : 1) * W;
        // update preferred node density (could use degree again to be sure)
        if (prefDir != edOutDirected) {
          NodeDensity[Set.GetKey(i)].Val1 += W;
        }
        // printf("Edge += (%d, %d, %f)\n", N, (int) NodeDensity[Set.GetKey(i)].Val2, (double) W);
      }
    }
    // printf("NodeDensity =\n");
    // for (i = 0; i < NodeDensity.Len(); i++) {
    //   printf("  (%f, %d)", (double) NodeDensity[i].Val1, (int) NodeDensity[i].Val2);
    //   if (Set.IsKey(i)) { printf(" < updated"); }
    //   printf("\n");
    // }
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
/// Weight reshuffling (degree preserving)

/// Edge shuffling
/// --------------
/// Binary edge rewiring (degree, weight preserving)
/// Tetragonal weight redistributing (continuous, topology preserving)
///   URL: http://journals.aps.org/pre/pdf/10.1103/PhysRevE.84.026103

#endif