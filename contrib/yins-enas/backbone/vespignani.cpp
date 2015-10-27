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
  const float alpha = Env.GetIfArgPrefixInt("-a:", 2, "level of significance alpha");

  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", WGraph->GetNodes());
  printf("  edges: %d\n", WGraph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Declare variables
  // New graph that keeps surviving backbone edges
  PFltWNGraph WGraphBkbn;
  // Hashtables for source and destination weight sums
  TIntFltVH SrcWSumH, DstWSumH;
  
  // Part A: Get weight sums

  printf("\nComputing source and destination weight sums...");
  TSnap::GetWSumsH(WGraph, SrcWSumH, DstWSumH);
  // Computes weight sums (source & destination)
  template <class TEdgeW, template <class> class TGraph >
  void GetWSumsH(const TPt<TGraph<TEdgeW> >& WGraph, THash<TInt, TVec<TEdgeW> >& SrcWSumH, THash<TInt, TVec<TEdgeW> >& DstWSumH) {
    typename TGraph<TEdgeW>::TEdgeI EI;
    // Initialize hashtables with NodeIds
    // Is this what this does?  I'm not sure.
    // Ideally, we initialize hashtable with NodeIds and set all data to 0.
    // Also, would it be update the sums on every iteration
    // if I implemented one hashtable of vectors?
    // That would require less memory 
    SrcWSumH.Gen(WGraph->GetNodes());
    DstWSumH.Gen(WGraph->GetNodes());

    for (EI = WGraph->BegEI(); EI < WGraph->EndEI(); EI++) {
      // Does AddDat update the data?  
      // If not, what does?  
      // Update sum weight for corresponding source node
      SrcWSumH.AddDat(EI.GetId(), SrcWSumH.GetDat(EI.GetSrcNId()) + EI.GetW());

      // Update sum weight for corresponding destination node
      DstWSumH.AddDat(EI.GetId(), DstWSumH.GetDat(EI.GetDstNId()) + EI.GetW());
    }
  }
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Part B: Disparity Filter

  printf("\n Applying the disparity filter...");

  for (EI = WGraph->BegEI(); EI < WGraph->EndEI(); EI++) {
    int degSrc; // degree of source node
    int degDst; // degree of destination node
    // Is it possible to GetOutDeg and GetInDeg with NodeId?
    // Otherwise my plan is to implement two more hastables
    // Iterate over all the nodes once to fill in the in and out degrees

    // If the edge meets the criterion for either the source or the destination,
    // add it to the backbone graph. 
    if (((1 - EI.GetW() / SrcWSumH.GetDat(EI.GetSrcNId)) ** (degSrc - 1) < alpha) || ((1 - EI.GetW() / DstWSumH.GetDat(EI.GetDstNId)) ** (degDst - 1)) < alpha) {
      WGraphBkbn.AddEdge(EI.GetSrcNId(), EI.GetDstNId(), EI.GetW());
    }

  }
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());


  // OUTPUTTING (mostly verbose printing statements, don't get scared)
  
  printf("\nSaving %s...", BseFNm.CStr());
  WGraphBkbn.SaveFltWEdgeList(WGraphBkbn, OutFNm);
  printf(" DONE\n");

  
  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}