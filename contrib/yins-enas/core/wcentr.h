#ifndef core_wcentr
#define core_wcentr

// Headers (?)

#include "enums.h"

namespace TSnap {

/// Computes directed first degree centrality
template <class TEdgeW, template <class> class TGraph > void GetWDirDegreeCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WDegCentrH, const TEdgeDir& dir, const double& alpha = 1.0);
/// Computes first in degree centrality
template <class TEdgeW, template <class> class TGraph > void GetWInDegreeCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WInDegCentrH, const double& alpha = 1.0);
/// Computes first out degree centrality
template <class TEdgeW, template <class> class TGraph > void GetWOutDegreeCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WOutDegCentrH, const double& alpha = 1.0);
/// Computes first degree centrality
template <class TEdgeW, template <class> class TGraph > void GetWDegreeCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WDegCentrH, const double& alpha = 1.0);

/// Computes first degree centralities (in / out / undirected)
template <class TEdgeW, template <class> class TGraph > void GetWDegreeCentrVH(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltVH& WDegCentrVH, const double& alpha = 1.0);

/// Directed eigenvector centrality
template <class TEdgeW, template <class> class TGraph > double GetWDirEigenVectorCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WEigCentrH, const TEdgeDir& dir, const double& eps = 1e-4, const int& MaxIter = 100); 
/// Directed eigenvector centrality
template <class TEdgeW, template <class> class TGraph > double GetWInEigenVectorCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WInEigCentrH, const double& eps = 1e-4, const int& MaxIter = 100);
/// Directed eigenvector centrality
template <class TEdgeW, template <class> class TGraph > double GetWOutEigenVectorCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WOutEigCentrH, const double& eps = 1e-4, const int& MaxIter = 100);
/// Directed eigenvector centrality
template <class TEdgeW, template <class> class TGraph > double GetWEigenVectorCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WEigCentrH, const double& eps = 1e-4, const int& MaxIter = 100);

/// Eigenvector centralities (standard power method)
template <class TEdgeW, template <class> class TGraph > TFltV GetWEigenVectorCentrVH(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltVH& WEigCentrVH, const double& eps = 1e-4, const int& MaxIter = 100);

/// PageRank centrality (for more info see: http://en.wikipedia.org/wiki/PageRank)
template <class TEdgeW, template <class> class TGraph > double GetWPageRank(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WPRankH, const double& c=0.85, const double& eps=1e-4, const int& MaxIter=100);


/// Computes first in degree centralities
template <class TEdgeW, template <class> class TGraph >
void GetWDirDegreeCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WDegCentrH, const TEdgeDir& dir, const double& alpha) {
  typename TGraph<TEdgeW>::TNodeI NI;
  WDegCentrH.Gen(WGraph->GetNodes());
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    TEdgeW deg = 0;
    double centr = 0.0; // initialize to avoid errors
    if (WGraph->GetNodes() > 1) {
      switch(dir) {
        case edInDirected: deg = NI.GetWInDeg(); break;
        case edOutDirected: deg = NI.GetWOutDeg(); break;
        case edUnDirected: deg = NI.GetWDeg(); break;
      }
      centr = pow(double(deg), 1 - alpha) * pow(double(deg), alpha);
    } else {
      centr = 0.0;
    }
    WDegCentrH.AddDat(NI.GetId(), centr);
  }
}
template <class TEdgeW, template <class> class TGraph >
void GetWInDegreeCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WInDegCentrH, const double& alpha) {
  GetWDirDegreeCentr(WGraph, WInDegCentrH, edInDirected, alpha);
}
template <class TEdgeW, template <class> class TGraph >
void GetWOutDegreeCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WOutDegCentrH, const double& alpha) {
  GetWDirDegreeCentr(WGraph, WOutDegCentrH, edOutDirected, alpha);
}
template <class TEdgeW, template <class> class TGraph >
void GetWDegreeCentr(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WDegCentrH, const double& alpha) {
  GetWDirDegreeCentr(WGraph, WDegCentrH, edUnDirected, alpha);
}

template <class TEdgeW, template <class> class TGraph >
void GetWDegreeCentrVH(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltVH& WDegCentrVH, const double& alpha) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TFltV WDegCentrV;
  TIntFltH WInDegCentrH, WOutDegCentrH, WDegCentrH;
  WDegCentrVH.Gen(WGraph->GetNodes());
  GetWInDegreeCentr(WGraph, WInDegCentrH, alpha);
  GetWOutDegreeCentr(WGraph, WOutDegCentrH, alpha);
  GetWDegreeCentr(WGraph, WDegCentrH, alpha);
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    WDegCentrV.Clr();
    WDegCentrV.Add(WInDegCentrH.GetDat(NI.GetId()));
    WDegCentrV.Add(WOutDegCentrH.GetDat(NI.GetId()));
    WDegCentrV.Add(WDegCentrH.GetDat(NI.GetId()));
    WDegCentrVH.AddDat(NI.GetId(), WDegCentrV);
  }
}

// Standard power method for computing leading eigenvector and eigenvalue
template <class TEdgeW, template <class> class TGraph >
double GetWDirEigenVectorCentr(const TPt<TGraph<TEdgeW> >& Graph, TIntFltH& WEigCentrH, const TEdgeDir& dir, const double& eps, const int& MaxIter) {
  typename TGraph<TEdgeW>::TNodeI NI;
  const int NNodes = Graph->GetNodes();
  int deg = 0;
  double eig = 0.0, diff = 0.0;
  WEigCentrH.Gen(NNodes);
  // initialize vector values
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    WEigCentrH.AddDat(NI.GetId(), 1.0/NNodes); // uniform distribution initially
    IAssert(NI.GetId() == WEigCentrH.GetKey(WEigCentrH.Len() - 1));
  }
  TFltV TmpV(NNodes);
  for (int iter = 0; iter < MaxIter; iter++) {
    // add neighbor values
    int j = 0;
    for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++, j++) {
      switch(dir) {
        case edInDirected: deg = NI.GetInDeg(); break;
        case edOutDirected: deg = NI.GetOutDeg(); break;
        case edUnDirected: deg = NI.GetDeg(); break;
      }
      TmpV[j] = 0;
      for (int e = 0; e < deg; e++) { // this is now directed in / out / undirected centrality
        switch(dir) {
          case edInDirected: TmpV[j] += double(NI.GetInEW(e)) * WEigCentrH.GetDat(NI.GetInNId(e)); break;
          case edOutDirected: TmpV[j] += double(NI.GetOutEW(e)) * WEigCentrH.GetDat(NI.GetOutNId(e)); break;
          case edUnDirected: TmpV[j] += double(NI.GetNbrEW(e)) * WEigCentrH.GetDat(NI.GetNbrNId(e)); break;
        }
      }
    }
    // normalise entire vector by sum of squares rather than each row normalised already (which is PageRank)
    eig = 0;
    for (int i = 0; i < TmpV.Len(); i++) {
      eig += pow(TmpV[i], 2.0);
    }
    eig = sqrt(eig);
    for (int i = 0; i < TmpV.Len(); i++) {
      TmpV[i] /= eig;
    }
    // compute difference
    diff = 0.0;
    j = 0;
    for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++, j++) {
      diff += fabs(WEigCentrH.GetDat(NI.GetId()) - TmpV[j]);
    }
    // set new values
    j = 0;
    for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++, j++) {
      WEigCentrH.AddDat(NI.GetId(), TmpV[j]);
    }
    if (diff < eps) { break; }
  }
  return(diff); // didn't converge return(-1) (?)
}
template <class TEdgeW, template <class> class TGraph >
double GetWInEigenVectorCentr(const TPt<TGraph<TEdgeW> >& Graph, TIntFltH& InWEigCentrH, const double& eps, const int& MaxIter) {
  return GetWDirEigenVectorCentr(Graph, InWEigCentrH, edInDirected, eps, MaxIter);
}
template <class TEdgeW, template <class> class TGraph >
double GetWOutEigenVectorCentr(const TPt<TGraph<TEdgeW> >& Graph, TIntFltH& OutWEigCentrH, const double& eps, const int& MaxIter) {
  return GetWDirEigenVectorCentr(Graph, OutWEigCentrH, edOutDirected, eps, MaxIter);
}
template <class TEdgeW, template <class> class TGraph >
double GetWEigenVectorCentr(const TPt<TGraph<TEdgeW> >& Graph, TIntFltH& WEigCentrH, const double& eps, const int& MaxIter) {
  return GetWDirEigenVectorCentr(Graph, WEigCentrH, edUnDirected, eps, MaxIter);
}

template <class TEdgeW, template <class> class TGraph >
TFltV GetWEigenVectorCentrVH(const TPt<TGraph<TEdgeW> >& Graph, TIntFltVH& WEigCentrVH, const double& eps, const int& MaxIter) {
  typename TGraph<TEdgeW>::TNodeI NI;
  TIntFltH WInEigCentrH, WOutEigCentrH, WEigCentrH;
  TFltV DiffV, WEigV;
  DiffV.Clr();
  DiffV.Add(GetWInEigenVectorCentr(Graph, WInEigCentrH, eps, MaxIter));
  DiffV.Add(GetWOutEigenVectorCentr(Graph, WOutEigCentrH, eps, MaxIter));
  DiffV.Add(GetWEigenVectorCentr(Graph, WEigCentrH, eps, MaxIter));
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    WEigV.Clr();
    WEigV.Add(WInEigCentrH.GetDat(NI.GetId()));
    WEigV.Add(WOutEigCentrH.GetDat(NI.GetId()));
    WEigV.Add(WEigCentrH.GetDat(NI.GetId()));
    WEigCentrVH.AddDat(NI.GetId(), WEigV);
  }
  return(DiffV);
}

// Berkhin Algorithm 1 -- P. Berkhin, A Survey on PageRank Computing, Internet Mathematics, 2005
template <class TEdgeW, template <class> class TGraph >
double GetWPageRank(const TPt<TGraph<TEdgeW> >& WGraph, TIntFltH& WPRankH, const double& c, const double& eps, const int& MaxIter) {
  typename TGraph<TEdgeW>::TNodeI NI;
  const int NNodes = WGraph->GetNodes();
  double diff = 0.0; 
  WPRankH.Gen(NNodes);
  // initialise with uniform score across nodes (standard uniform teleportation probability)
  for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++) {
    WPRankH.AddDat(NI.GetId(), 1.0 / double(NNodes));
  }
  TFltV TmpV(NNodes);
  // iterations
  for (int iter = 0; iter < MaxIter; iter++) {
    int j = 0; // counts nodes
    for (NI = WGraph->BegNI(); NI < WGraph->EndNI(); NI++, j++) {
      TmpV[j] = 0;
      for (int edge = 0; edge < NI.GetInDeg(); edge++) {
        const int InNId = NI.GetInNId(edge);
        const TEdgeW EdgeW = NI.GetInEW(edge);
        const TEdgeW InNOutWDeg = WGraph->GetNI(InNId).GetWOutDeg();
        // cast TEdgeW to double for transition matrix probability
        if (InNOutWDeg > 0) { // is this needed (?)
          TmpV[j] += c * WPRankH.GetDat(InNId) * (double(EdgeW) / double(InNOutWDeg));
        }
      }
      // TmpV[j] =  C*TmpV[j]; // Berkhin (the correct way of doing it)
    }
    double sum = 0, temp;
    diff = 0.0;
    for (int i = 0; i < TmpV.Len(); i++) { sum += TmpV[i]; } // L1 norm (vector is strictly positive valued)
    const double Leaked = (1.0 - sum) / double(NNodes); // standard uniform teleportation probability
    for (int i = 0; i < WPRankH.Len(); i++) { // re-insert leaked PageRank
      temp = TmpV[i] + Leaked;
      diff += fabs(temp - WPRankH[i]); // double / float absolute value
      WPRankH[i] = temp;
    }
    if (diff < eps) { break; }
  }
  return(diff);
}

} // namespace TSnap

#endif
