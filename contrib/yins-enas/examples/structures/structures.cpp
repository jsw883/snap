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
  int CnComId;

  // STRUCTURES (computations)
  
  TSnap::PercolateGraph<PNGraph>(Graph, GraphCopy, WCnComV, p);
  
  printf("Clusters:\n");
  for (WCnComI = WCnComV.BegI(), CnComId = 0; CnComId < 10; WCnComI++, CnComId++) {
    printf("%d: %d\n", CnComId, WCnComI->Len());
  }
  printf("number of clusters: %d\n", WCnComV.Len());
  printf("nodes: %d\n", Graph->GetNodes());
  printf("edges: %d\n", Graph->GetEdges());
  
  // TODO: compute NF for each connected component and aggregate the results
  // TODO: multiple iterations and average results
  // TODO: iterate over several values of percolation probability
  
  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}