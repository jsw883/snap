#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
   
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Backbone extractor (Vespignani). build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output network name(filename extensions added)");
  const float alpha = Env.GetIfArgPrefixFlt("-a:", 0.01, "level of significance alpha");

  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE\n");
  printf("  nodes: %d\n", WGraph->GetNodes());
  printf("  edges: %d\n", WGraph->GetEdges());
  printf("  time elapsed: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());

  // Declare variables

  // Hashtables for source and destination weight sums
  TIntFltH OutWDegH, InWDegH;

  // Hashtables for binary source and destination degrees
  TIntIntH OutDegH, InDegH;

  
  // Part A: Get weight sums

  printf("\nGetting source and destination weight sums...");
  TSnap::GetWOutDegH(WGraph, OutWDegH);
  TSnap::GetWInDegH(WGraph, InWDegH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());

  printf("\nGetting binary source and destination degrees...");
  TSnap::GetOutDegSeqH(WGraph, OutDegH);
  TSnap::GetInDegSeqH(WGraph, InDegH);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Part B: Disparity Filter

  printf("\n Applying the disparity filter...");
  typename PFltWNGraph::TObj::TEdgeI EI;
  // Iterates over the edges, and applies the "skip over empty nodes" (EI--) operator
  // at the end of each iteration.  This iterates through the nodes until the out-degree
  // of the current node is zero, or if the end node is reached.  See lines 183 - 189
  // of wgraph.h
  // The -- operator was used because I'm not sure what other symbols can be used.
  for (EI = WGraph->BegEI(); EI < WGraph->EndEI(); EI--) {
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
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());

  // OUTPUTTING 

  printf("Pruned graph:\n");
  printf("  nodes: %d\n", WGraph->GetNodes());
  printf("  edges: %d\n", WGraph->GetEdges());
  
  printf("\nSaving...");
  TSnap::SaveFltWEdgeList(WGraph, TStr::Fmt("%s-%f.snap", OutFNm.CStr(), alpha), "");
  printf(" DONE\n");

  
  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}