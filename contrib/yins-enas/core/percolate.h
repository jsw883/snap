//#//////////////////////////////////////////////
/// Graph traversal algorithms

#ifndef core_percolate
#define core_percolate

// Headers (?)

//#//////////////////////////////////////////////
/// Graph percolation theory

void GetWccStats(const TIntPrV& WccSzCnt, int& numWcc, int& mnWccSz, double& medWccSz, double& meanWccSz, int& mxWccSz);

namespace TSnap {

// template <class PGraph> static PGraph EdgePercolateGraph(const PGraph& Graph, const double& p = 0.5, const bool& PercolatedUndirected = true);
template <class PGraph> static PGraph EdgePercolateGraph(const PGraph& Graph, const double& p = 0.5);
template <class PGraph> double FindPercolationThreshold(const PGraph& Graph, const double& tol = 1e-3, const double& lowerBound = 0.0, const double& upperBound = 1.0, const int& rep = 10);

// template <class PGraph>
// static PGraph EdgePercolateGraph(const PGraph& Graph, const double& p, const bool& PercolatedUndirected) {
//   typename PGraph::TObj::TNodeI NI;
//   PGraph GraphCopy = PGraph::TObj::New();
//   *GraphCopy = *Graph;
//   // Iterate through the nodes, randomly delete all edges between two nodes with percolation probability
//   for (NI = GraphCopy->BegNI(); NI < GraphCopy->EndNI(); ) {
//     for (int edge = 0; edge < NI.GetOutDeg(); edge++) {
//       if (drand48() < p) {
//         Graph->DelEdge(NI.GetId(), NI.GetOutNId());
//         if (Graph.HasFlag(gfDirected) && PercolatedUndirected) {
//           Graph->DelEdge(NI.GetOutNId(), NI.GetId());
//         }
//       }
//     }
//   }
//   return GraphCopy;
// }

template <class PGraph>
static PGraph EdgePercolateGraph(const PGraph& Graph, const double& p) {
  typename PGraph::TObj::TEdgeI EI;
  PGraph GraphCopy = PGraph::TObj::New();
  *GraphCopy = *Graph;
  // Iterate through the edges, randomly delete with percolation probability
  for (EI = GraphCopy->BegEI(); EI < GraphCopy->EndEI(); ) {
    if (drand48() < p) {
      GraphCopy->DelEdge(EI);
    } else {
      EI++;
    }
  }
  return GraphCopy;
}

template <class PGraph>
double FindPercolationThreshold(const PGraph& Graph, const double& tol, const double& lowerBound, const double& upperBound, const int& rep) {
  // Reserver memory now for efficiency
  PGraph GraphCopy = PGraph::TObj::New(Graph->GetNodes(), Graph->GetEdges());
  TIntPrV WccSzCnt;
  int numNodes = Graph->GetNodes();
  int numWcc, mnWccSz, mxWccSz,  nextWccSz;
  double medWccSz, meanWccSz;
  double pl = lowerBound, pr = upperBound, p;
  
  printf("----------------------------------------\n");
  printf("Binary search for percolation threshold\n");
  printf("----------------------------------------\n");
  
  int i, temp;
  while (2 * (pr - pl) > tol) {
    p = (pr + pl) / 2;
    temp = 0;
    for (i = 0; i < rep; i++) {  // TODO: adaptive smoothing
      GraphCopy.Clr();
      WccSzCnt.Clr();
      GraphCopy = EdgePercolateGraph(Graph, p);
      
      GetWccSzCnt(GraphCopy, WccSzCnt);
      GetWccStats(WccSzCnt, numWcc, mnWccSz, medWccSz, meanWccSz, mxWccSz);
      nextWccSz = WccSzCnt.Len() > 1 ? WccSzCnt.LastLast().Val1 : WccSzCnt[0].Val1;
      
      // TODO: allow different threshold objective conditions to be applied
      
      // log(numNodes) - log(mxWccSz) < 1
      // log(numNodes) - log(mxWccSz) <= log(mxWccSz) - log(meanWccSz)
      // log(numNodes) - log(mxWccSz) <= log(mxWccSz) - log(nextWccSz)
      
      if (log(numNodes) - log(mxWccSz) <= log(mxWccSz) - log(meanWccSz)) { // <= log(mxWccSz) - log(meanWccSz)) {
        temp += 1;
      }
    }
    
    if (temp > rep / 2) {
      printf("(pl: %f) --> |     (pr: %f)\n", pl, pr);
      pl = p;
    } else {
      printf("(pl: %f)     | <-- (pr: %f)\n", pl, pr);
      pr = p;
    }
    
  }
  
  printf("----------------------------------------\n");
  printf("Threshold (p: %f)\n", p);
  printf("\n");
  
  GraphCopy.Clr();
  WccSzCnt.Clr();
  GraphCopy = EdgePercolateGraph(Graph, p);
  
  GetWccSzCnt(GraphCopy, WccSzCnt);
  GetWccStats(WccSzCnt, numWcc, mnWccSz, medWccSz, meanWccSz, mxWccSz);
  nextWccSz = WccSzCnt.Len() > 1 ? WccSzCnt.LastLast().Val1 : WccSzCnt[0].Val1;
      
  printf("Sample percolation summary\n");
  printf("  probability = %f\n", p);
  printf("  numNodes = %d\n", numNodes);
  printf("  numWcc = %d\n", numWcc);
  printf("  mnWccSz = %d\n", mnWccSz);
  printf("  medWccSz = %f\n", medWccSz);
  printf("  meanWccSz = %f\n", meanWccSz);
  printf("  mxWccSz = %d\n", mxWccSz);
  printf("  secondMxWccSz = %d\n", nextWccSz);
  printf("  log(numNodes) - log(mxWccSz) = %f\n", log(numNodes) - log(mxWccSz));
  printf("  log(mxWccSz) - log(meanWccSz) = %f\n", log(mxWccSz) - log(meanWccSz));
  printf("  log(numNodes) - log(mxWccSz) = %f\n", log(numNodes) - log(mxWccSz));
  printf("  log(mxWccSz) - log(nextWccSz) = %f\n", log(mxWccSz) - log(nextWccSz));
  
  return p;
}

} // namespace TSnap

#endif