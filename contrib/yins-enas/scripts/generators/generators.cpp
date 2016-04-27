#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Graph generators. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "./graph", "output prefix (filename extensions added)");
  const int nodes = Env.GetIfArgPrefixInt("-n:", 1000, "nodes (default: 1000)");
  const int k = Env.GetIfArgPrefixInt("-k:", 1, "circle connection degree (default: 1)");
  const double p = Env.GetIfArgPrefixFlt("-p:", 0.5, "Erdos Renyi edge probability (default: 0.5)");
  const double exponent = Env.GetIfArgPrefixFlt("--exp:", 2.5, "Power law exponent (default: 2.5)");
  const bool isDirected = Env.GetIfArgPrefixBool("--dir:", false, "Allow directed edges (default: false)");
  
  TStr Name;
  
  PUNGraph Graph;
  
  Graph = TSnap::GenLine<PUNGraph>(nodes, isDirected);
  Name = TStr::Fmt("%s-%s-%d.snap", OutFNm.CStr(), "line", nodes);
  printf("\nSaving %s...", Name.CStr());
  TSnap::SaveEdgeList(Graph, Name, "");
  printf(" DONE\n");

  int size = round(sqrt(nodes));
  Graph = TSnap::GenGrid<PUNGraph>(size, size, isDirected);
  Name = TStr::Fmt("%s-%s-%d-%d.snap", OutFNm.CStr(), "grid", size, size);
  printf("\nSaving %s...", Name.CStr());
  TSnap::SaveEdgeList(Graph, TStr::Fmt("%s-%s-%d-%d.snap", OutFNm.CStr(), "grid", size, size), "");
  printf(" DONE\n");

  Graph = TSnap::GenCircle<PUNGraph>(nodes, k, isDirected);
  Name = TStr::Fmt("%s-%s-%d-%d.snap", OutFNm.CStr(), "circle", nodes, k);
  printf("\nSaving %s...", Name.CStr());
  TSnap::SaveEdgeList(Graph, Name, "");
  printf(" DONE\n");

  Graph = TSnap::GenFull<PUNGraph>(nodes);
  Name = TStr::Fmt("%s-%s-%d.snap", OutFNm.CStr(), "complete", nodes);
  printf("\nSaving %s...", Name.CStr());
  TSnap::SaveEdgeList(Graph, Name, "");
  printf(" DONE\n");

  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}