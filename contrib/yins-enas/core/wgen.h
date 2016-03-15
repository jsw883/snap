#ifndef yins_wstats
#define yins_wstats

// Headers (?)

// #include "Snap.h"
// #include "wgraph.h"

//#//////////////////////////////////////////////
/// Weighted graph generators

namespace TSnap {

/// Weighted Erdos Renyi model (geometric distribution, discrete weights)
///   URL: http://arxiv.org/abs/0902.0897
template <class TEdgeW, template <class> class TGraph >
TPt<TGraph> GenWErdosRenyi(const int& Nodes, const TEdgeW& TotalW, TRnd& Rnd) {
  TPt<TGraph> GraphPt = TPt<TGraph>::New();
  typename TGraph& Graph = *GraphPt;
  double p = (double) 2 * TotalW / ((double) Nodes * (Nodes - 1) + 2 * TotalW);
  int Edges = (double) p * Nodes * (Nodes - 1) / 2;
  Graph.Reserve(Nodes, Edges);
  // TODO: Now that the number of edges is not guaranteed, this may not work
  IAssertR((double) (IsDir ? 2 : 1) * Nodes * (Nodes - 1) / 2 >= (double) Edges, TStr::Fmt("Not enough nodes (%d), for edges (%d).", Nodes, Edges));
  for (int node = 0; node < Nodes; node++) {
    IAssert(Graph.AddNode(node) == node);
  }
  for (int SrcNId = 0; SrcNId < Nodes; SrcNId++) {
    for (int DstNId = 0; DstNId < Nodes; DstNId++) {
      if (SrcNId != DstNId && !Graph.IsEdge(SrcNId, DstNId)) { // don't want to increment edge weight accidentally
        Graph.AddEdge(SrcNId, DstNId, Rnd.GetGeoDev(p) - 1);
      }
    }
  }
  return GraphPt;
}

/// Weighted Barabasi-Albert model (hacky)
/// This is currently binary preferential attachment with geometric weights
/// applied agnostically to the resulting preferential attachment topology.
///
/// Need a Hash.AddSorted() method to implement weighted preference (hacky).
///
/// URL: http://journals.aps.org/pre/pdf/10.1103/PhysRevE.86.026117
template <class TEdgeW, template <class> class TGraph >
TPt<TGraph> GenWPrefAttach(const int& Nodes, const int& NodeOutDeg, TEdgeDir& prefDir, const TEdgeW& TotalW, TRnd& Rnd) {
  TPt<TGraph> GraphPt = TPt<TGraph>::New();
  typename TGraph& Graph = *GraphPt;
  Graph.Reserve(Nodes, NodeOutDeg*Nodes);
  TIntV NIdV(NodeOutDeg*Nodes, 0);
  // first edge
  Graph.AddNode(0);  Graph.AddNode(1);
  NIdV.Add(0);  NIdV.Add(1);
  Graph.AddEdge(0, 1);
  THash<TInt, TEdgeW> NodeSet;
  for (int node = 2; node < Nodes; node++) {
    NodeSet.Clr(false);
    while (NodeSet.Len() < NodeOutDeg && NodeSet.Len() < node) {
      NodeSet.AddKey(NIdV[TInt::Rnd.GetUniDevInt(NIdV.Len())]);
    }
    const int N = Graph.AddNode();
    for (int i = 0; i < NodeSet.Len(); i++) {
      Graph.AddEdge(N, NodeSet[i], Rnd.GetGeoDev(p) - 1);
      NIdV.Add(N);
      NIdV.Add(NodeSet[i]);
    }
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
