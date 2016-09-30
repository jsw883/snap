//#//////////////////////////////////////////////
/// Graph filtering

#ifndef core_wfilter
#define core_wfilter

#include "wdeg.h"
#include "deg.h"
#include <cmath>

// Headers (?)

//#//////////////////////////////////////////////
/// Vespignani method

namespace TSnap {

/// Prunes using the Vespignani method (for more info see: http://www.pnas.org/content/106/16/6483.full)
template <class TEdgeW, template <class> class TGraph > static TPt<TGraph<TEdgeW> > FilterEdgesVespignani(const TPt<TGraph<TEdgeW> >& Graph, const TIntFltH& InWDegH, const TIntFltH& OutWDegH, const TIntIntH& InDegH, const TIntIntH& OutDegH, const double& alpha);
/// Find Vespignani threshold corresponding to a specific ratio of total edge weight remaining
template <class TEdgeW, template <class> class TGraph > double FindVespignaniThreshold(const TPt<TGraph<TEdgeW> >& Graph, const double& ratio = 0.5, const double& tol = 1e-3, const double& lowerBound = 0.0, const double& upperBound = 1.0);

/// Prunes using the Vespignani method (for more info see: http://www.pnas.org/content/106/16/6483.full)
template <class TEdgeW, template <class> class TGraph >
static TPt<TGraph<TEdgeW> > FilterEdgesVespignani(const TPt<TGraph<TEdgeW> >& Graph, const double& alpha) {
  typename PFltWNGraph::TObj::TEdgeI EI;
  TPt<TGraph<TEdgeW> > GraphCopy = TGraph<TEdgeW>::New(Graph->GetNodes(), Graph->GetEdges());
  *GraphCopy = *Graph;

  // Declare variables
  TIntFltH OutWDegH, InWDegH;
  TIntIntH OutDegH, InDegH;
  
  // Get initial distribution of degrees, weighted and unweighted
  TSnap::GetWInDegH(GraphCopy, InWDegH);
  TSnap::GetWOutDegH(GraphCopy, OutWDegH);
  TSnap::GetInDegH(GraphCopy, InDegH);
  TSnap::GetOutDegH(GraphCopy, OutDegH);
  
  // Implicitly iterated forward if an edge is deleted, else explicitly iterate forward with EI++
  for (EI = GraphCopy->BegEI(); EI < GraphCopy->EndEI(); ) {
    // Break early to avoid segfault if all edges are removed when alpha = 0
    if (GraphCopy->GetEdges() == 0) {
      break;
    }
    // Check if the only out edge for the source node
    if ((OutDegH.GetDat(EI.GetSrcNId()) == 1) && (InDegH.GetDat(EI.GetDstNId()) > 1)) {
      // Remove edge only if not significant for destination node
      if (!(pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)) < alpha)) {
        GraphCopy->DelEdge(EI);
      } else {
        EI++;
      }
    // Check if the only in edge for the destination node
    } else if ((OutDegH.GetDat(EI.GetSrcNId()) > 1) && (InDegH.GetDat(EI.GetDstNId()) == 1)) {
      // Remove edge only if not significant for source node
      if (!(pow((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId())), (OutDegH.GetDat(EI.GetSrcNId()) - 1)) < alpha)) {
        GraphCopy->DelEdge(EI);
      } else {
        EI++;
      }
    // Check if not the only out edge for source node and not the only in edge for destination mpde
    } else if ((OutDegH.GetDat(EI.GetSrcNId()) > 1) && (InDegH.GetDat(EI.GetDstNId()) > 1)) {
      // Remove edge only if not significant for either side
      if (!
        ((pow((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId())), (OutDegH.GetDat(EI.GetSrcNId()) - 1)) < alpha) || 
                (pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)) < alpha))) {
            GraphCopy->DelEdge(EI);
          } else {
            EI++;
        }
    // Edge is retained if the only out edge for the source and the only in edge for the destination node
    } else {
      EI++;
    }
  }
  return GraphCopy;
}

/// Find Vespignani threshold corresponding to a specific ratio of total edge weight remaining
template <class TEdgeW, template <class> class TGraph >
double FindVespignaniThreshold(const TPt<TGraph<TEdgeW> >& Graph, const double& ratio, const double& tol, const double& lowerBound, const double& upperBound) {
  // Variables
  double TargetW = Graph->GetTotalW(), FilteredW = 0;
  double al = lowerBound, ar = upperBound, a;
  TPt<TGraph<TEdgeW> > GraphCopy = TGraph<TEdgeW>::New(Graph->GetNodes(), Graph->GetEdges());
  
  printf("--------------------------------------\n");
  printf("Binary search for Vespignani threshold\n");
  printf("--------------------------------------\n");

  while (2 * std::abs(ratio - FilteredW / TargetW) > tol) {
    a = (ar + al) / 2;
    
    GraphCopy.Clr();
    GraphCopy = FilterEdgesVespignani<TFlt, TWNGraph>(Graph, a);
    FilteredW = GraphCopy->GetTotalW();
    
    if (FilteredW / TargetW < ratio) {
      printf("(l: %f) --> ||     (r: %f)\n", al, ar);
      al = a;
    } else {
      printf("(l: %f)     || <-- (r: %f)\n", al, ar);
      ar = a;
    }
    
  }
  
  printf("--------------------------------------\n");
  printf("Alpha (a: %f)\n", a);
  printf("--------------------------------------\n");
  printf("\n");
  
  return a;

}

} // namespace TSnap

#endif
