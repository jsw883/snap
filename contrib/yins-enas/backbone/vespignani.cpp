// THIS CODE WILL NOT COMPILE (yet, hopefully)
// very rough first draft of implementation of backbone extraction method
// detailed by Serrano, Boguna, and Vespignani
// for use within the SNAP library, extended by James Williams for use
// with weight directed graphs.
//
// This is a heavily modded version of wcentrality.cpp and wdeg.h by James Williams

#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
   
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Backbone extractor (Vespignani). build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output network name(filename extensions added)");
  const float alpha = Env.GetIfArgPrefixInt("-a:", 0.01, "level of significance alpha");

  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", WGraph->GetNodes());
  printf("  edges: %d\n", WGraph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
)  // Declare variables
  // New graph that keeps surviving backbone edges
  PFltWNGraph WGraphBkbn;
  // Hashtables for source and destination weight sums
  TIntFltVH OutWDegH, InWDegH;
  // Replace these hashtables with GetWInDegH and GetWOutDegH from wgraph.h
  // GetWInDegH corresponds to InWDegHt
  // GetWOutDegH corresponds to OutWDegH


  // Part A: Get weight sums

  printf("\nComputing source and destination weight sums...");
  TSnap::GetWSumsH(WGraph, OutWDegH, InWDegH);
  // Computes weight sums (source & destination)
  template <class TEdgeW, template <class> class TGraph >
  void GetWSumsH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& OutWDegH, THash<TInt, TVec<TEdgeW> >& InWDegH) {
    typename TGraph<TEdgeW>::TEdgeI EI;
    // Initialize hashtables with NodeIds
    // Is this what this does?  I'm not sure.
    // Ideally, we initialize hashtable with NodeIds and set all data to 0.
    // Also, would it be update the sums on every iteration
    // if I implemented one hashtable of vectors?
    // That would require less memory 
    OutWDegH.Gen(WGraph->GetNodes());
    InWDegH.Gen(WGraph->GetNodes());

    for (EI = WGraph->BegEI(); EI < WGraph->EndEI(); EI++) {
      // Does AddDat update the data?  
      // If not, what does?  
      // Update sum weight for corresponding source node
      OutWDegH.AddDat(EI.GetSrcNId(), OutWDegH.GetDat(EI.GetSrcNId()) + EI.GetW());

      // Update sum weight for corresponding destination node
      InWDegH.AddDat(EI.GetSrcNId(), InWDegH.GetDat(EI.GetDstNId()) + EI.GetW());
    }
  }
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Part B: Disparity Filter

  printf("\n Applying the disparity filter...");

  for (EI = WGraph->BegEI(); EI < WGraph->EndEI(); EI++) {

    // Use GetInDegSeqH and GetOutDegSeqH at the beginning
    // Is it possible to GetOutDeg and GetInDeg with NodeId?
    // Otherwise my plan is to implement two more hastables
    // Iterate over all the nodes once to fill in the in and out degrees

    // If the edge meets the criterion for either the source or the destination,
    // add it to the backbone graph. 

    // If we were computing this on the fly, WGraph.GetNI(EI.GetSrcNId).GetOutWDeg(); replaces the 
    // hashtable lookup for WDeg of Source Node ID
    // This is if the calculation is not iteration independent (this command only works if
    // we are removing edges in place.)
    if (!
      ((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId)) ** (OutDegH.GetDat(EI.GetSrcNId) - 1) < alpha) || 
      ((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId)) ** (DegH.GetDat(EI.GetDstNId) - 1) < alpha)
      ) {
      WGraph.DelEdge(EI.GetSrcNId(), EI.GetDstNId());
    }

  }
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());


  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  printf("\nSaving %s...", BseFNm.CStr());
  WGraph.SaveFltWEdgeList(WGraphBkbn, OutFNm);
  printf(" DONE\n");

  
  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}