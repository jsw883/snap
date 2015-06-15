#ifndef core_centrnew
#define core_centrnew

// Headers (?)

#include "enums.h"

// Edge direction enumerators (for specifying in / out / undirected algorithms)

namespace TSnap {

/// Computes directed first degree centrality
template <class PGraph> void GetDirDegreeCentr(const PGraph& Graph, TIntFltH& DegCentrH, const TEdgeDir& dir);
/// Computes first in degree centrality
template <class PGraph> void GetInDegreeCentr(const PGraph& Graph, TIntFltH& InDegCentrH);
/// Computes first out degree centrality
template <class PGraph> void GetOutDegreeCentr(const PGraph& Graph, TIntFltH& OutDegCentrH);
/// Computes first degree centrality
template <class PGraph> void GetDegreeCentr(const PGraph& Graph, TIntFltH& DegCentrH);

/// Computes first degree centralities (in / out / undirected)
template <class PGraph> void GetDegreeCentrVH(const PGraph& Graph, TIntFltVH& DegCentrVH);

/// Directed eigenvector centrality
template <class PGraph> double GetDirEigenVectorCentr(const PGraph& Graph, TIntFltH& EigCentrH, const TEdgeDir& dir, const double& eps = 1e-4, const int& MaxIter = 100); 
/// Directed eigenvector centrality
template <class PGraph> double GetInEigenVectorCentr(const PGraph& Graph, TIntFltH& InEigCentrH, const double& eps = 1e-4, const int& MaxIter = 100);
/// Directed eigenvector centrality
template <class PGraph> double GetOutEigenVectorCentr(const PGraph& Graph, TIntFltH& OutEigCentrH, const double& eps = 1e-4, const int& MaxIter = 100);
/// Directed eigenvector centrality
template <class PGraph> double GetEigenVectorCentr(const PGraph& Graph, TIntFltH& EigCentrH, const double& eps = 1e-4, const int& MaxIter = 100);

/// Eigenvector centralities (standard power method)
template<class PGraph> TFltV GetEigenVectorCentrVH(const PGraph& Graph, TIntFltVH& EigCentrVH, const double& eps = 1e-4, const int& MaxIter = 100);

/// PageRank centrality (for more info see: http://en.wikipedia.org/wiki/PageRank)
template <class PGraph> double GetPageRankNew(const PGraph& Graph, TIntFltH& PRankH, const double& c=0.85, const double& eps=1e-4, const int& MaxIter=100);

/// Computes first in degree centralities
template <class PGraph>
void GetDirDegreeCentr(const PGraph& Graph, TIntFltH& DegCentrH, const TEdgeDir& dir) {
  typename PGraph::TObj::TNodeI NI;
  DegCentrH.Gen(Graph->GetNodes());
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    int deg = 0;
    double centr = 0.0; // initialize to avoid errors
    if (Graph->GetNodes() > 1) {
      switch(dir) {
        case edInDirected: deg = NI.GetInDeg(); break;
        case edOutDirected: deg = NI.GetOutDeg(); break;
        case edUnDirected: deg = NI.GetDeg(); break;
      }
      centr = double(deg) / double(Graph->GetNodes() - 1);
    } else {
      centr = 0.0;
    }
    DegCentrH.AddDat(NI.GetId(), centr);
  }
}
template <class PGraph>
void GetInDegreeCentr(const PGraph& Graph, TIntFltH& InDegCentrH) {
  GetDirDegreeCentr(Graph, InDegCentrH, edInDirected);
}
template <class PGraph>
void GetOutDegreeCentr(const PGraph& Graph, TIntFltH& OutDegCentrH) {
  GetDirDegreeCentr(Graph, OutDegCentrH, edOutDirected);
}
template <class PGraph>
void GetDegreeCentr(const PGraph& Graph, TIntFltH& DegCentrH) {
  GetDirDegreeCentr(Graph, DegCentrH, edUnDirected);
}

template <class PGraph>
void GetDegreeCentrVH(const PGraph& Graph, TIntFltVH& DegCentrVH) {
  typename PGraph::TObj::TNodeI NI;
  TFltV DegCentrV;
  TIntFltH InDegCentrH, OutDegCentrH, DegCentrH;
  DegCentrVH.Gen(Graph->GetNodes());
  GetInDegreeCentr(Graph, InDegCentrH);
  GetOutDegreeCentr(Graph, OutDegCentrH);
  GetDegreeCentr(Graph, DegCentrH);
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    DegCentrV.Clr();
    DegCentrV.Add(InDegCentrH.GetDat(NI.GetId()));
    DegCentrV.Add(OutDegCentrH.GetDat(NI.GetId()));
    DegCentrV.Add(DegCentrH.GetDat(NI.GetId()));
    DegCentrVH.AddDat(NI.GetId(), DegCentrV);
  }
}

// Standard power method for computing leading eigenvector and eigenvalue
template <class PGraph>
double GetDirEigenVectorCentr(const PGraph& Graph, TIntFltH& EigCentrH, const TEdgeDir& dir, const double& eps, const int& MaxIter) {
  typename PGraph::TObj::TNodeI NI;
  const int NNodes = Graph->GetNodes();
  int deg = 0;
  double eig = 0.0, diff = 0.0;
  EigCentrH.Gen(NNodes);
  // initialize vector values
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    EigCentrH.AddDat(NI.GetId(), 1.0 / NNodes); // uniform distribution initially
    IAssert(NI.GetId() == EigCentrH.GetKey(EigCentrH.Len() - 1));
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
          case edInDirected: TmpV[j] += EigCentrH.GetDat(NI.GetInNId(e)); break;
          case edOutDirected: TmpV[j] += EigCentrH.GetDat(NI.GetOutNId(e)); break;
          case edUnDirected: TmpV[j] += EigCentrH.GetDat(NI.GetNbrNId(e)); break;
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
      diff += fabs(EigCentrH.GetDat(NI.GetId()) - TmpV[j]);
    }
    // set new values
    j = 0;
    for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++, j++) {
      EigCentrH.AddDat(NI.GetId(), TmpV[j]);
    }
    if (diff < eps) { break; }
  }
  return(diff); // didn't converge return(-1) (?)
}
template <class PGraph>
double GetInEigenVectorCentr(const PGraph& Graph, TIntFltH& InEigCentrH, const double& eps, const int& MaxIter) {
  return GetDirEigenVectorCentr(Graph, InEigCentrH, edInDirected, eps, MaxIter);
}
template <class PGraph>
double GetOutEigenVectorCentr(const PGraph& Graph, TIntFltH& OutEigCentrH, const double& eps, const int& MaxIter) {
  return GetDirEigenVectorCentr(Graph, OutEigCentrH, edOutDirected, eps, MaxIter);
}
template <class PGraph>
double GetEigenVectorCentr(const PGraph& Graph, TIntFltH& EigCentrH, const double& eps, const int& MaxIter) {
  return GetDirEigenVectorCentr(Graph, EigCentrH, edUnDirected, eps, MaxIter);
}

template <class PGraph>
TFltV GetEigenVectorCentrVH(const PGraph& Graph, TIntFltVH& EigCentrVH, const double& eps, const int& MaxIter) {
  typename PGraph::TObj::TNodeI NI;
  TIntFltH InEigCentrH, OutEigCentrH, EigCentrH;
  TFltV DiffV, EigV;
  DiffV.Clr();
  DiffV.Add(GetInEigenVectorCentr(Graph, InEigCentrH, eps, MaxIter));
  DiffV.Add(GetOutEigenVectorCentr(Graph, OutEigCentrH, eps, MaxIter));
  DiffV.Add(GetEigenVectorCentr(Graph, EigCentrH, eps, MaxIter));
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    EigV.Clr();
    EigV.Add(InEigCentrH.GetDat(NI.GetId()));
    EigV.Add(OutEigCentrH.GetDat(NI.GetId()));
    EigV.Add(EigCentrH.GetDat(NI.GetId()));
    EigCentrVH.AddDat(NI.GetId(), EigV);
  }
  return(DiffV);
}

// Berkhin Algorithm 1 -- P. Berkhin, A Survey on PageRank Computing, Internet Mathematics, 2005
template<class PGraph>
double GetPageRankNew(const PGraph& Graph, TIntFltH& PRankH, const double& c, const double& eps, const int& MaxIter) {
  typename PGraph::TObj::TNodeI NI;
  const int NNodes = Graph->GetNodes();
  double diff = 0.0; 
  PRankH.Gen(NNodes);
  // initialise with uniform score across nodes (standard uniform teleportation probability)
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    PRankH.AddDat(NI.GetId(), 1.0 / double(NNodes));
  }
  TFltV TmpV(NNodes);
  // iterations
  for (int iter = 0; iter < MaxIter; iter++) {
    int j = 0; // counts nodes
    for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++, j++) {
      TmpV[j] = 0;
      for (int edge = 0; edge < NI.GetInDeg(); edge++) {
        const int InNId = NI.GetInNId(edge);
        const int InNOutDeg = Graph->GetNI(InNId).GetOutDeg();
        if (InNOutDeg > 0) { // is this needed (?)
          TmpV[j] += c * PRankH.GetDat(InNId) / double(InNOutDeg);
        }
      }
      // TmpV[j] =  C*TmpV[j]; // Berkhin (the correct way of doing it)
    }
    double sum = 0, temp;
    diff = 0.0;
    for (int i = 0; i < TmpV.Len(); i++) { sum += TmpV[i]; } // L1 norm (vector is strictly positive valued)
    const double Leaked = (1.0 - sum) / double(NNodes); // standard uniform teleportation probability
    for (int i = 0; i < PRankH.Len(); i++) { // re-insert leaked PageRank
      temp = TmpV[i] + Leaked;
      diff += fabs(temp - PRankH[i]); // double / float absolute value
      PRankH[i] = temp;
    }
    if (diff < eps) { break; }
  }
  return(diff);
}

} // namespace TSnap

#endif
