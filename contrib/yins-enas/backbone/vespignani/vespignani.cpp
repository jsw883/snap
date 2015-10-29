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

  int edgesCleared = 0;
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
  for (EI = WGraph->BegEI(); EI < WGraph->EndEI(); ) {
  	if (WGraph->GetEdges() == 0) {
  		break;
  	} else if (true){
  		EI--;
		printf("%i\n", WGraph->GetNI(EI.GetSrcNId()).GetOutDeg());
		WGraph->DelEdge(EI.GetSrcNId(), EI.GetDstNId());
		printf("%i\n", WGraph->GetNI(EI.GetSrcNId()).GetOutDeg());
		edgesCleared++;
		printf("%i\n", edgesCleared);
  	} else {
  		EI++;
  	}
  }

  //   // If the edge meets the criterion for either the source or the destination,
  //   // add it to the backbone graph. 

  //   // If we were computing this on the fly, WGraph.GetNI(EI.GetSrcNId).GetOutWDeg(); replaces the 
  //   // hashtable lookup for WDeg of Source Node ID
  //   // This is if the calculation is not iteration independent (this command only works if
  //   // we are removing edges in place.)

  // 	// if ((pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)) < 0) ||
  // 	// 	pow((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId())), (OutDegH.GetDat(EI.GetSrcNId()) - 1)) < 0){
  // 	// 	neg++;
  // 	// } 

  //   // Check whether this is the only out-edge for the source node
  //   if ((OutDegH.GetDat(EI.GetSrcNId()) == 1) && (InDegH.GetDat(EI.GetDstNId()) > 1)) {
  //     // If so, remove edge only if not significant for destination node
  //     if (!(pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)) < alpha)) {
  //       // printf("%f\n", pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)));
  //       WGraph->DelEdge(EI.GetSrcNId(), EI.GetDstNId());
  //     } else {
  //     	printf("%f\n", pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)));
  //     }
  //   }
  //   // Check whether this is the only in-edge for the destination node
  //   else if ((OutDegH.GetDat(EI.GetSrcNId()) > 1) && (InDegH.GetDat(EI.GetDstNId()) == 1)) {
  //     // If so, remove edge only if not significant for source node
  //     if (!(pow((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId())), (OutDegH.GetDat(EI.GetSrcNId()) - 1)) < alpha)) {
  //       WGraph->DelEdge(EI.GetSrcNId(), EI.GetDstNId());
  //   } else {
  //     	printf("%f\n", pow((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId())), (OutDegH.GetDat(EI.GetSrcNId()) - 1)));
  //     }
  //   }
  //   // Check whether this is not the only out-edge for source and not the only in-edge for destination
  //   else if ((OutDegH.GetDat(EI.GetSrcNId()) > 1) && (InDegH.GetDat(EI.GetDstNId()) > 1)) {
  //     // If so, remove edge only if not significant for either side.
  //     if (!
  //       ((pow((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId())), (OutDegH.GetDat(EI.GetSrcNId()) - 1)) < alpha) || 
  //               (pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)) < alpha))
  //       ) {
  //         	WGraph->DelEdge(EI.GetSrcNId(), EI.GetDstNId());
  //         } else {
  //     		printf("%f\n", pow((1 - EI.GetW() / OutWDegH.GetDat(EI.GetSrcNId())), (OutDegH.GetDat(EI.GetSrcNId()) - 1)));
  //     		printf("%f\n", pow((1 - EI.GetW() / InWDegH.GetDat(EI.GetDstNId())), (InDegH.GetDat(EI.GetDstNId()) - 1)));
          	
  //         }
  //   } else {
  //   	WGraph->DelEdge(EI.GetSrcNId(), EI.GetDstNId());
  //   }
  //   // Implicitly, an edge is kept if it is the only out-edge for the source and only in-edge for destination
  //   // Which aligns with the Vespignani paper.
  // }
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());

  // OUTPUTTING 

  printf("Pruned graph:\n");
  printf("  nodes: %d\n", WGraph->GetNodes());
  printf("  edges: %d\n", WGraph->GetEdges());
  
  printf("\nSaving...");
  TSnap::SaveFltWEdgeList(WGraph, TStr::Fmt("%s-%.10f", OutFNm.CStr(), alpha), "");
  printf(" DONE\n");

  
  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}