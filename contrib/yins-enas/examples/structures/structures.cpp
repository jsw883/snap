#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Graph connectivity. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const double p = Env.GetIfArgPrefixFlt("-p:", 0.5, "percolation probability");
  const TEdgeDir d = (TEdgeDir) Env.GetIfArgPrefixInt("-d:", 3, "direction of traversal: in = 1, out = 2, undected = 3");
  const double iters = Env.GetIfArgPrefixFlt("--iters:", 10, "number of iterations to average results");
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", Graph->GetNodes());
  printf("  edges: %d\n", Graph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Declare variables
  PNGraph GraphCopy;
  TCnComV WCnComV;
  TCnComV::TIter WCnComI;
  int iter, nodes;
  double radius, diameter;
  double AvSize, AvRadius, AvDiameter;
  double AvRadiusToSizeRatio, AvDiameterToSizeRatio;
  TIntV NF, NCnComV;
  TIntV GiantSizeV;
  TFltV GiantSizeRatioV;
  TFltV AvSizeV, AvRadiusV, AvDiameterV;
  TFltV AvRadiusToSizeRatioV, AvDiameterToSizeRatioV;

  // Set random seed based on time (hacky, but better than not setting)
  srand48(time(NULL));

  // STRUCTURES (computations)
  
  for (iter = 0; iter < iters; iter++) {
    // Percolate graph according to percolation probability
    GraphCopy = TSnap::PercolateGraph<PNGraph>(Graph, p);
    // Get weakly connected components (cluster)
    TSnap::GetWccs(GraphCopy, WCnComV);
    // Counts and giant sizes
    NCnComV.Add(WCnComV.Len());
    GiantSizeV.Add(WCnComV[0].Len());
    GiantSizeRatioV.Add(((double) WCnComV[1].Len()) / ((double) WCnComV[0].Len()));
    // Compute average size, radius, diameter, and size ratios
    TSnap::TFixedMemoryNeighborhood<PNGraph> FixedMemoryNeighborhood(GraphCopy);
    AvSize = 0;
    AvRadius = 0;
    AvRadiusToSizeRatio = 0;
    AvDiameter = 0;
    AvDiameterToSizeRatio = 0;
    for (WCnComI = WCnComV.BegI(); WCnComI < WCnComV.EndI(); WCnComI++) {
      // Compute the nodes, radius, and diameter
      FixedMemoryNeighborhood.ComputeSubsetExactNF(WCnComI->NIdV, edOutDirected, NF);
      nodes = WCnComI->Len();
      radius = TSnap::InterpolateNF(NF, 0.5);
      diameter = TSnap::InterpolateNF(NF, 1.0);
      // Increment averages
      AvSize += nodes;
      AvRadius += radius;
      AvRadiusToSizeRatio += radius / nodes;
      AvDiameter += diameter;
      AvDiameterToSizeRatio += diameter / nodes;
    }
    AvSizeV.Add(AvSize / NCnComV.Last());
    AvRadiusV.Add(AvRadius / NCnComV.Last());
    AvRadiusToSizeRatioV.Add(AvRadiusToSizeRatio / NCnComV.Last());
    AvDiameterV.Add(AvDiameter / NCnComV.Last());
    AvDiameterToSizeRatioV.Add(AvDiameterToSizeRatio / NCnComV.Last());
  }
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  printf("\nSaving %s.structures.summary...", BseFNm.CStr());
  const TStr CombinedFNm = TStr::Fmt("%s.structures.summary", OutFNm.CStr());
  FILE *F = fopen(CombinedFNm.CStr(), "wt");
  fprintf(F, "# Percolation structures summary for p = %f, d = %d\n", p, d);
  fprintf(F, "# Nodes: %d\tEdges: %d\n", Graph->GetNodes(), Graph->GetEdges());
  fprintf(F, "# NCnCom\tGiantSize\tGiantSizeRatio\tAvSize\tAvRadius\tAvRadiusToSizeRatio\tAvDiameter\tAvDiameterToSizeRatio\n");
  for (iter = 0; iter < iters; iter++) {
    fprintf(F, "%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f", (int) NCnComV[iter], (int) GiantSizeV[iter], (double) GiantSizeRatioV[iter], (double) AvSizeV[iter], (double) AvRadiusV[iter], (double) AvRadiusToSizeRatioV[iter], (double) AvDiameterV[iter], (double) AvDiameterToSizeRatioV[iter]);
    fprintf(F, "\n");
  }
  printf(" DONE\n");
  
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}