//#//////////////////////////////////////////////
/// Graph traversal algorithms

#ifndef core_layout
#define core_layout

// Headers (?)

#include "utils.h"

#include <cmath>
#include <ctime>

//#//////////////////////////////////////////////
/// Transformations

namespace TSnap {

/// Transforms bounding box, optionally inferring input range automatically
void TransformLayout(TIntFltPrH& CoordH, const TFltPr& xrange, const TFltPr& yrange, const bool& infer_source_range = false);

} // namespace TSnap {

//#//////////////////////////////////////////////
/// Layout algorithms

namespace TSnap {

/// Random layout method
void RandomLayout(const TIntV& NIdV, TIntFltPrH& CoordH, TRnd& Rnd = TFlt::Rnd);
/// Circular layout method
void CircularLayout(const TIntV& NIdV, TIntFltPrH& CoordH);

/// Fruchterman Reingold layout method
template<class PGraph>
void ReingoldLayout(const PGraph& Graph, const TIntV& NIdV, TIntFltPrH& CoordH, const int& iterations = 100, const double& cooling = 3, const bool& verbose = true) {
  
  // Variables
  // typename PGraph::TObj::TNodeI UI, VI;
  typename PGraph::TObj::TEdgeI EI;
  TIntFltPrH DispH;
  // TIntV NIdV;
  double xdiff, ydiff, d, coeff, temperature;
  
  // TFltPrV DispV, CoordV;
  
  TIntV::TIter NI;
  int U, V;
  
  int N = Graph->GetNodes();
  
  // Initialize positions using circular layout method
  // TSnap::CircularLayout(NIdV, CoordH);
  for (U = 0; U < NIdV.Len(); U++) {
    DispH.AddKey(NIdV[U]);
  }
  TSnap::TransformLayout(CoordH, TFltPr(-N/2, N/2), TFltPr(-N/2, N/2), false);
  
  // Method
  double area = pow(N, cooling);
  double k = area / N;
  double sqrtk = sqrt(k);
  double c = pow(N, 1.5);
  
  bool connected = TSnap::IsConnected(Graph);
  
  TExeTm ExeTm;
  Progress progress(ExeTm, iterations, 5, "Computing Fruchterman Reingold");
  if (verbose) {
    progress.start();
  }
  for (int i = 0; i < iterations; i++) {  
    
    // Qudaratic cooling
    temperature = pow(0.5*sqrt(N)*(1 - i / (double) iterations), cooling);
    
    // Repulsive
    for (U = 0; U < NIdV.Len(); U++) {
      for (V = U, V++; V < NIdV.Len(); V++) {
        // Calculate
        if (NIdV[U] != NIdV[V]) {
          xdiff = CoordH[V].Val1 - CoordH[U].Val1;
          ydiff = CoordH[V].Val2 - CoordH[U].Val2;
          d = xdiff*xdiff + ydiff*ydiff;
          if (!connected) {
            coeff = (c - d*sqrt(d)) / (c*d);
          } else {
            coeff = k / d;
          }
          DispH[U].Val1 -= xdiff * coeff;
          DispH[U].Val2 -= ydiff * coeff;
          DispH[V].Val1 += xdiff * coeff;
          DispH[V].Val2 += ydiff * coeff;
        }
      }
    }
    
    // Attractive
    for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
      // Variables
      int SrcNId = EI.GetSrcNId();
      int DstNId = EI.GetDstNId();
      // Calculate
      xdiff = CoordH.GetDat(DstNId).Val1 - CoordH.GetDat(SrcNId).Val1;
      ydiff = CoordH.GetDat(DstNId).Val2 - CoordH.GetDat(SrcNId).Val2;
      // TODO: make weighted
      coeff = sqrt(xdiff*xdiff + ydiff*ydiff) / sqrtk;
      DispH.GetDat(SrcNId).Val1 += xdiff * coeff;
      DispH.GetDat(SrcNId).Val2 += ydiff * coeff;
      DispH.GetDat(DstNId).Val1 -= xdiff * coeff;
      DispH.GetDat(DstNId).Val2 -= ydiff * coeff;
    }
    
    // Calculate
    for (U = 0; U < NIdV.Len(); U++) {
      // Calculate
      coeff = sqrt(DispH[U].Val1*DispH[U].Val1 + DispH[U].Val2*DispH[U].Val2);
      if (coeff > temperature) {
        DispH[U].Val1 *= temperature / coeff;
        DispH[U].Val2 *= temperature / coeff;
      }
      CoordH[U].Val1 += DispH[U].Val1;
      CoordH[U].Val2 += DispH[U].Val2;
      // Reset 
      DispH[U].Val1 = 0;
      DispH[U].Val2 = 0;
    }
    
    if (verbose) { progress++; }
    
  }
  
  // Transform
  TSnap::TransformLayout(CoordH, TFltPr(0, 1), TFltPr(0, 1), true);
  
}

/// Fruchterman Reingold layout method
template<class PGraph>
void AtlasLayout(const PGraph& Graph, const TIntV& NIdV, TIntFltPrH& CoordH, const int& iterations = 100, const double& cooling = 3, const double& scaling = 1, const double& gravity = 1, const double& weights = 0, const bool& nohubs = false, const bool& linlog = false, const bool& verbose = true) {
  
  // Variables
  // typename PGraph::TObj::TNodeI UI, VI;
  typename PGraph::TObj::TEdgeI EI;
  TIntFltPrH DispH;
  TIntFltH DegH;
  // TIntV NIdV;
  double xdiff, ydiff, d, coeff, temperature;
  
  // TFltPrV DispV, CoordV;
  
  TIntV::TIter NI;
  int U, V;
  
  int N = Graph->GetNodes();
  
  // Initialize positions using circular layout method
  // TSnap::RandomLayout(NIdV, CoordH);
  for (U = 0; U < NIdV.Len(); U++) {
    DispH.AddKey(NIdV[U]);
    DegH.AddDat(NIdV[U], Graph->GetNI(NIdV[U]).GetDeg());
  }
  TSnap::TransformLayout(CoordH, TFltPr(-N/2, N/2), TFltPr(-N/2, N/2), false);
  
  // Method
  double area = pow(N, cooling);
  double k = area / N;
  double x = 0, y = 0;

  TExeTm ExeTm;
  Progress progress(ExeTm, iterations, 5, "Computing Force Atlas (not optimized)");
  if (verbose) {
    progress.start();
  }
  for (int i = 0; i < iterations; i++) {  

    // Qudaratic cooling
    temperature = pow(0.5*sqrt(N)*(1 - i / (double) iterations), cooling);
    
    // Repulsive
    for (U = 0; U < NIdV.Len(); U++) {
      for (V = U, V++; V < NIdV.Len(); V++) {
        // Calculate
        if (NIdV[U] != NIdV[V]) {
          xdiff = CoordH[V].Val1 - CoordH[U].Val1;
          ydiff = CoordH[V].Val2 - CoordH[U].Val2;
          d = xdiff*xdiff + ydiff*ydiff;
          coeff = scaling * (DegH[U] + 1) * (DegH[V] + 1) / d;
          DispH[U].Val1 -= xdiff * coeff;
          DispH[U].Val2 -= ydiff * coeff;
          DispH[V].Val1 += xdiff * coeff;
          DispH[V].Val2 += ydiff * coeff;
        }
      }
    }
    
    // Center of Gravity
    // x = 0;
    // y = 0;
    // for (U = 0; U < NIdV.Len(); U++) {
    //   x += CoordH[V].Val1;
    //   y += CoordH[V].Val2;
    // }
    // x /= N;
    // y /= N;

    // Gravity
    for (U = 0; U < NIdV.Len(); U++) {
      xdiff = CoordH[U].Val1 - x;
      ydiff = CoordH[U].Val2 - y;
      d = sqrt(xdiff*xdiff + ydiff*ydiff);
      coeff = gravity * (Graph->GetNI(NIdV[U]).GetDeg() + 1) / d;
      DispH[U].Val1 -= xdiff * coeff;
      DispH[U].Val2 -= ydiff * coeff;
    }

    // Attractive
    for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
      // Variables
      int SrcNId = EI.GetSrcNId();
      int DstNId = EI.GetDstNId();
      double W = pow(EI.GetW(), weights);
      // Calculate
      xdiff = CoordH.GetDat(DstNId).Val1 - CoordH.GetDat(SrcNId).Val1;
      ydiff = CoordH.GetDat(DstNId).Val2 - CoordH.GetDat(SrcNId).Val2;
      d = sqrt(xdiff*xdiff + ydiff*ydiff);
      if (linlog) {
        coeff = log(1 + d) / (d*k);
      } else {
        coeff = 1 / k;
      }
      if (nohubs) {
        coeff /= DegH.GetDat(SrcNId) + 1;
      }
      DispH.GetDat(SrcNId).Val1 += xdiff * coeff * W;
      DispH.GetDat(SrcNId).Val2 += ydiff * coeff * W;
      DispH.GetDat(DstNId).Val1 -= xdiff * coeff * W;
      DispH.GetDat(DstNId).Val2 -= ydiff * coeff * W;
    }
    
    // Calculate
    for (U = 0; U < NIdV.Len(); U++) {
      // Calculate
      coeff = sqrt(DispH[U].Val1*DispH[U].Val1 + DispH[U].Val2*DispH[U].Val2);
      if (coeff > temperature) {
        DispH[U].Val1 *= temperature / coeff;
        DispH[U].Val2 *= temperature / coeff;
      }
      CoordH[U].Val1 += DispH[U].Val1;
      CoordH[U].Val2 += DispH[U].Val2;
      // Reset 
      DispH[U].Val1 = 0;
      DispH[U].Val2 = 0;
    }
    
    if (verbose) { progress++; }
    
  }
  
  // Transform
  TSnap::TransformLayout(CoordH, TFltPr(0, 1), TFltPr(0, 1), true);

}

} // namespace TSnap {

#endif
