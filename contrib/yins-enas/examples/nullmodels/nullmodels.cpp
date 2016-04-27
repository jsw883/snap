#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Weighted graph null models. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", " input network (tab separated list of edges with edge weights)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const TStr Method = Env.GetIfArgPrefixStr("-g:", "GWShuffling", "generator or algorithm to apply (default: 3)");
  const int k = Env.GetIfArgPrefixInt("-k:", -1, "edges (default: Graph->GetEdges())");
  const double Threshold = Env.GetIfArgPrefixFlt("--threshold:", -1, "lower threshold for exponentially weighted random graph model");
  double Scale = Env.GetIfArgPrefixFlt("--scale:", -1, "Pareto scale for preferential attachment model (default: FitParetoWeights(Graph, Scale, Shape))");
  double Shape = Env.GetIfArgPrefixFlt("--shape:", -1, "Pareto shape for preferential attachment model (default: FitParetoWeights(Graph, Scale, Shape))");
  
  int nodes = Env.GetIfArgPrefixInt("-n:", -1, "nodes (default: Graph->GetNodes())");
  // int edges = Env.GetIfArgPrefixInt("-m:", -1, "edges (default: Graph->GetEdges())");
  double TotalW = Env.GetIfArgPrefixFlt("-w:", -1, "sum of weights (default: Graph->GetTotalW())");

  PFltWNGraph WGraph, WRGraph;

  if (InFNm == "") {

    // TODO: this needs to be generalized
    IAssertR(Method == "GWShuffling", "Input network must be provided for randomization algorithms (g > 3).");

  } else {

    // Load graph and create directed and undirected graphs (pointer to the same memory)
    printf("\nLoading %s...", InFNm.CStr());
    WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
    printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
    
    TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
    
    nodes = WGraph->GetNodes();
    // edges = WGraph->GetEdges();
    TotalW = WGraph->GetTotalW();

    TSnap::FitParetoWeights<TFlt, TWNGraph>(WGraph, Scale, Shape);

  }

  // WEIGHTED RANDOM GRAPH GENERATORS
  
  // TODO: extend `==` to .IsStrIn(Method) to allow multiple algorithms applied

  TRnd Rnd(0);

  printf("Generating random graph model...");
  
  if (Method == "ExpWRGM") {
    IAssertR(false, "ExpWRGM is not currently enabled.");
    IAssertR(TotalW != -1, "w must be specified.");
    IAssertR(Threshold != -1, "Threshold must be specified.");
    // WRGraph = TSnap::GenExpErdosRenyi<TFlt, TWNGraph>(nodes, TotalW, Threshold, Rnd);
  }
  
  if (Method == "GeoWRGM") {
    IAssertR(false, "GeoWRGM is not currently enabled.");
    IAssertR(TotalW != -1, "w must be specified.");
    // WRGraph = TSnap::GenGeoErdosRenyi<TFlt, TWNGraph>(nodes, TotalW, Rnd);
  }
  
  if (Method == "WPrefAttach") {
    IAssertR(k != -1, "k must be specified.");
    WRGraph = TSnap::GenParetoBarabasi<TFlt, TWNGraph>(nodes, k, Scale, Shape, edUnDirected, Rnd);
    TSnap::printFltWGraphSummary(WRGraph, true, "\nWRGraph\n-------");
  }
  
  if (Method == "GWShuffling") {
    TSnap::WeightShuffling<TFlt>(WGraph);
    WRGraph = WGraph;
    TSnap::printFltWGraphSummary(WRGraph, true, "\nWRGraph\n-------");
  }
  
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  printf("\nSaving %s-%s.snap...", OutFNm.CStr(), Method.CStr());
  TSnap::SaveFltWEdgeList(WRGraph, TStr::Fmt("%s-%s.snap", OutFNm.CStr(), Method.CStr()), "");
  printf(" DONE\n");

  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}