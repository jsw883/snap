#include "stdafx.h"

template<class T>
std::string toString(const T& value) {
  std::ostringstream oss;
  oss << value;
  return oss.str();
}

template <class T>
int numDigits(T i) {
  int d = 0;
  if (i < 0) d = 1;
  while (i) {
    i /= 10;
    d++;
  }
  return d;
}

void arrowCoords(
    const TFltPr& SrcCoord, const TFltPr& DstCoord,
    TFltPr& ACoord, TFltPr& BCoord,
    const double& arrowSize, const double& arrowAngle = PI * 45.0 / 180) {
  double angle = atan2 (
    DstCoord.Val2 - SrcCoord.Val2, DstCoord.Val1 - SrcCoord.Val1) + PI;
  ACoord.Val1 = DstCoord.Val1 + arrowSize * cos(angle - arrowAngle);
  ACoord.Val2 = DstCoord.Val2 + arrowSize * sin(angle - arrowAngle);
  BCoord.Val1 = DstCoord.Val1 + arrowSize * cos(angle + arrowAngle);
  BCoord.Val2 = DstCoord.Val2 + arrowSize * sin(angle + arrowAngle);
}

template <class Surface>
void render(
    const PFltWNGraph& WGraph, const TIntFltPrH& CoordH,
    Cairo::RefPtr<Surface> surface, const double& w, const double& h,
    const TIntFltH& NIdvrH, const TIntFltH& NIdvwH,
    const TIntFltTrH& NIdvfRGBH, const double& vfAlpha,
    const TIntFltTrH& NIdvcRGBH, const double& vcAlpha,
    const bool& label,
    const TStr& direction, const double& as, const double& ew,
    const TFltTr& ecsRGB, const TFltTr& ecdRGB,
    const double& ecAlpha) {
  
  // Variables
  
  Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);
  
  TFltWNGraph::TNodeI NI;
  TFltWNGraph::TEdgeI EI;
  
  double s = std::min(w, h);
  
  // TIntV::TIter VI;
  
  // printf("Transformed\n");
  // printf("-----------\n");
  // for (VI = NIdV.BegI(); VI < NIdV.EndI(); VI++) {a
  //   TFltPr& UCoord = CoordH.GetDat(VI->Val);
  //   printf("%d: (%f, %f)\n", VI->Val, (double) UCoord.Val1, (double) UCoord.Val2);
  // }
  
  // Config

  // const Cairo::Matrix matrix(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
  
  Cairo::RefPtr<Cairo::ToyFontFace> font = Cairo::ToyFontFace::create(
    "", Cairo::FONT_SLANT_ITALIC, Cairo::FONT_WEIGHT_BOLD);
  cr->set_font_face(font);

  Cairo::TextExtents extents;

  // Background
  
  cr->save();

  cr->set_source_rgb(1.00, 1.00, 1.00);
  cr->paint();

  cr->restore();
  
  // Edges
  
  cr->save();

  cr->set_line_cap(Cairo::LINE_CAP_BUTT);
  cr->set_line_width(ew);
  
  if (direction != "gradient" || direction != "duotone") {
    cr->set_source_rgba(ecsRGB.Val1, ecsRGB.Val2, ecsRGB.Val3, ecAlpha);
  }
  
  for (EI = WGraph->BegEI(); EI < WGraph->EndEI(); EI++) {
    
    const TFltPr& SrcCoord = CoordH.GetDat(EI.GetSrcNId());
    const TFltPr& DstCoord = CoordH.GetDat(EI.GetDstNId());

    if (direction == "gradient") {
      Cairo::RefPtr<Cairo::LinearGradient> grad = Cairo::LinearGradient::create(SrcCoord.Val1, SrcCoord.Val2, DstCoord.Val1, DstCoord.Val2);
      grad->add_color_stop_rgba(0.0, ecsRGB.Val1, ecsRGB.Val2, ecsRGB.Val3, ecAlpha);
      grad->add_color_stop_rgba(1.0 / 3, ecsRGB.Val1, ecsRGB.Val2, ecsRGB.Val3, ecAlpha);
      grad->add_color_stop_rgba(2.0 / 3, ecdRGB.Val1, ecdRGB.Val2, ecdRGB.Val3, ecAlpha);
      grad->add_color_stop_rgba(1.0, ecdRGB.Val1, ecdRGB.Val2, ecdRGB.Val3, ecAlpha);
      cr->set_source(grad);
      cr->line_to(DstCoord.Val1, DstCoord.Val2);
      cr->stroke();
    }

    cr->move_to(SrcCoord.Val1, SrcCoord.Val2);

    if (direction == "duotone") {
      cr->set_source_rgba(ecsRGB.Val1, ecsRGB.Val2, ecsRGB.Val3, ecAlpha);
      cr->line_to((SrcCoord.Val1 + DstCoord.Val1) / 2, (SrcCoord.Val2 + DstCoord.Val2) / 2);
      cr->stroke_preserve();
      cr->set_source_rgba(ecdRGB.Val1, ecdRGB.Val2, ecdRGB.Val3, ecAlpha);
      cr->line_to(DstCoord.Val1, DstCoord.Val2);
      cr->stroke();
    }

    if (direction == "arrow") {
      double xdiff = DstCoord.Val1 - SrcCoord.Val1;
      double ydiff = DstCoord.Val2 - SrcCoord.Val2;
      double ratio = (NIdvwH.GetDat(EI.GetDstNId()) / 2 + s * NIdvrH.GetDat(EI.GetDstNId())) / sqrt(xdiff * xdiff + ydiff * ydiff);
      TFltPr NewDstCoord(DstCoord.Val1 - ratio * xdiff, DstCoord.Val2 - ratio * ydiff);
      TFltPr ACoord;
      TFltPr BCoord;
      arrowCoords(SrcCoord, NewDstCoord, ACoord, BCoord, s *  as);
      cr->line_to((ACoord.Val1 + BCoord.Val1) / 2, (ACoord.Val2 + BCoord.Val2) / 2);
      cr->stroke();
      cr->move_to(NewDstCoord.Val1, NewDstCoord.Val2);
      cr->line_to(ACoord.Val1, ACoord.Val2);
      cr->line_to(BCoord.Val1, BCoord.Val2);
      cr->close_path();
      cr->fill();
    }

    if (direction != "gradient" || direction != "duotone" || direction != "arrow") {
      cr->line_to(DstCoord.Val1, DstCoord.Val2);
      cr->stroke();
    }

  }

  cr->restore();
  
  // Nodes

  double scale = 2.0 / numDigits(WGraph->GetMxNId());

  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    
    cr->save();

    const int& NId = NI.GetId();
    const TFltPr& Coord = CoordH.GetDat(NId);
    const TFltTr& vfRGB = NIdvfRGBH.GetDat(NId);
    const TFltTr& vcRGB = NIdvcRGBH.GetDat(NId);
    
    std::string text = toString(NId);

    cr->translate(Coord.Val1, Coord.Val2);
    cr->set_line_width(NIdvwH.GetDat(NId));

    cr->begin_new_path();
    cr->arc(0.0, 0.0, s * NIdvrH.GetDat(NId), 0, 2.0 * PI);

    cr->set_source_rgba(vfRGB.Val1, vfRGB.Val2, vfRGB.Val3, vfAlpha);
    cr->fill_preserve();
    cr->set_source_rgba(vcRGB.Val1, vcRGB.Val2, vcRGB.Val3, vcAlpha);
    cr->stroke();

    if (label) {
    
      cr->set_font_size(scale * s * NIdvrH.GetDat(NId));
      cr->get_text_extents(text, extents);
      cr->translate(
        - extents.x_bearing - 0.5 * extents.width,
        - extents.y_bearing - 0.5 * extents.height);
      
      cr->show_text(text);

    }

    cr->restore();

  }

  cr->show_page();

}

template <class TKey, class TVal>
void ScaleH(
    THash<TKey, TVal>& GenH, const double& minVal, const double& maxVal) {
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

void GetNIdValH(
    const TStr& FNm, TIntFltH& NIdValH, const TIntV& NIdV,
    const TFlt DefaultVal, const double scale = 3.0) {
  if (!FNm.Empty()) {
    NIdValH = TSnap::LoadTxtIntFltH(FNm);
    ScaleH(NIdValH, DefaultVal, scale*DefaultVal);
  } else {
    for (TIntV::TIter VI = NIdV.BegI(); VI < NIdV.EndI(); VI++) {
      NIdValH.AddDat(VI->Val, DefaultVal);
    }
  }
}

void GetNIdColH(
    const TStr& FNm, TIntFltTrH& NIdRGBH, const TIntV& NIdV,
    const TFltTr DefaultCol, const double& S = 1.0, const double& L = 0.5) {
  TIntStrH NIdColH;
  if (!FNm.Empty()) {
    TStr Ext = FNm.RightOfLast('.').CStr();
    if (Ext == "NIdHexH") {
      NIdColH = TSnap::LoadTxtIntStrH(FNm);
      ConvertHexToRGB(NIdColH, NIdRGBH);
    } else if (Ext == "NIdCategoryH") {
      TIntIntH::TIter HI;
      TIntIntH NIdCategoryH = TSnap::LoadTxtIntIntH(FNm);
      int MaxCategory;
      for (HI = NIdCategoryH.BegI(); HI < NIdCategoryH.EndI(); HI++) {
        const TInt& Category = HI.GetDat();
        if (Category > MaxCategory) {
          MaxCategory = Category;
        }
      }
      TIntFltTrH RGBH;
      GenHSLBasedRGB(MaxCategory, S, L, RGBH);
      for (HI = NIdCategoryH.BegI(); HI < NIdCategoryH.EndI(); HI++) {
        NIdRGBH.AddDat(HI.GetKey(), RGBH.GetDat(HI.GetDat()));
      }
    } else {
      IAssertR(false, "External color vectors must have extension NIdHexH or NIdCategoryH");
    }
  } else {
    for (TIntV::TIter VI = NIdV.BegI(); VI < NIdV.EndI(); VI++) {
      NIdRGBH.AddDat(VI->Val, DefaultCol);
    }
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
  
  const TStr layout = Env.GetIfArgPrefixStr("--layout:", "circular", "layout algorithm (random / circular / reingold / atlas / precomputed)");
  const TStr initial = Env.GetIfArgPrefixStr("--initial:", "", "initial precomputed layout (*.CoordH)");

  // Reingold
  
  const int iterations = Env.GetIfArgPrefixInt("--iterations:", 1500, "number of iterations for force directed");
  const double cooling = Env.GetIfArgPrefixFlt("--cooling:", 1.5, "cooling coefficient for force directed");
  const bool shuffle = Env.GetIfArgPrefixBool("--shuffle:", false, "shuffle vertex order for circular, reingold, and atlas layouts");
  
  // Atlas

  const double scaling = Env.GetIfArgPrefixFlt("--scaling:", 1, "repulsion scaling for force directed");
  const double gravity = Env.GetIfArgPrefixFlt("--gravity:", 1, "gravity for force directed");
  const double weights = Env.GetIfArgPrefixFlt("--weights:", 0, "weight influence exponent for force directed");
  const bool nohubs = Env.GetIfArgPrefixBool("--nohubs:", false, "dissuade hubs for force directed");
  const bool linlog = Env.GetIfArgPrefixBool("--linlog:", false, "switch linlog mode for force directed");

  // Node appearance
  
  const bool label = Env.GetIfArgPrefixBool("--label:", false, "label vertices by NId (default: F)");

  double vr = Env.GetIfArgPrefixFlt("--vr:", 0.0, "vertex radius relative to minimum axis (default: 0.1*sqrt(nodes))");
  double vw = Env.GetIfArgPrefixFlt("--vw:", 1.0, "vertex border width");
  double vrscale = Env.GetIfArgPrefixFlt("--vrscale:", 3.0, "vertex radius scale");
  double vwscale = Env.GetIfArgPrefixFlt("--vwscale:", 3.0, "vertex border scale");


  const TStr vf = Env.GetIfArgPrefixStr("--vf:", "000000", "vertex fill (default: black)");
  const TStr vc = Env.GetIfArgPrefixStr("--vc:", "FFFFFF", "vertex border color (default: white)");
  double vfAlpha = Env.GetIfArgPrefixFlt("--vfalpha:", 1.0, "vertex fill alpha");
  double vcAlpha = Env.GetIfArgPrefixFlt("--vcalpha:", -1.0, "vertex color alpha (default: --vfalpha)");
  
  const TStr NIdvrHFNm = Env.GetIfArgPrefixStr("--vrv:", "", "vertex radius mapping relative to vertex radius (overrides --vr)");
  const TStr NIdvwHFNm = Env.GetIfArgPrefixStr("--vwv:", "", "vertex border width mapping (overrides --vw)");
  const TStr NIdvfHFNm = Env.GetIfArgPrefixStr("--vfv:", "", "vertex fill mapping (overrides --vf)");
  const TStr NIdvcHFNm = Env.GetIfArgPrefixStr("--vcv:", "", "vertex border color mapping (overrides --vcstr)");
  
  const bool community = Env.GetIfArgPrefixBool("--vfcommunity:", false, "color vertices by community (overrides --vf and --vfv) (default: F)");
  const double eps = Env.GetIfArgPrefixFlt("--eps:", 1.0e-5, "minimum quality improvement threshold");
  const double moves = Env.GetIfArgPrefixFlt("--moves:", 1.0e-2, "minimum number of moves (relative)");
  const int iters = Env.GetIfArgPrefixInt("--iters:", 1.0e+4, "maximum number of iterations");

  const double S = Env.GetIfArgPrefixFlt("-s:", 1.0, "community vertex color saturation value (0.0 - 1.0)");
  const double L = Env.GetIfArgPrefixFlt("-l:", 0.5, "community vertex lightness value (0.0 - 1.0)");
  
  TFltTr vfRGB, vcRGB;
  
  ConvertHexToRGB(vf, vfRGB);
  ConvertHexToRGB(vc, vcRGB);

  if (vcAlpha < 0) vcAlpha = vfAlpha;
  
  // Edge appearance
  
  double ew = Env.GetIfArgPrefixFlt("--ew:", 1.0, "edge width");
  
  const TStr ec = Env.GetIfArgPrefixStr("--ec:", "000000", "edge color (default: black)");
  TStr ecs = Env.GetIfArgPrefixStr("--ecs:", "FF0000", "source edge color (default: red)");
  TStr ecd = Env.GetIfArgPrefixStr("--ecd:", "0000FF", "destination edge color (default: blue)");

  double ecAlpha = Env.GetIfArgPrefixFlt("--ecalpha:", 0.25, "edge color alpha");
  
  const TStr direction = Env.GetIfArgPrefixStr("--direction:", "", "how to show directionality (arrow / gradient / duotone)");
  double as = Env.GetIfArgPrefixFlt("--as:", 0.0, "arrow size relative to minimum axis (default: 0.05*sqrt(nodes))");

  TFltTr ecsRGB, ecdRGB;
  
  if (direction != "gradient" || direction != "duotone") {
    ecs = ec;
    ecd = ec;
  }

  ConvertHexToRGB(ecs, ecsRGB);
  ConvertHexToRGB(ecd, ecdRGB);

  printf("ecsRGB: (%f, %f, %f)\n", (double) ecsRGB.Val1, (double) ecsRGB.Val2, (double) ecsRGB.Val3);
  printf("ecdRGB: (%f, %f, %f)\n", (double) ecdRGB.Val1, (double) ecdRGB.Val2, (double) ecdRGB.Val3);

  // Variables
  
  TStr Name, LayoutString;
  
  // Load graph and create directed and undirected graphs (pointer to the same memory)
  
  printf("\nLoading %s...", InFNm.CStr());
  PFltWNGraph WGraph = TSnap::LoadFltWEdgeList<TWNGraph>(InFNm);
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  TSnap::printFltWGraphSummary(WGraph, true, "\nWGraph\n------");
  
  // Node appearance
  
  if (vr == 0.0) vr = std::min(0.01, 0.1 / sqrt(WGraph->GetNodes()));
  if (as == 0.0) as = std::min(0.01, 0.05 / sqrt(WGraph->GetNodes()));
  
  TIntFltH NIdvrH, NIdvwH;
  TIntV NIdV;
  TIntFltTrH NIdvfRGBH, NIdvcRGBH;

  WGraph->GetNIdV(NIdV);

  GetNIdValH(NIdvrHFNm, NIdvrH, NIdV, vr, vrscale);
  GetNIdValH(NIdvwHFNm, NIdvwH, NIdV, vw, vwscale);

  if (community) {
    // Compute community
    TIntIntVH NIdCmtyVH;
    int NCmty;
    double LouvainQ;
    LouvainQ = TSnap::LouvainMethod<TSnap::ModularityCommunity<TFlt>, TFlt>(WGraph, NIdCmtyVH, NCmty, edUnDirected, eps, moves, iters);
    // Get colors
    TIntFltTrH RGBH;
    GenHSLBasedRGB(NCmty, S, L, RGBH);
    // NId
    TIntIntVH::TIter HI;
    for (HI = NIdCmtyVH.BegI(); HI < NIdCmtyVH.EndI(); HI++) {
      NIdvfRGBH.AddDat(HI.GetKey(), RGBH.GetDat(HI.GetDat().Last()));
    }
    printf("\nCommunities computed (quality: %f)\n", LouvainQ);
  } else {
    GetNIdColH(NIdvfHFNm, NIdvfRGBH, NIdV, vfRGB, S, L);
  }

  GetNIdColH(NIdvcHFNm, NIdvcRGBH, NIdV, vcRGB, S, L);

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
  
  printf("\nComputing %s layout...", layout.CStr());
  

  if (initial == "random") {

    TSnap::RandomLayout(NIdV, CoordH);
    printf("\nRANDOM\n\n");

  } else if (initial == "circular") {

    TSnap::CircularLayout(NIdV, CoordH);
    printf("\nCIRCULAR\n\n");

  } else if (!initial.Empty()) {

    CoordH = TSnap::LoadTxtIntFltPrH(initial);
    printf("\nPRECOMPUTED\n\n");

  } else {

    TSnap::RandomLayout(NIdV, CoordH);

  }

  if (layout == "precomputed") {

    if (initial.Empty()) {
      IAssertR(false, "Precomputed must be specified.");
    }
    LayoutString = TStr::Fmt("");

  } else if (layout == "random") {

    TSnap::RandomLayout(NIdV, CoordH);
    LayoutString = TStr::Fmt("");

  } else if (layout == "circular") {

    TSnap::CircularLayout(NIdV, CoordH);
    LayoutString = TStr::Fmt("");

  } else if (layout == "reingold") {

    TSnap::ReingoldLayout(WGraph, NIdV, CoordH, iterations, cooling);
    LayoutString = TStr::Fmt("C%3.2e", cooling);


  } else if (layout == "atlas") {

    TSnap::AtlasLayout(WGraph, NIdV, CoordH, iterations, cooling, scaling, gravity, weights, nohubs, linlog);
    LayoutString = TStr::Fmt("C%3.2e.S%3.2e.G%3.2e.W%3.2e.H%d.L%d", cooling, scaling, gravity, weights, nohubs, linlog);

  } else {

    IAssertR(false, "Layout must be one of \"random\", \"circular\", \"reingold\", \"atlas\", or \"precomputed\".");

  }
  TSnap::TransformLayout(CoordH, TFltPr(b, w - b), TFltPr(b, h - b), true);
  
  printf(" DONE (time elapsed: %s (%s))\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  
  // Saving
  
  if (layout != "precomputed") {

    Name = TStr::Fmt("%s.%s.%s.CoordH", OutFNm.CStr(), LayoutString.CStr(), layout.CStr());
    printf("\nSaving %s...", Name.CStr());
    TSnap::SaveTxt(CoordH, Name.CStr(), TStr::Fmt("Layout coordinates for %s method", layout.CStr()), "NodeId", "x\ty");
    printf(" DONE\n");  
    
  }

  // Drawing

  if (pdf) {
    
    #ifdef CAIRO_HAS_PDF_SURFACE
    
      Name = TStr::Fmt("%s.%s.%s.%dx%d.PDF", OutFNm.CStr(), LayoutString.CStr(), layout.CStr(), w, h);
      printf("\nDrawing %s...", Name.CStr());
      Cairo::RefPtr<Cairo::PdfSurface> surface = Cairo::PdfSurface::create(Name.CStr(), w, h);
      
      render(WGraph, CoordH, surface, w, h, NIdvrH, NIdvwH, NIdvfRGBH, vfAlpha, NIdvcRGBH, vcAlpha, label, direction, as, ew, ecsRGB, ecdRGB, ecAlpha);
      
      printf("DONE\n");
      
    #else
      
      printf("\nCompile cairo with PDF support for graph drawing (seriously).\n");
      printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
      return 1;
      
    #endif
      
  }
  
  if (png) {
  
    #ifdef CAIRO_HAS_PNG_FUNCTIONS

      Name = TStr::Fmt("%s.%s.%s.%dx%d.PNG", OutFNm.CStr(), LayoutString.CStr(), layout.CStr(), w, h);
      printf("\nDrawing %s...", Name.CStr());
      Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, w, h);
      
      render(WGraph, CoordH, surface, w, h, NIdvrH, NIdvwH, NIdvfRGBH, vfAlpha, NIdvcRGBH, vcAlpha, label, direction, as, ew, ecsRGB, ecdRGB, ecAlpha);

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
