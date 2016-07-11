#include "stdafx.h"


int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
   
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Vespignani backbone method. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (alpha value and filename extensions added)");
  const float alpha = Env.GetIfArgPrefixFlt("-a:", 0.01, "alpha significance level threshold");
  const TStr AlphaVFNm = Env.GetIfArgPrefixStr("--alphav:", "", "vector of alpha significance level threshold (overrides -a)");
  const bool verbose = Env.GetIfArgPrefixBool("--verbose:", true, "verbose output for each step of the Vespignani method");

  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  const PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  // Variables
  
  PFltWNGraph WGraphCopy = PFltWNGraph::New();
  TFltV AlphaV;
  TFltV::TIter VI;
  
  if (!AlphaVFNm.Empty()) {
    AlphaV = TSnap::LoadTxtFltV(AlphaVFNm);
  } else {
    AlphaV.Add(alpha);
  }
  
  // VESPIGNANI METHOD

  Progress progress(ExeTm, AlphaV.Len(), 5, "Computing Vespignani method", !verbose);
  if (verbose) {
    printf("\n");
  }
  for (VI = AlphaV.BegI(); VI < AlphaV.EndI(); VI++) {
    const double& alpha = VI->Val;
    
    printf("Computing Vespignani method (alpha: %e)", alpha);
    printf("\n");
	
    *WGraphCopy = *WGraph;
    
    TSnap::FilterEdgesVespignani<TFlt, TWNGraph>(WGraphCopy, alpha);
    TSnap::RemoveIsolated(WGraphCopy);
    TSnap::SaveFltWEdgeList(WGraphCopy, TStr::Fmt("%s-%9e.snap", OutFNm.CStr(), alpha), TStr::Fmt("Vespignani backbone with alpha: %e", alpha));
    
    if (verbose) {
      TSnap::printFltWGraphSummary(WGraphCopy, true, TStr::Fmt("WGraphCopy (alpha: %e)\n------", alpha));
      printf("\n");
    }
    
    progress++;
  }
  
  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}