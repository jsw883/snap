#include "stdafx.h"


int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
   
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Vespignani backbone method. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (alpha value and filename extensions added)");
  double alpha = Env.GetIfArgPrefixFlt("-a:", 0.01, "alpha significance level threshold");
  const TStr AlphaVFNm = Env.GetIfArgPrefixStr("--alphav:", "", "vector of alpha significance level threshold (overrides -a)");
  const bool verbose = Env.GetIfArgPrefixBool("--verbose:", true, "verbose output for each step of the Vespignani method");

  const bool bootstrap = Env.GetIfArgPrefixBool("--bootstrap:", false, "bootstrap Vespignani method to retain --ratio of total weight W");
  const double ratio = Env.GetIfArgPrefixFlt("--ratio:", 0.50, "bootstrap target ratio of total weight W");
  const double lowerBound = Env.GetIfArgPrefixFlt("--lowerbound:", 0.0, "lower bound for alpha (binary search)");
  const double upperBound = Env.GetIfArgPrefixFlt("--upperbound:", 1.0, "upper bound for alpha (binary search)");
  const double tol = Env.GetIfArgPrefixFlt("--tolerance:", 5.0e-3, "tolerance for alpha (binary search)");
  const double spread = Env.GetIfArgPrefixFlt("--spread:", 2.0, "spread for bootstrapped alpha benchmark (binary search)");

  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  const PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  // Variables
  
  PFltWNGraph WGraphCopy;
  TFltV AlphaV;
  TFltV::TIter VI;
  
  if (!AlphaVFNm.Empty()) {
    AlphaV = TSnap::LoadTxtFltV(AlphaVFNm);
  } else {
    if (bootstrap) {
      alpha = TSnap::FindVespignaniThreshold<TFlt, TWNGraph>(WGraph, ratio, tol, lowerBound, upperBound);
      AlphaV.Add(alpha / spread);
      AlphaV.Add(alpha);
      AlphaV.Add(alpha * spread);
    } else {
      AlphaV.Add(alpha);
    }
  }

  // VESPIGNANI METHOD

  Progress progress(ExeTm, AlphaV.Len(), 5, "Computing Vespignani method", !verbose);
  progress.start();
  if (verbose) {
    printf("\n");
  }
  int i = 0;
  for (VI = AlphaV.BegI(); VI < AlphaV.EndI(); VI++) {
    const double& alpha = VI->Val;
    
    // Compute method and save filtered

    printf("Computing Vespignani method (alpha: %e)\n\n", alpha);
    
    WGraphCopy = TSnap::FilterEdgesVespignani<TFlt, TWNGraph>(WGraph, alpha);
    TSnap::RemoveIsolated(WGraphCopy);
    TSnap::SaveFltWEdgeList(WGraphCopy, TStr::Fmt("%s-%9e.snap", OutFNm.CStr(), alpha), TStr::Fmt("Vespignani backbone with alpha: %e", alpha));

    // Save bootstrapped
    
    if (bootstrap && i == 1) {
      TSnap::SaveFltWEdgeList(WGraphCopy, TStr::Fmt("%s-bootstrapped.snap", OutFNm.CStr()), TStr::Fmt("Vespignani backbone with alpha: %e", alpha));
    }
    
    // Verbose summary

    if (verbose) {
      TSnap::printFltWGraphSummary(WGraphCopy, true, TStr::Fmt("WGraphCopy (alpha: %e)\n------", alpha));
      printf("\n");
    }

    i++;
    progress++;
  }

  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}