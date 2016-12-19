#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
   
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Node centrality. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges with edge weights)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  const TStr OutDir = OutFNm.LeftOfLast('/');
  const TStr BseFNm = OutFNm.RightOfLast('/');
  const double eps = Env.GetIfArgPrefixFlt("--eps:", 1.0e-5, "minimum quality improvement threshold");
  const double moves = Env.GetIfArgPrefixFlt("--moves:", 1.0e-2, "minimum number of moves (relative)");
  const int iters = Env.GetIfArgPrefixInt("--iters:", 1.0e+4, "maximum number of iterations");
  const bool subgraphs = Env.GetIfArgPrefixBool("--subgraphs:", false, "output community subgraphs: T / F");
  
  const double S = Env.GetIfArgPrefixFlt("-s:", 1.0, "community vertex color saturation value (0.0 - 1.0)");
  const double L = Env.GetIfArgPrefixFlt("-l:", 0.5, "community vertex lightness value (0.0 - 1.0)");

  // Load graph and create directed and undirected graphs (pointer to the same memory)
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  // Declare variables
  TIntIntVH NIdCmtyVH, CmtyNIdVH;
  int NCmty;
  double LouvainQ;

  PFltWNGraph WSubGraph;

  TStr Name;
  TStr method = "louvain";

  // COMMUNITY
  
  // Louvain method (modularity objective)
  
  printf("\nLouvain method (weighted)...");
  LouvainQ = TSnap::LouvainMethod<TSnap::ModularityCommunity<TFlt>, TFlt>(WGraph, NIdCmtyVH, NCmty, edUnDirected, eps, moves, iters);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  printf("  quality: %f\n", LouvainQ);
  
  // Get colors
  TIntFltTrH RGBH, NIdRGBH;
  TIntStrH NIdHexH;
  GenHSLBasedRGB(NCmty, S, L, RGBH);
  // NId
  TIntIntVH::TIter HI;
  for (HI = NIdCmtyVH.BegI(); HI < NIdCmtyVH.EndI(); HI++) {
    NIdRGBH.AddDat(HI.GetKey(), RGBH.GetDat(HI.GetDat().Last()));
  }

  // OUTPUTTING

  TSnap::CmtyHierarchySummary(NIdCmtyVH, 1, -1, "\nLouvain hierarchy\n-----------------");
  
  Name = TStr::Fmt("%s.%s.NIdCmtyVH", OutFNm.CStr(), method.CStr());
  printf("Saving %s...", Name.CStr());
  TSnap::SaveTxt(NIdCmtyVH, Name.CStr(), "Louvain modularity community hierarchy (weighted)", "NodeId", "CmtyV");
  printf(" DONE\n");
  
  Name = TStr::Fmt("%s.%s.NIdRGBH", OutFNm.CStr(), method.CStr());
  printf("Saving %s...", Name.CStr());
  TSnap::SaveTxt(NIdRGBH, Name.CStr(), "Louvain modularity final community hierarchy coloring (weighted)", "NodeId", "Col");
  printf(" DONE\n");
  
  ConvertRGBToHex(NIdRGBH, NIdHexH);
  
  Name = TStr::Fmt("%s.%s.NIdHexH", OutFNm.CStr(), method.CStr());
  printf("Saving %s...", Name.CStr());
  TSnap::SaveTxt(NIdHexH, Name.CStr(), "Louvain modularity final community hierarchy coloring (weighted)", "NodeId", "Col");
  printf(" DONE\n");
  
  if (subgraphs) {

    printf("0\n");

    // Separate nodes into communities
    // for (int i = 0; i < NCmty; i++) {
    //   CmtyNIdVH.AddKey(NCmty);
    // }
    for (HI = NIdCmtyVH.BegI(); HI < NIdCmtyVH.EndI(); HI++) {
      const int& CmtyId = HI.GetDat().Last();
      const int& NId = HI.GetKey();
      if (!CmtyNIdVH.IsKey(CmtyId)) {
        CmtyNIdVH.AddDat(CmtyId, TIntV::GetV(NId));
      } else {
        CmtyNIdVH.GetDat(CmtyId).Add(NId);
      }
    }

    printf("1\n");

    for (HI = CmtyNIdVH.BegI(); HI < CmtyNIdVH.EndI(); HI++) {
      const int& CmtyId = HI.GetKey();
      const TIntV& NIdV = HI.GetDat();
      WSubGraph = TSnap::GetSubGraph(WGraph, NIdV);
      TSnap::SaveFltWEdgeList(WSubGraph, TStr::Fmt("%s/%04d.snap", OutDir.CStr(), CmtyId), TStr::Fmt("Wcommunity: %04d", CmtyId));
    }

    printf("2\n");

  }

  Catch
  
  printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
}