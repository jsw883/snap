#include "stdafx.h"


int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
   
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Vespignani backbone method. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (alpha value and filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const TEdgeDir d = (TEdgeDir) Env.GetIfArgPrefixInt("-d:", 3, "direction of traversal: in = 1, out = 2, undected = 3");

  const double lowerAlphaBound = Env.GetIfArgPrefixFlt("--loweralphabound:", 0.0, "lower bound for alpha (iterative)");
  const double upperAlphaBound = Env.GetIfArgPrefixFlt("--upperalphabound:", 1.0, "upper bound for alpha (iterative)");
  const double step = Env.GetIfArgPrefixFlt("--step:", 5.0e-2, "alpha step size (iterative)");

  const int lowerSizeBound = Env.GetIfArgPrefixInt("--lowersizebound:", 2, "lower bound for weakly connected component sizes");
  const double upperSizeRatioBound = Env.GetIfArgPrefixFlt("--uppersizeratio:", 0, "upper bound for weakly connected component sizes");

  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  const PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  // Variables
  
  PFltWNGraph WGraphCopy;
  TFltV AlphaV;
  TFltV::TIter VI;

  TCnComV WCnComV;  
  TCnComV::TIter WCnComI;
  int wcncom, nodes, upperSizeBound;
  double radius, diameter;
  TIntV WCnComNodesV;
  TFltV WCnComRadiusV, WCnComDiameterV;
  double AvSize, AvRadius, AvDiameter;
  double AvRadiusToSizeRatio, AvDiameterToSizeRatio;
  TUInt64V NF;
  TIntV NCnComV;
  TIntV GiantSizeV;
  TFltV GiantSizeRatioV;
  TFltV AvSizeV, AvRadiusV, AvDiameterV;
  TFltV AvRadiusToSizeRatioV, AvDiameterToSizeRatioV;
  
  int i, j;

  for (double alpha = lowerAlphaBound; alpha <= upperAlphaBound; alpha += step) {
    AlphaV.Add(alpha);
  }

  // VESPIGNANI METHOD

  Progress progress(ExeTm, AlphaV.Len(), 5, "Computing Vespignani method");
  progress.start();
  i = 0;
  for (VI = AlphaV.BegI(); VI < AlphaV.EndI(); VI++) {
    const double& alpha = VI->Val;

    // Compute method and save filtered

    WGraphCopy = TSnap::FilterEdgesVespignani<TFlt, TWNGraph>(WGraph, alpha);

     // Get weakly connected components (cluster)
    upperSizeBound = upperSizeRatioBound * WGraphCopy->GetNodes();
    TSnap::GetWccs(WGraphCopy, WCnComV);

    // Counts and giant sizes
    NCnComV.Add(WCnComV.Len());
    GiantSizeV.Add(WCnComV[0].Len());
    GiantSizeRatioV.Add(((double) WCnComV[1].Len()) / ((double) WCnComV[0].Len()));

    // Compute average size, radius, diameter, and size ratios
    TSnap::TFixedMemoryNeighborhood<PFltWNGraph> FixedMemoryNeighborhood(WGraphCopy);
    // Variables
    WCnComNodesV.Clr();
    WCnComRadiusV.Clr();
    WCnComDiameterV.Clr();
    AvSize = 0;
    AvRadius = 0;
    AvRadiusToSizeRatio = 0;
    AvDiameter = 0;
    AvDiameterToSizeRatio = 0;
    j = 0;
    for (WCnComI = WCnComV.BegI(); WCnComI < WCnComV.EndI(); WCnComI++) {
      nodes = WCnComI->Len();
      if (nodes < lowerSizeBound || nodes > upperSizeBound) {
        continue;
      }
      // Compute the nodes, radius, and diameter
      FixedMemoryNeighborhood.ComputeSubsetNF(WCnComI->NIdV, d, NF);
      radius = TSnap::InterpolateNF(NF, 0.5);
      diameter = TSnap::InterpolateNF(NF, 1.0);
      // Save
      WCnComNodesV.Add(nodes);
      WCnComRadiusV.Add(radius);
      WCnComDiameterV.Add(diameter);
      // Averages
      AvSize += nodes;
      AvRadius += radius;
      AvRadiusToSizeRatio += radius / nodes;
      AvDiameter += diameter;
      AvDiameterToSizeRatio += diameter / nodes;
      j++;
    }
    
    // Save
    const TStr CombinedFNm = TStr::Fmt("%s.step.summary.%9e", OutFNm.CStr(), alpha);
    FILE *F = fopen(CombinedFNm.CStr(), "wt");
    fprintf(F, "# Iterative vespignani weakly connected components summary for alpha = %f, d = %d, i = %d\n", alpha, d, i);
    fprintf(F, "# WCnComs: %d\n", WCnComV.Len());
    fprintf(F, "# WCnComId\tNodes\tRadius\tDiameter\n");
    for (wcncom = 0; wcncom < j; wcncom++) {
      fprintf(F, "%d\t%d\t%f\t%f", wcncom, (int) WCnComNodesV[wcncom], (double) WCnComRadiusV[wcncom], (double) WCnComDiameterV[wcncom]);
      fprintf(F, "\n");
    }

    // Averages
    AvSizeV.Add(AvSize / NCnComV.Last());
    AvRadiusV.Add(AvRadius / NCnComV.Last());
    AvRadiusToSizeRatioV.Add(AvRadiusToSizeRatio / NCnComV.Last());
    AvDiameterV.Add(AvDiameter / NCnComV.Last());
    AvDiameterToSizeRatioV.Add(AvDiameterToSizeRatio / NCnComV.Last());

    i++;
    progress++;
  }

  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  printf("\nSaving %s.structures.summary...", BseFNm.CStr());
  const TStr CombinedFNm = TStr::Fmt("%s.structures.summary", OutFNm.CStr());
  FILE *F = fopen(CombinedFNm.CStr(), "wt");
  fprintf(F, "# Iterative vespignani structures summary for alpha = %f:%f:%f, d = %d\n", lowerAlphaBound, step, upperAlphaBound, d);
  fprintf(F, "# Nodes: %d\tEdges: %d\n", WGraph->GetNodes(), WGraph->GetEdges());
  fprintf(F, "# NCnCom\tGiantSize\tGiantSizeRatio\tAvSize\tAvRadius\tAvRadiusToSizeRatio\tAvDiameter\tAvDiameterToSizeRatio\n");
  for (i = 0; i < AlphaV.Len(); i++) {
    fprintf(F, "%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f", (int) NCnComV[i], (int) GiantSizeV[i], (double) GiantSizeRatioV[i], (double) AvSizeV[i], (double) AvRadiusV[i], (double) AvRadiusToSizeRatioV[i], (double) AvDiameterV[i], (double) AvDiameterToSizeRatioV[i]);
    fprintf(F, "\n");
  }
  printf(" DONE\n");

  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}
