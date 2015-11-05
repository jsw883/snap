#ifndef core_wfilter
#define core_wfilter

// Headers (?)

namespace TSnap {

/// Prunes using the Vespignani method (for more info see: http://www.pnas.org/content/106/16/6483.full)
template <class TEdgeW, template <class> class TGraph > void GetVespignaniPruned(TPt<TGraph<TEdgeW> >& WGraph, const TIntFltH& InWDegH, const TIntFltH& OutWDegH, const TIntIntH& InDegH, const TIntIntH& OutDegH, const double& alpha);


/// Prunes using the Vespignani method (for more info see: http://www.pnas.org/content/106/16/6483.full)
template <class TEdgeW, template <class> class TGraph >
void GetVespignaniPruned(TPt<TGraph<TEdgeW> >& WGraph, const TIntFltH& InWDegH, const TIntFltH& OutWDegH, const TIntIntH& InDegH, const TIntIntH& OutDegH, const double& alpha) {
  typename PFltWNGraph::TObj::TEdgeI EI;
  // Iterates over the edges.  Implicitly iterated forward if an edge
  // is deleted, else explicitly iterated with EI++
  for (EI = WGraph->BegEI(); EI < WGraph->EndEI(); ) {
    // If there are no edges left, break out of the loop
    // This is only possible when alpha = 0; and even then,
    // it may not happen (see if-else conditions following)
    // Without this break, we get segfault as we're trying
    // to iterate over nonexistent nodes
    if (WGraph->GetEdges() == 0) {
      break;
    }

    // Check whether this is the only out-edge for the source node
    if ((OutDegH.GetDat(EI.GetSrcNId()) == 1) && (InDegH.GetDat(EI.GetDstNId()) > 1)) {
      // If so, remove edge only if not significant for destination node
      if (!(pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)) < alpha)) {
        WGraph->DelEdge(EI.GetSrcNId(), EI.GetDstNId());
        // Iterating the EI forward isn't necessary as removing 
        // an edge implicitly iterates the EI to the next edge.
      } else {
        // But if we haven't removed an edge, move the EI forward.
        EI++;
      }
    
    // Check whether this is the only in-edge for the destination node
    } else if ((OutDegH.GetDat(EI.GetSrcNId()) > 1) && (InDegH.GetDat(EI.GetDstNId()) == 1)) {
    //   If so, remove edge only if not significant for source node
      if (!(pow((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId())), (OutDegH.GetDat(EI.GetSrcNId()) - 1)) < alpha)) {
        WGraph->DelEdge(EI.GetSrcNId(), EI.GetDstNId());
      } else {
        EI++;
      }
    // Check whether this is not the only out-edge for source and not the only in-edge for destination
    } else if ((OutDegH.GetDat(EI.GetSrcNId()) > 1) && (InDegH.GetDat(EI.GetDstNId()) > 1)) {
      // If so, remove edge only if not significant for either side.
      if (!
        ((pow((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId())), (OutDegH.GetDat(EI.GetSrcNId()) - 1)) < alpha) || 
                (pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)) < alpha))) {
            WGraph->DelEdge(EI.GetSrcNId(), EI.GetDstNId());
          } else {
            EI++;
        }
    // An edge is kept if it is the only out-edge for the source and only in-edge for destination
    // which aligns with the Vespignani paper.
    } else {
      EI++;
    }
  }
}

} // namespace TSnap

#endif
