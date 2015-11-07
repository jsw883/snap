#ifndef core_wfilter
#define core_wfilter

// Headers (?)

namespace TSnap {

/// Prunes using the Vespignani method (for more info see: http://www.pnas.org/content/106/16/6483.full)
template <class TEdgeW, template <class> class TGraph > void FilterEdgesVespignani(TPt<TGraph<TEdgeW> >& WGraph, const TIntFltH& InWDegH, const TIntFltH& OutWDegH, const TIntIntH& InDegH, const TIntIntH& OutDegH, const double& alpha);

/// Prunes using the Vespignani method (for more info see: http://www.pnas.org/content/106/16/6483.full)
template <class TEdgeW, template <class> class TGraph >
void FilterEdgesVespignani(TPt<TGraph<TEdgeW> >& WGraph, const TIntFltH& InWDegH, const TIntFltH& OutWDegH, const TIntIntH& InDegH, const TIntIntH& OutDegH, const double& alpha) {
  typename PFltWNGraph::TObj::TEdgeI EI;
  // Implicitly iterated forward if an edge is deleted, else explicitly iterate forward with EI++
  for (EI = WGraph->BegEI(); EI < WGraph->EndEI(); ) {
    // Break early to avoid segfault if all edges are removed when alpha = 0
    if (WGraph->GetEdges() == 0) {
      break;
    }
    // Check if the only out edge for the source node
    if ((OutDegH.GetDat(EI.GetSrcNId()) == 1) && (InDegH.GetDat(EI.GetDstNId()) > 1)) {
      // Remove edge only if not significant for destination node
      if (!(pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)) < alpha)) {
        WGraph->DelEdge(EI.GetSrcNId(), EI.GetDstNId());
      } else {
        EI++;
      }
    // Check if the only in edge for the destination node
    } else if ((OutDegH.GetDat(EI.GetSrcNId()) > 1) && (InDegH.GetDat(EI.GetDstNId()) == 1)) {
      // Remove edge only if not significant for source node
      if (!(pow((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId())), (OutDegH.GetDat(EI.GetSrcNId()) - 1)) < alpha)) {
        WGraph->DelEdge(EI.GetSrcNId(), EI.GetDstNId());
      } else {
        EI++;
      }
    // Check if not the only out edge for source node and not the only in edge for destination mpde
    } else if ((OutDegH.GetDat(EI.GetSrcNId()) > 1) && (InDegH.GetDat(EI.GetDstNId()) > 1)) {
      // Remove edge only if not significant for either side
      if (!
        ((pow((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId())), (OutDegH.GetDat(EI.GetSrcNId()) - 1)) < alpha) || 
                (pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)) < alpha))) {
            WGraph->DelEdge(EI.GetSrcNId(), EI.GetDstNId());
          } else {
            EI++;
        }
    // Edge is retained if the only out edge for the source and the only in edge for the destination node
    } else {
      EI++;
    }
  }
}

} // namespace TSnap

#endif
