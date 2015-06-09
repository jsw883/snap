// Headers (?)

#include "stdafx.h"
#include "galg.h"

// // Returns k deep egonets for node CtrNId (in / out / undirected)
// PNGraph TSnap::GetInEgonet(const PNGraph& Graph, const int CtrNId, const int k) {
//   PNGraph Ego = TNGraph::New(); Ego->AddNode(CtrNId);
//   TCnCom BCnCom;
//   GetBcc(Graph, Ego, BCnCom, k);
//   return(TSnap::GetSubGraph(Graph, BCnCom.NIdV));
// }
// PNGraph TSnap::GetOutEgonet(const PNGraph& Graph, const int CtrNId, const int k) {
//   PNGraph Ego = TNGraph::New(); Ego->AddNode(CtrNId);
//   TCnCom FCnCom;
//   GetFcc(Graph, Ego, FCnCom, k);
//   return(TSnap::GetSubGraph(Graph, FCnCom.NIdV));
// }
// PNGraph TSnap::GetEgonet(const PNGraph& Graph, const int CtrNId, const int k) {
//   PNGraph Ego = TNGraph::New(); Ego->AddNode(CtrNId);
//   TCnCom CnCom;
//   GetUcc(Graph, Ego, CnCom, k);
//   return(TSnap::GetSubGraph(Graph, CnCom.NIdV));
// }
