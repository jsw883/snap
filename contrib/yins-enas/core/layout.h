//#//////////////////////////////////////////////
/// Graph traversal algorithms

#ifndef core_layout
#define core_layout

// Headers (?)

#include <cmath>

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
  typename PGraph::TObj::TNodeI UI, VI;
  typename PGraph::TObj::TEdgeI EI;
  TIntFltPrH DispH;
  // TIntV NIdV;
  double xdiff, ydiff, d, coeff, temperature;
  
  TIntV::TIter NI;
  
  int N = Graph->GetNodes();
  
  // Initialize positions using circular layout method
  // Graph->GetNIdV(NIdV);
  TSnap::CircularLayout(NIdV, CoordH);
  for (UI = Graph->BegNI(); UI < Graph->EndNI(); UI++) {
    DispH.AddKey(UI.GetId());
  }
  TSnap::TransformLayout(CoordH, TFltPr(-N/2, N/2), TFltPr(-N/2, N/2), false);
  
  // printf("Circular\n");
  // printf("--------\n");
  // for (NI = NIdV.BegI(); NI < NIdV.EndI(); NI++) {
  //   TFltPr& UCoord = CoordH.GetDat(NI->Val);
  //   printf("%d: (%f, %f)\n", NI->Val, (double) UCoord.Val1, (double) UCoord.Val2);
  // }
  
  // Method
  double area = pow(N, cooling);
  double k = area / N;
  double c = pow(N, 1.5);
  
  bool connected = TSnap::IsConnected(Graph);
  
  for (int i = 0; i < iterations; i++) {  
    
    // Qudaratic cooling
    temperature = pow(1 - i / (double) iterations, cooling);
    // printf("temperature: %f\n", temperature);
    
    // Repulsive
    for (UI = Graph->BegNI(); UI < Graph->EndNI(); UI++) {
      // Variables
      int UNId = UI.GetId();
      TFltPr& UCoord = CoordH.GetDat(UNId);
      TFltPr& UDisp = DispH.GetDat(UNId);
      // Iterate
      UDisp = TFltPr(0, 0);
      for (VI = Graph->BegNI(); VI < Graph->EndNI(); VI++) {
        // Variables
        int VNId =  VI.GetId();
        TFltPr& VCoord = CoordH.GetDat(VNId);
        // Calculate
        if (VI.GetId() != UI.GetId()) {
          xdiff = VCoord.Val1 - UCoord.Val1;
          ydiff = VCoord.Val2 - UCoord.Val2;
          d = pow(xdiff, 2) + pow(ydiff, 2);
          if (!connected) {
            coeff = (c - pow(d, 1.5)) / (c*d);
          } else {
            coeff = k / d;
          }
            UDisp.Val1 -= xdiff * coeff;
            UDisp.Val2 -= ydiff * coeff;
        }
      }
    }
    
    // Attractive
    for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
      // Variables
      int SrcNId = EI.GetSrcNId();
      int DstNId = EI.GetDstNId();
      TFltPr& UCoord = CoordH.GetDat(SrcNId);
      TFltPr& VCoord = CoordH.GetDat(DstNId);
      TFltPr& UDisp = DispH.GetDat(SrcNId);
      TFltPr& VDisp = DispH.GetDat(DstNId);
      // Calculate
      xdiff = VCoord.Val1 - UCoord.Val1;
      ydiff = VCoord.Val2 - UCoord.Val2;
      coeff = sqrt(pow(xdiff, 2) + pow(ydiff, 2)) / sqrt(k); // TODO: make weighted
      UDisp.Val1 += xdiff * coeff;
      UDisp.Val2 += ydiff * coeff;
      VDisp.Val1 -= xdiff * coeff;
      VDisp.Val2 -= ydiff * coeff;
    }
    
    // Calculate
    for (UI = Graph->BegNI(); UI < Graph->EndNI(); UI++) {
      // Variables
      int UNId = UI.GetId();
      TFltPr& UCoord = CoordH.GetDat(UNId);
      TFltPr& UDisp = DispH.GetDat(UNId);
      // printf("(xdisp: %f, ydisp: %f)\n", (double) UDisp.Val1, (double) UDisp.Val2);
      // Calculate
      coeff = sqrt(pow(UDisp.Val1, 2) + pow(UDisp.Val2, 2));
      if (coeff > temperature) {
        UDisp.Val1 *= temperature / coeff;
        UDisp.Val2 *= temperature / coeff;
      }
      UCoord.Val1 += UDisp.Val1;
      UCoord.Val2 += UDisp.Val2;
    }
    // printf("\n");  
  }
  
  // printf("Reingold\n");
  // printf("--------\n");
  // for (NI = NIdV.BegI(); NI < NIdV.EndI(); NI++) {
  //   TFltPr& UCoord = CoordH.GetDat(NI->Val);
  //   printf("%d: (%f, %f)\n", NI->Val, (double) UCoord.Val1, (double) UCoord.Val2);
  // }
  
  // Transform
  // TSnap::TransformLayout(CoordH, TFltPr(0, 1), TFltPr(0, 1), true);
  
}

} // namespace TSnap {

#endif