//#//////////////////////////////////////////////
/// Graph traversal algorithms

#ifndef core_layout
#define core_layout

// Headers (?)

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
void ReingoldLayout(const PGraph& Graph, const TIntV& NIdV, TIntFltPrH& CoordH, const int& iterations = 100, const double& cooling = 3) {
  
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
  TSnap::CircularLayout(NIdV, CoordH);
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

  }
  
  // Transform
  TSnap::TransformLayout(CoordH, TFltPr(0, 1), TFltPr(0, 1), true);
  
}

} // namespace TSnap {

#endif