#ifndef core_wcmty
#define core_wcmty

// Headers (?)

//#//////////////////////////////////////////////
/// Louvain method

// TODO: implement local improvements (for any community detection method)

namespace TSnap {

template <class Community, class TEdgeW>
double LouvainMethod(const TPt<TWNGraph<TEdgeW> >& Graph, TIntIntVH& NIdCmtyVH, const TEdgeDir& dir, const double& eps = 1e-5, const double& delta = 1e-2, const int& MaxIter = 1000, TRnd& Rnd = TInt::Rnd);

/// Base community class to be inherited and specialized.
template <class TEdgeW>
class Community {
public:
  // Graph being partitioned, node to community mapping, and node self weights
  TPt<TWNGraph<TEdgeW> > Graph;
  TIntIntH NIdCmtyIdH;
  THash<TInt, TEdgeW> NIdWH; // avoid requiring graph self loops
  TIntIntH NIdSizeH; // accurate node sizes
  // Community sizes and weights for efficiency
  TIntV CmtySizeV;
  TVec<TEdgeW> CmtyInWV, CmtyOutWV, CmtyWV; // community weights
  // Totals
  int NCmty;
  TEdgeW GraphTotalW;
public:
  /// Constructor only requires graph to be specified.
  Community(const TPt<TWNGraph<TEdgeW> >& Graph) : Graph(Graph) {
    // Iterators
    typename TWNGraph<TEdgeW>::TNodeI NI;
    int NId, CmtyId;
    // Setup community sizes and weights
    for (NI = Graph->BegNI(), CmtyId = 0; NI < Graph->EndNI(); NI++, CmtyId++) {
      NId = NI.GetId();
      NIdCmtyIdH.AddDat(NId, CmtyId);
      NIdWH.AddDat(NId, 0);
      NIdSizeH.AddDat(NId, 1);
      CmtySizeV.Add(NIdSizeH.GetDat(NId));
      CmtyInWV.Add(NI.GetWInDeg());
      CmtyOutWV.Add(NI.GetWOutDeg());
      CmtyWV.Add(0);
    }
    NCmty = CmtyId;
    GraphTotalW = this->Graph->GetTotalW();
  }
  /// Computes the change in quality if a node is moved into another community. To be implemented by inheriting child.
  virtual double NodeMoveQuality(const int& NId, const int& NewCmtyId) = 0;
  /// Computes the GetQuality. To be implemented by inheriting child.
  virtual double GetQuality() = 0;
  /// Move node into another community, updating community weights accordingly
  void MoveNode(const int& NId, const int& NewCmtyId) {
    // Iterator for getting node properties and neighborhood
    typename TWNGraph<TEdgeW>::TNodeI NI = Graph->GetNI(NId);
    TInt& OldCmtyId = NIdCmtyIdH.GetDat(NId); // exact reference needed
    int DstNId, SrcNId, NSize = NIdSizeH.GetDat(NId);
    TEdgeW W, NW = NIdWH.GetDat(NId);
    // Update community sizes
    // printf("\n");
    // printf("CmtySizeV[%d] (= %d) -= %d\n", (int) OldCmtyId, (int) CmtySizeV[OldCmtyId], (int) NSize);
    // printf("CmtySizeV[%d] (= %d) += %d\n", (int) NewCmtyId, (int) CmtySizeV[NewCmtyId], (int) NSize);
    CmtySizeV[OldCmtyId] -= NSize;
    CmtySizeV[NewCmtyId] += NSize;
    // printf("CmtySizeV[%d] (= %d) -= %d\n", (int) OldCmtyId, (int) CmtySizeV[OldCmtyId], (int) NSize);
    // printf("CmtySizeV[%d] (= %d) += %d\n", (int) NewCmtyId, (int) CmtySizeV[NewCmtyId], (int) NSize);
    // printf("\n");
    // Update community weights
    CmtyInWV[OldCmtyId] -= NW; CmtyOutWV[OldCmtyId] -= NW; CmtyWV[OldCmtyId] -= NW;
    CmtyInWV[NewCmtyId] += NW; CmtyOutWV[NewCmtyId] += NW; CmtyWV[NewCmtyId] += NW;
    for (int e = 0; e < NI.GetOutDeg(); e++) {
      DstNId = NI.GetOutNId(e);
      W = NI.GetOutEW(e);
      if (NIdCmtyIdH.GetDat(DstNId) == OldCmtyId) {
        CmtyWV[OldCmtyId] -= W;
        CmtyOutWV[OldCmtyId] -= W;
        CmtyOutWV[NewCmtyId] += W;
      } else {
        CmtyOutWV[OldCmtyId] -= W;
        CmtyOutWV[NewCmtyId] += W;
        if (NIdCmtyIdH.GetDat(DstNId) == NewCmtyId) {
          CmtyWV[NewCmtyId] += W;
        }
      }
    }
    for (int e = 0; e < NI.GetInDeg(); e++) {
      SrcNId = NI.GetInNId(e);
      W = NI.GetInEW(e);
      if (NIdCmtyIdH.GetDat(SrcNId) == OldCmtyId) {
        CmtyWV[OldCmtyId] -= W;
        CmtyInWV[OldCmtyId] -= W;
        CmtyInWV[NewCmtyId] += W;
      } else {
        CmtyInWV[OldCmtyId] -= W;
        CmtyInWV[NewCmtyId] += W;
        if (NIdCmtyIdH.GetDat(SrcNId) == NewCmtyId) {
          CmtyWV[NewCmtyId] += W;
        }
      }
    }
    OldCmtyId = NewCmtyId; // updates old community to new community
  }
  /// Clean up by removing empty communities and renumbering communities by size (descending)
  void CleanCmty() {
    // Variables
    TIntIntH OldNewCmtyH;
    TIntV NewCmtySizeV;
    TVec<TEdgeW> NewCmtyInWV, NewCmtyOutWV, NewCmtyWV;
    TIntV::TIter VI;
    TIntIntH::TIter HI;
    int OldCmtyId, NewCmtyId;
    // Set up community renumbering using community sizes
    for (VI = CmtySizeV.BegI(), OldCmtyId = 0; VI < CmtySizeV.EndI(); VI++, OldCmtyId++) {
      if (VI->Val > 0) {
        OldNewCmtyH.AddDat(OldCmtyId, VI->Val);
      }
    }
    OldNewCmtyH.SortByDat(false); // sort communities by size (descending)
    // printf("SIZE\n");
    int sum = 0;
    for (HI = OldNewCmtyH.BegI(), NewCmtyId = 0; HI < OldNewCmtyH.EndI(); HI++, NewCmtyId++) {
      // printf("%d: %d (old: %d)\n", NewCmtyId, (int) HI.GetDat(), (int) HI.GetKey());
      sum += HI.GetDat();
      HI.GetDat() = NewCmtyId; // renumber community
    }
    // printf("SUM = %d\n\n", sum);
    NCmty = NewCmtyId;
    // Renumber communities
    for (HI = NIdCmtyIdH.BegI(); HI < NIdCmtyIdH.EndI(); HI++) {
      TInt& CmtyId = HI.GetDat();
      CmtyId = OldNewCmtyH.GetDat(CmtyId);
    }
    // Reorder community sizes and weights
    for (HI = OldNewCmtyH.BegI(); HI < OldNewCmtyH.EndI(); HI++) {
      OldCmtyId = HI.GetKey();
      NewCmtySizeV.Add(CmtySizeV[OldCmtyId]);
      NewCmtyInWV.Add(CmtyInWV[OldCmtyId]);
      NewCmtyOutWV.Add(CmtyOutWV[OldCmtyId]);
      NewCmtyWV.Add(CmtyWV[OldCmtyId]);
    }
    CmtySizeV = NewCmtySizeV;
    CmtyInWV = NewCmtyInWV;
    CmtyOutWV = NewCmtyOutWV;
    CmtyWV = NewCmtyWV;
  }
  /// Collapse graph, converting communities into nodes
  void CollapseGraph() {
    // Variables
    typename TWNGraph<TEdgeW>::TNodeI NI;
    typename TWNGraph<TEdgeW>::TEdgeI EI;
    TIntIntH::TIter HI;
    int NId, CmtyId, SrcCmtyId, DstCmtyId;
    // Create new graph with nodes as communities
    TPt<TWNGraph<TEdgeW> > NewGraph = TPt<TWNGraph<TEdgeW> >::New();
    for (CmtyId = 0; CmtyId < NCmty; CmtyId++) {
      NewGraph->AddNode(CmtyId);
    }
    for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
      SrcCmtyId = NIdCmtyIdH.GetDat(EI.GetSrcNId());
      DstCmtyId = NIdCmtyIdH.GetDat(EI.GetDstNId());
      if (SrcCmtyId != DstCmtyId) {
        NewGraph->AddEdge(SrcCmtyId, DstCmtyId, EI.GetW()); // increments W
      }
    }
    // Update attributes of community
    Graph = NewGraph;
    NIdCmtyIdH.Clr();
    NIdWH.Clr();
    NIdSizeH.Clr();
    for (NI = Graph->BegNI(), CmtyId = 0; NI < Graph->EndNI(); NI++, CmtyId++) {
      NId = NI.GetId();
      NIdCmtyIdH.AddDat(NId, CmtyId);
      NIdWH.AddDat(NId, CmtyWV[CmtyId]); // update node self weights
      NIdSizeH.AddDat(NId, CmtySizeV[CmtyId]); // update node sizes
    }
  }
};


/// Modularity objective community
template <class TEdgeW>
class ModularityCommunity : public Community<TEdgeW> {
public:
  /// Calls parent constructor 
  ModularityCommunity(const TPt<TWNGraph<TEdgeW> >& Graph) : Community<TEdgeW>(Graph) { }
  /// Computes the change in quality if a node is moved into another community. To be implemented by inheriting child.
  virtual double NodeMoveQuality(const int& NId, const int& NewCmtyId);
  /// Computes the quality. To be implemented by inheriting child.
  virtual double GetQuality();
};


/// Computes the change in quality if a node is moved into another community. To be implemented by inheriting child.
template <class TEdgeW>
double ModularityCommunity<TEdgeW>::NodeMoveQuality(const int& NId, const int& NewCmtyId) {
  // Variables
  typename TWNGraph<TEdgeW>::TNodeI NI = this->Graph->GetNI(NId);
  double old_mod, new_mod;
  int DstNId, SrcNId, OldCmtyId = this->NIdCmtyIdH.GetDat(NId);
  TEdgeW W, NW = this->NIdWH.GetDat(NId);
  // Local copies of old and new community weights
  TEdgeW OldCmtyInW = this->CmtyInWV[OldCmtyId], OldCmtyOutW = this->CmtyOutWV[OldCmtyId], OldCmtyW = this->CmtyWV[OldCmtyId];
  TEdgeW NewCmtyInW = this->CmtyInWV[NewCmtyId], NewCmtyOutW = this->CmtyOutWV[NewCmtyId], NewCmtyW = this->CmtyWV[NewCmtyId];
  // Compute old modularity
  old_mod = OldCmtyW - ((double) OldCmtyOutW*OldCmtyInW) / ((double) this->GraphTotalW)
          + NewCmtyW - ((double) NewCmtyOutW*NewCmtyInW) / ((double) this->GraphTotalW);
  // Update community weights
  OldCmtyInW -= NW; OldCmtyOutW -= NW; OldCmtyW -= NW;
  NewCmtyInW += NW; NewCmtyOutW += NW; NewCmtyW += NW;
  for (int e = 0; e < NI.GetOutDeg(); e++) {
    DstNId = NI.GetOutNId(e);
    W = NI.GetOutEW(e);
    if (this->NIdCmtyIdH.GetDat(DstNId) == OldCmtyId) {
      OldCmtyW -= W;
      OldCmtyOutW -= W;
      NewCmtyOutW += W;
    } else {
      OldCmtyOutW -= W;
      NewCmtyOutW += W;
      if (this->NIdCmtyIdH.GetDat(DstNId) == NewCmtyId) {
        NewCmtyW += W;
      }
    }
  }
  for (int e = 0; e < NI.GetInDeg(); e++) {
    SrcNId = NI.GetInNId(e);
    W = NI.GetInEW(e);
    if (this->NIdCmtyIdH.GetDat(SrcNId) == OldCmtyId) {
      OldCmtyW -= W;
      OldCmtyInW -= W;
      NewCmtyInW += W;
    } else {
      OldCmtyInW -= W;
      NewCmtyInW += W;
      if (this->NIdCmtyIdH.GetDat(SrcNId) == NewCmtyId) {
        NewCmtyW += W;
      }
    }
  }
  // Compute new modularity
  new_mod = OldCmtyW - ((double) OldCmtyOutW*OldCmtyInW) / ((double) this->GraphTotalW)
          + NewCmtyW - ((double) NewCmtyOutW*NewCmtyInW) / ((double) this->GraphTotalW);
  // Return change in modularity
  return (new_mod - old_mod) / ((double) this->GraphTotalW); // normalized modularity
}

/// Computes the quality. To be implemented by inheriting child.
template <class TEdgeW>
double ModularityCommunity<TEdgeW>::GetQuality() {
  // Variables
  typename TIntIntH::TIter HI;
  double mod = 0.0;
  int CmtyId;
  // Compute modularity by iterating over the communities (rather than edges)
  for (CmtyId = 0; CmtyId < this->NCmty; CmtyId++) {
    if (this->CmtySizeV[CmtyId] > 0) {
      // Community weights
      TEdgeW CmtyW = this->CmtyWV[CmtyId];
      TEdgeW CmtyOutW = this->CmtyOutWV[CmtyId];
      TEdgeW CmtyInW = this->CmtyInWV[CmtyId];
      // Community modularity
      mod += CmtyW - ((double) CmtyOutW*CmtyInW) / ((double) this->GraphTotalW);
    }
  }
  return mod / ((double) this->GraphTotalW); // normalized modularity
}

}

namespace TSnap {

template <class Community, class TEdgeW>
double LouvainMethod(const TPt<TWNGraph<TEdgeW> >& Graph, TIntIntVH& NIdCmtyVH, const TEdgeDir& dir, const double& eps, const double& delta, const int& MaxIter, TRnd& Rnd) {
  
  // Variables
  TPt<TWNGraph<TEdgeW> > GraphCopy = Graph; // smart pointer working graph copy
  typename TWNGraph<TEdgeW>::TNodeI NI;
  TIntIntVH::TIter HI;
  TIntV NIdV;
  // Compute the absolute minimum number of moves required
  int MinMove = (int) delta*Graph->GetNodes();
  // Phase ariables
  int phase = 0;
  double phaseImprov = 0.0;
  // Setup community and output community hierarchy 
  NIdCmtyVH.Clr();
  int i;
  for (NI = GraphCopy->BegNI(), i = 0; NI < GraphCopy->EndNI(); NI++, i++) {
    int NId = NI.GetId();
    NIdCmtyVH.AddKey(NId);
    NIdCmtyVH.GetDat(NId).Add(i);
  }

  Community Cmty(GraphCopy);

  // Louvain greedy optimization method
  while (phase == 0 || phaseImprov > eps) { // while quality improving
    
    TIntV::TIter VI;
    int count, sum;
  
    // printf("\nCMTSIZEV\n");
    sum = 0;
    for (VI = Cmty.CmtySizeV.BegI(), count = 0; VI < Cmty.CmtySizeV.EndI(); VI++, count++) {
      // printf("%d: %d\n", (int) count, (int) VI->Val);
      sum += VI->Val;
    }
    // printf("SUM = %d\n\n", sum);

    // Reset phase improvement in modularity
    phaseImprov = 0.0;
    // Iter variables
    int moves = 0, iter = 0;
    double iterImprov = 0.0;
    
    // Phase A
    // -------
    
    while (iter == 0 || (iterImprov > eps && moves > MinMove && iter < MaxIter)) { // while quality improving and parameters not exceeded
      
      // Reset moves and iter improvement in modularity
      moves = 0;
      iterImprov = 0.0;
      
      // TODO: randomly permute nodes (?)
      
      // GraphCopy.GetNIdV(NIdV);
      // NIdV.Shuffle(Rnd);
      
      for (NI = GraphCopy->BegNI(); NI < GraphCopy->EndNI(); NI++) { // nodes, randomly permuted
        // Local variables
        double moveImprov = 0.0, bestMoveImprov = 0.0;
        int NId = NI.GetId();
        int OldCmtyId, NewCmtyId, BestCmtyId = -1;
        
        // TODO: implement switching using
        //         (a) all
        //         (b) neighbors
        //         (c) random communities
        
        // Only iterate over the neighbors for the direction specified
        for (int e = 0; e < NI.GetDeg(dir); e++) {
          OldCmtyId = Cmty.NIdCmtyIdH.GetDat(NId);
          NewCmtyId = Cmty.NIdCmtyIdH.GetDat(NI.GetNbrNId(e, dir));
          if (NewCmtyId != OldCmtyId) {
            moveImprov = Cmty.NodeMoveQuality(NId, NewCmtyId);
            if (moveImprov > bestMoveImprov) {
              BestCmtyId = NewCmtyId;
              bestMoveImprov = moveImprov;
            }
          }
        }
        // Execute best move if it gives an improvement in modularity 
        if (bestMoveImprov > 0.0 && BestCmtyId != -1) {
          Cmty.MoveNode(NId, BestCmtyId);
          iterImprov += bestMoveImprov;
          moves++;
        }
        
      }
      
      iter++;
      phaseImprov += iterImprov;
      
    }
    
    if (phaseImprov == 0.0) {
      break; // break if not improved (communities have not changed)
    }
    
    // TIntIntH::TIter HHI;
    // printf("MAPPING\n");
    // for (HHI = Cmty.NIdCmtyIdH.BegI(); HHI < Cmty.NIdCmtyIdH.EndI(); HHI++) {
    //   printf("%d: %d\n", (int) HHI.GetKey(), (int) HHI.GetDat());
    // }
    // printf("\n");

    // Remove empty communities and renumber communities by size (descending)
    Cmty.CleanCmty();
    
    // Update output community hierarchy
    // printf("OUTPUT (phase: %d)\n", phase);
    for (HI = NIdCmtyVH.BegI(); HI < NIdCmtyVH.EndI(); HI++) {
      TIntV& CmtyV = HI.GetDat();
      if (phase == 0) {
        // printf("%d: (%d, %d)\n", (int) HI.GetKey(), (int) HI.GetKey(), (int) Cmty.NIdCmtyIdH.GetDat(CmtyV[phase]));
        CmtyV.Add(Cmty.NIdCmtyIdH.GetDat(HI.GetKey()));
      } else {
        // printf("%d: (%d, %d)\n", (int) HI.GetKey(), (int) CmtyV[phase], (int) Cmty.NIdCmtyIdH.GetDat(CmtyV[phase]));
        CmtyV.Add(Cmty.NIdCmtyIdH.GetDat(CmtyV[phase]));
      }
    }
    // printf("\n");    
    // -------
    
    // Phase B
    // -------
    
    // Collapse graph, converting communities into nodes
    Cmty.CollapseGraph();
    GraphCopy = Cmty.Graph; // update working graph copy
    
    // -------
    
    phase++;
    
  }
  
  return Cmty.GetQuality(); // return modularity
  
}

} // namespace TSnap

namespace TSnap {

// Summary method
void CmtyHierarchySummary(const TIntIntVH& NIdCmtyVH, const int& PhaseThreshold = 1, const int& NCmtyThreshold = -1, const TStr& Desc = "Community hierarchy\n-------------------");

} // namespace TSnap

#endif
