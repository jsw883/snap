#include "stdafx.h"

template <class Surface>
void render(const PFltWNGraph& WGraph, const TIntFltPrH& CoordH, Cairo::RefPtr<Surface> surface, const double& w, const double& h, const TIntFltH& NIdvrH, const TIntFltH& NIdvwH, const TIntFltTrH& NIdvfRGBH, const double& vfAlpha, const TIntFltTrH& NIdvcRGBH, const double& vcAlpha, const double& ew, const TFltTr& ecRGB, const double& ecAlpha) {
  
  // Variables
  
  Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);
  
  TFltWNGraph::TNodeI NI;
  TFltWNGraph::TEdgeI EI;
  
  double s = std::min(w, h);
  
  // TIntV::TIter VI;
  
  // printf("Transformed\n");
  // printf("-----------\n");
  // for (VI = NIdV.BegI(); VI < NIdV.EndI(); VI++) {
  //   TFltPr& UCoord = CoordH.GetDat(VI->Val);
  //   printf("%d: (%f, %f)\n", VI->Val, (double) UCoord.Val1, (double) UCoord.Val2);
  // }
  
  // Config
  
  // Background
  
  cr->save();
  cr->set_source_rgb(1.00, 1.00, 1.00);
  cr->paint();
  cr->restore();
  
  // Edges
  
  cr->set_line_width(ew);
  cr->set_source_rgba(ecRGB.Val1, ecRGB.Val2, ecRGB.Val3, ecAlpha);
  
  for (EI = WGraph->BegEI(); EI < WGraph->EndEI(); EI++) {
    
    const TFltPr& SrcCoord = CoordH.GetDat(EI.GetSrcNId());
    const TFltPr& DstCoord = CoordH.GetDat(EI.GetDstNId());

    cr->move_to(SrcCoord.Val1, SrcCoord.Val2);
    cr->line_to(DstCoord.Val1, DstCoord.Val2);
    cr->stroke();

  }
  
  // Nodes
  
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    
    const int& NId = NI.GetId(); 
    const TFltPr& Coord = CoordH.GetDat(NId);
    const TFltTr& vfRGB = NIdvfRGBH.GetDat(NId);
    const TFltTr& vcRGB = NIdvcRGBH.GetDat(NId);
    
    cr->set_line_width(NIdvwH.GetDat(NId));
    
    cr->arc(Coord.Val1, Coord.Val2, s*NIdvrH.GetDat(NId), 0, 2.0 * PI);
    cr->set_source_rgba(vfRGB.Val1, vfRGB.Val2, vfRGB.Val3, vfAlpha);
    cr->fill_preserve();
    cr->set_source_rgba(vcRGB.Val1, vcRGB.Val2, vcRGB.Val3, vcAlpha);
    cr->stroke();
    
  }
  
  cr->show_page();

}

template <class TKey, class TVal>
void ScaleH(THash<TKey, TVal>& GenH, const double& minVal, const double& maxVal) {
  typename THash<TKey, TVal>::TIter HI;
  double minActual = GenH[0], maxActual = GenH[0];
  for (HI = GenH.BegI(); HI < GenH.EndI(); HI++) {
    const TVal& Val = HI.GetDat();
    if (Val < minActual) { minActual = Val; }
    if (Val > maxActual) { maxActual = Val; }
  }
  double scale = (maxVal - minVal) / (maxActual - minActual);
  for (HI = GenH.BegI(); HI < GenH.EndI(); HI++) {
    TVal& Val = HI.GetDat();
    Val = minVal + scale*(Val - minActual);
  }
}

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Graph drawing. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "input network (tab separated list of edges)");
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  
  // Config
  
  const int w = Env.GetIfArgPrefixInt("-w:", 1000, "output width (pixels)");
  const int h = Env.GetIfArgPrefixInt("-h:", 1000, "output height (pixels)");
  const int b = Env.GetIfArgPrefixInt("-b:", 10, "output border (pixels)");
  const bool png = Env.GetIfArgPrefixBool("--png:", true, "output PNG");
  const bool pdf = Env.GetIfArgPrefixBool("--pdf:", true, "output PDF");
  
  // Layout method
  
  const TStr layout = Env.GetIfArgPrefixStr("--layout:", "circular", "layout algorithm (random / circular / reingold)");
  
  // Reingold
  
  const int iterations = Env.GetIfArgPrefixInt("--iterations:", 1500, "number of iterations for reingold");
  const double cooling = Env.GetIfArgPrefixFlt("--cooling:", 1.5, "cooling coefficient for reingold");
  const bool shuffle = Env.GetIfArgPrefixBool("--shuffle:", false, "shuffle vertex order for circular layout (and reingold)");
  
  // Node appearance
  
  double vr = Env.GetIfArgPrefixFlt("--vr:", 0.0, "vertex radius relative to minimum axis (default: 0.1*sqrt(nodes))");
  double vw = Env.GetIfArgPrefixFlt("--vw:", 1.0, "vertex border width");
  const TStr vfHex = Env.GetIfArgPrefixStr("--vfstr:", "000000", "vertex fill (default: black)");
  const TStr vcHex = Env.GetIfArgPrefixStr("--vcstr:", "FFFFFF", "vertex border color (default: white)");
  double vfAlpha = Env.GetIfArgPrefixFlt("--vfalpha:", 1.0, "vertex fill alpha");
  double vcAlpha = Env.GetIfArgPrefixFlt("--vcalpha:", -1.0, "vertex color alpha (default: --vfalpha)");
  
  const TStr NIdvrHFNm = Env.GetIfArgPrefixStr("--vrv:", "", "vertex radius mapping relative to vertex radius (--vr)");
  const TStr NIdvwHFNm = Env.GetIfArgPrefixStr("--vwv:", "", "vertex border width mapping (overrides --vw)");
  const TStr NIdvfHexHFNm = Env.GetIfArgPrefixStr("--vfstrv:", "", "vertex fill mapping (overrides --vfstr)");
  const TStr NIdvcHexHFNm = Env.GetIfArgPrefixStr("--vcstrv:", "", "vertex border color mapping (overrides --vcstr)");
  
  // Edge appearance
  
  double ew = Env.GetIfArgPrefixFlt("--ew:", 1.0, "edge width");
  const TStr ecHex = Env.GetIfArgPrefixStr("--ecstr:", "000000", "edge color (default: black)");
  double ecAlpha = Env.GetIfArgPrefixFlt("--ecalpha:", 0.25, "edge color alpha");
  
  // Variables
  
  TStr Name;
  
  TFltTr vfRGB, vcRGB, ecRGB;
  
  ConvertHexToRGB(vfHex, vfRGB);
  ConvertHexToRGB(vcHex, vcRGB);
  ConvertHexToRGB(ecHex, ecRGB);
  
  if (vcAlpha < 0) {
    vcAlpha = vfAlpha;
  }
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  if (vr == 0.0) {
    vr = std::min(0.01, 0.1 / sqrt(WGraph->GetNodes()));
  }
  
  // Load node appearance array
  
  TIntFltH NIdvrH, NIdvwH;
  TIntV NIdV;
  TIntV::TIter VI;
  TIntStrH NIdvfHexH, NIdvcHexH;
  TIntFltTrH NIdvfRGBH, NIdvcRGBH;

  WGraph->GetNIdV(NIdV);
  
  if (!NIdvrHFNm.Empty()) {
    NIdvrH = TSnap::LoadTxtIntFltH(NIdvrHFNm);
    ScaleH(NIdvrH, vr, 2*vr);
  } else {
    for (VI = NIdV.BegI(); VI < NIdV.EndI(); VI++) {
      NIdvrH.AddDat(VI->Val, vr);
    }
  }
  
  if (!NIdvwHFNm.Empty()) {
    NIdvwH = TSnap::LoadTxtIntFltH(NIdvwHFNm);
    ScaleH(NIdvwH, vw, 2*vw);
  } else {
    for (VI = NIdV.BegI(); VI < NIdV.EndI(); VI++) {
      NIdvwH.AddDat(VI->Val, vw);
    }
  }
  
  if (!NIdvfHexHFNm.Empty()) {
    NIdvfHexH = TSnap::LoadTxtIntStrH(NIdvfHexHFNm);
  } else {
    for (VI = NIdV.BegI(); VI < NIdV.EndI(); VI++) {
      NIdvfHexH.AddDat(VI->Val, vfHex);
    }
  }
  ConvertHexToRGB(NIdvfHexH, NIdvfRGBH);

  if (!NIdvcHexHFNm.Empty()) {
    NIdvcHexH = TSnap::LoadTxtIntStrH(NIdvcHexHFNm);
  } else {
    for (VI = NIdV.BegI(); VI < NIdV.EndI(); VI++) {
      NIdvcHexH.AddDat(VI->Val, vcHex);
    }
  }
  ConvertHexToRGB(NIdvcHexH, NIdvcRGBH);

  // Layout method
  
  TIntFltPrH CoordH;
  TIntIntH NIdDegH;
  
  if (shuffle) {
    TRnd Rnd = TInt::Rnd;
    NIdV.Shuffle(Rnd);
  } else {
    TSnap::GetDegH(WGraph, NIdDegH);
    NIdDegH.SortByDat();
    NIdDegH.GetKeyV(NIdV); // Circular layout sorted by degree preferentially
  }
  
  // Layouts
  
  printf("\nComputing %s layout...", layout.CStr());
  
  if (layout == "random") {
    TSnap::RandomLayout(NIdV, CoordH);
  } else if (layout == "circular") {
    TSnap::CircularLayout(NIdV, CoordH);
  } else if (layout == "reingold") {
    TSnap::ReingoldLayout(WGraph, NIdV, CoordH, iterations, cooling);
  } else {
    IAssertR(false, "Layout must be one of \"random\", \"circular\", \"reingold\".");
  }
  TSnap::TransformLayout(CoordH, TFltPr(b, w - b), TFltPr(b, h - b), true);
  
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Saving
  
  Name = TStr::Fmt("%s.%s.CoordH", OutFNm.CStr(), layout.CStr());
  printf("\nSaving %s...", Name.CStr());
  TSnap::SaveTxt(CoordH, Name.CStr(), "Degree centrality (in / out / undirected)", "NodeId", "InDegCentr\tOutDegCentr\tDegCentr");
  printf(" DONE\n");  
  
  // Drawing

  if (pdf) {
    
    #ifdef CAIRO_HAS_PDF_SURFACE
    
      Name = TStr::Fmt("%s.%s.PDF", OutFNm.CStr(), layout.CStr());
      printf("\nDrawing %s...", Name.CStr());
      Cairo::RefPtr<Cairo::PdfSurface> surface = Cairo::PdfSurface::create(Name.CStr(), w, h);
      
      render(WGraph, CoordH, surface, w, h, NIdvrH, NIdvwH, NIdvfRGBH, vfAlpha, NIdvcRGBH, vcAlpha, ew, ecRGB, ecAlpha);
      
      printf("DONE\n");
      
    #else
      
      printf("\nCompile cairo with PDF support for graph drawing (seriously).\n");
      printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
      return 1;
      
    #endif
      
  }
  
  if (png) {
  
    #ifdef CAIRO_HAS_PNG_FUNCTIONS

      Name = TStr::Fmt("%s.%s.PNG", OutFNm.CStr(), layout.CStr());
      printf("\nDrawing %s...", Name.CStr());
      Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, w, h);
      
      render(WGraph, CoordH, surface, w, h, NIdvrH, NIdvwH, NIdvfRGBH, vfAlpha, NIdvcRGBH, vcAlpha, ew, ecRGB, ecAlpha);

      surface->write_to_png(Name.CStr());
      
      printf("DONE\n");

    #else

      printf("\nCompile cairo with PNG support for graph drawing (seriously).\n");
      printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
      return 1;

    #endif
      
  }

  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
} 