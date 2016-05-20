// Headers (?)

#include "stdafx.h"
#include "layout.h"

//#//////////////////////////////////////////////
/// Transformations

/// Transforms bounding box, optionally inferring input range automatically
void TSnap::TransformLayout(TIntFltPrH& CoordH, const TFltPr& xrange, const TFltPr& yrange, const bool& infer_source_range) {
  // Variables
  TIntFltPrH::TIter HI;
  // Source range
  TFltPr source_xrange;
  TFltPr source_yrange;
  if (infer_source_range) {
    source_xrange = TFltPr(CoordH[0].Val1, CoordH[0].Val1);
    source_yrange = TFltPr(CoordH[0].Val2, CoordH[0].Val2);
    for (HI = CoordH.BegI(); HI < CoordH.EndI(); HI++) {
      const TFltPr& Coord = HI.GetDat();
      if (Coord.Val1 < source_xrange.Val1) { source_xrange.Val1 = Coord.Val1; }
      if (Coord.Val1 > source_xrange.Val2) { source_xrange.Val2 = Coord.Val1; }
      if (Coord.Val2 < source_yrange.Val1) { source_yrange.Val1 = Coord.Val2; }
      if (Coord.Val2 > source_yrange.Val2) { source_yrange.Val2 = Coord.Val2; }
    }
  } else {
    source_xrange = TFltPr(0, 1);
    source_yrange = TFltPr(0, 1);
  }
  // Transform source range to destination range
  double xscale = (xrange.Val2 - xrange.Val1) / (source_xrange.Val2 - source_xrange.Val1);
  double yscale = (yrange.Val2 - yrange.Val1) / (source_yrange.Val2 - source_yrange.Val1);
  for (HI = CoordH.BegI(); HI < CoordH.EndI(); HI++) {
    TFltPr& Coord = HI.GetDat();
    Coord.Val1 = xrange.Val1 + xscale*(Coord.Val1 - source_xrange.Val1);
    Coord.Val2 = yrange.Val1 + yscale*(Coord.Val2 - source_yrange.Val1);
  }
}

//#//////////////////////////////////////////////
/// Layout algorithms

/// Random layout method
void TSnap::RandomLayout(const TIntV& NIdV, TIntFltPrH& CoordH, TRnd& Rnd) {
  //Variables
  TIntV::TIter NI;
  // Method
  CoordH.Clr();
  for (NI = NIdV.BegI(); NI < NIdV.EndI(); NI++) {
    CoordH.AddDat(NI->Val, TFltPr(Rnd.GetUniDev(), Rnd.GetUniDev()));
  }
}

/// Circular layout method
void TSnap::CircularLayout(const TIntV& NIdV, TIntFltPrH& CoordH) {
  // Variables
  TIntV::TIter NI;
  double n = NIdV.Len();
  double i = 0;
  // Method
  CoordH.Clr();
  for (NI = NIdV.BegI(); NI < NIdV.EndI(); NI++, i++) {
    double x = 0.5 + 0.5*cos(2 * M_PI * (i / n));
    double y = 0.5 + 0.5*sin(2 * M_PI * (i / n));
    CoordH.AddDat(NI->Val, TFltPr(x, y));
  }
}
