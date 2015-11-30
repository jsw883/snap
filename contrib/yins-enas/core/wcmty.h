#ifndef core_wcmty
#define core_wcmty

// Headers (?)

//#//////////////////////////////////////////////
/// Generic community functions

namespace TSnap {

template <class TEdgeW, class Community>
void GetCmtyGraph(TPt<TWNGraph<TEdgeW> >& Graph, Community& Cmty) {
  
  printf("collapsing graph:\n");
  
  typename TWNGraph<TEdgeW>::TEdgeI EI;
  TIntIntH::TIter HI;
  // Clear graph and community weights before copying
  TPt<TWNGraph<TEdgeW> > CmtyGraph = TPt<TWNGraph<TEdgeW> >::New();
  TVec<TEdgeW> CmtyWV;
  // For unique communities (after cleaning)
  for (int CmtyId = 0; CmtyId < Cmty.NCmty; CmtyId++) {
    CmtyGraph->AddNode(CmtyId);
  }
  // for (HI = Cmty.NIdCmtyIdH.BegI(); HI < Cmty.NIdCmtyIdH.EndI(); HI++) {
  //   int CmtyId = HI.GetDat();
  //   if (!CmtyGraph->IsNode(CmtyId)) {
  //     CmtyGraph->AddNode(CmtyId);
  //   }
  //   CmtyWV.SortByKey(true); // not strictly necessary (?)
  // }
  // Iterate graph edges
  for (EI = Graph->BegEI(); EI < Graph->EndEI(); EI++) {
    int SrcCmtyId = Cmty.NIdCmtyIdH.GetDat(EI.GetSrcNId());
    int DstCmtyId = Cmty.NIdCmtyIdH.GetDat(EI.GetDstNId());
    if (SrcCmtyId != DstCmtyId) {
      CmtyGraph->AddEdge(SrcCmtyId, DstCmtyId, EI.GetW()); // incrementally "add edge" or "add to edge weight"
    } // else {
      // printf("SRC ID: %d\nDST ID: %d\n (W: %f)\n", SrcCmtyId, DstCmtyId, (double) EI.GetW());
      // CmtyWV[SrcCmtyId] += EI.GetW();
    // }
  }
  
  printf("community graph: (%d, %d, %f)\n", CmtyGraph->GetNodes(), CmtyGraph->GetEdges(), (double) CmtyGraph->GetTotalW());
  
  // Cmty = Community(CmtyGraph, Cmty.CmtyWV);
  Cmty.Graph = CmtyGraph;
  typename TWNGraph<TEdgeW>::TNodeI NI;
  Cmty.NIdCmtyIdH.Clr();
  Cmty.NIdWH.Clr();
  int CmtyId = 0;
  for (NI = CmtyGraph->BegNI(); NI < CmtyGraph->EndNI(); NI++) {
    int NId = NI.GetId();
    Cmty.NIdCmtyIdH.AddDat(NId, CmtyId);
    Cmty.NIdWH.AddDat(NId, Cmty.CmtyWV[CmtyId]);
    CmtyId++;
  }
  Cmty.CmtySizeV.PutAll(1);
  Graph = CmtyGraph;
}

} // namespace TSnap

//#//////////////////////////////////////////////
/// Louvain method

// TODO: think carefully about default values
// TODO: update class to allow self loops (?)
// TODO: implement local improvements (for any community detection method)

namespace TSnap {

template <class Community, class TEdgeW>
double LouvainMethod(const TPt<TWNGraph<TEdgeW> >& Graph, TIntIntVH& NIdCmtyVH, const TEdgeDir& Dir, const double& eps = 1e-5, const double& delta = 1e-2, const int& max_iters = 1000);

// Community class to be inherited
template <class TEdgeW>
class Community {
public:
  TPt<TWNGraph<TEdgeW> > Graph;
  TIntIntH NIdCmtyIdH; // community membership hash table (NId, Cmty)
  THash<TInt, TEdgeW> NIdWH; // avoids requiring self loops
  int NCmty;
  TEdgeW TotalW;
  TIntV CmtySizeV; // community sizes (CmtyId, Size)
  TVec<TEdgeW> CmtyInWV, CmtyOutWV, CmtyWV; // community weights
public:
  // Community(const TPt<TWNGraph<TEdgeW> >& Graph) : Graph(Graph) {
  //   THash<TInt, TEdgeW> NIdWH;
  //   CmtyWV.Gen(Graph->GetNodes());
    
  //   Community(Graph, CmtyWV);
  // }
  Community(const TPt<TWNGraph<TEdgeW> >& Graph, const THash<TInt, TEdgeW>& NIdWH) : Graph(Graph), NIdWH(NIdWH) {
    typename TWNGraph<TEdgeW>::TNodeI NI;
    int CmtyId = 0;
    for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
      int NId = NI.GetId();
      NIdCmtyIdH.AddDat(NId, CmtyId);
      CmtySizeV.Add(1);
      CmtyInWV.Add(NIdWH.GetDat(NId) + NI.GetWInDeg());
      CmtyOutWV.Add(NIdWH.GetDat(NId) + NI.GetWOutDeg());
      CmtyWV.Add(NIdWH.GetDat(NId));
      CmtyId++;
    }
    NCmty = CmtyId;
    TotalW = this->Graph->GetTotalW();
  }
  // Methods
  void move_node(const int& NId, const int& NewCmtyId) {
    typename TWNGraph<TEdgeW>::TNodeI NI = Graph->GetNI(NId);
    // get old community
    TInt& OldCmtyId = NIdCmtyIdH.GetDat(NId);
    // sizes
    CmtySizeV[OldCmtyId]--;
    CmtySizeV[NewCmtyId]++;
    // move edge weights
    TEdgeW NIdW = NIdWH.GetDat(NId);
    CmtyInWV[OldCmtyId] -= NIdW;
    CmtyOutWV[OldCmtyId] -= NIdW;
    CmtyWV[OldCmtyId] -= NIdW;
    CmtyInWV[NewCmtyId] += NIdW;
    CmtyOutWV[NewCmtyId] += NIdW;
    CmtyWV[NewCmtyId] += NIdW;
    for (int e = 0; e < NI.GetOutDeg(); e++) {
      int DstNId = NI.GetOutNId(e);
      TEdgeW W = NI.GetOutEW(e);
      if (NIdCmtyIdH.GetDat(DstNId) == OldCmtyId) {
        CmtyWV[OldCmtyId] -= W;
        // CmtyInWV[OldCmtyId] += W;
        CmtyOutWV[OldCmtyId] -= W;
        CmtyOutWV[NewCmtyId] += W;
      } else {
        CmtyOutWV[OldCmtyId] -= W;
        CmtyOutWV[NewCmtyId] += W;
        if (NIdCmtyIdH.GetDat(DstNId) == NewCmtyId) {
          // CmtyInWV[NewCmtyId] -= W;
          CmtyWV[NewCmtyId] += W;
        }
      }
    }
    for (int e = 0; e < NI.GetInDeg(); e++) {
      int SrcNId = NI.GetInNId(e);
      TEdgeW W = NI.GetInEW(e);
      if (NIdCmtyIdH.GetDat(SrcNId) == OldCmtyId) {
        CmtyWV[OldCmtyId] -= W;
        // CmtyOutWV[OldCmtyId] += W;
        CmtyInWV[OldCmtyId] -= W;
        CmtyInWV[NewCmtyId] += W;
      } else {
        CmtyInWV[OldCmtyId] -= W;
        CmtyInWV[NewCmtyId] += W;
        if (NIdCmtyIdH.GetDat(SrcNId) == NewCmtyId) {
          // CmtyOutWV[NewCmtyId] -= W;
          CmtyWV[NewCmtyId] += W;
        }
      }
    }
    OldCmtyId = NewCmtyId; // set new community
  }
  void clean_up() {
    TIntIntH OldNewCmtyH;
    TIntV NewCmtySizeV;
    TVec<TEdgeW> NewCmtyInWV, NewCmtyOutWV, NewCmtyWV;
    TIntV::TIter VI;
    TIntIntH::TIter HI;
    // set up community id remapping
    int OldCmtyId = 0;
    for (VI = CmtySizeV.BegI(); VI < CmtySizeV.EndI(); VI++) {
      if (VI->Val > 0) {
        OldNewCmtyH.AddDat(OldCmtyId, VI->Val);
      }
      OldCmtyId++;
    }
    OldNewCmtyH.SortByDat(false); // sort descending
    int NewCmtyId = 0;
    for (HI = OldNewCmtyH.BegI(); HI < OldNewCmtyH.EndI(); HI++) {
      HI.GetDat() = NewCmtyId;
      NewCmtyId++;
    }
    NCmty = NewCmtyId;
    // renumber node id community mapping
    for (HI = NIdCmtyIdH.BegI(); HI < NIdCmtyIdH.EndI(); HI++) {
      TInt& CmtyId = HI.GetDat();
      CmtyId = OldNewCmtyH.GetDat(CmtyId); // not sure if this is const (?)
    }
    // reorder community weights
    // NewCmtySizeV.Gen(NCmty);
    // NewCmtyInWV.Gen(NCmty);
    // NewCmtyOutWV.Gen(NCmty);
    // NewCmtyWV.Gen(NCmty);
    for (HI = OldNewCmtyH.BegI(); HI < OldNewCmtyH.EndI(); HI++) {
      int OldCmtyId = HI.GetKey();
      NewCmtySizeV.Add(CmtySizeV[OldCmtyId]);
      NewCmtyInWV.Add(CmtyInWV[OldCmtyId]);
      NewCmtyOutWV.Add(CmtyOutWV[OldCmtyId]);
      NewCmtyWV.Add(CmtyWV[OldCmtyId]);
      NewCmtyId++;
    }
    CmtySizeV = NewCmtySizeV;
    CmtyInWV = NewCmtyInWV;
    CmtyOutWV = NewCmtyOutWV;
    CmtyWV = NewCmtyWV;
  }
};


// Modularity objective community  
template <class TEdgeW>
class ModularityCommunity : public Community<TEdgeW> {
private:
public:
  ModularityCommunity(const TPt<TWNGraph<TEdgeW> >& Graph, const THash<TInt, TEdgeW>& NIdWH) : Community<TEdgeW>(Graph, NIdWH) { }
  // Methods
  double move_node_diff(const int& NId, const int& NewCmtyId);
  double quality();
};

template <class TEdgeW>
double ModularityCommunity<TEdgeW>::move_node_diff(const int& NId, const int& NewCmtyId) {
  typename TWNGraph<TEdgeW>::TNodeI NI = this->Graph->GetNI(NId);
  const int& OldCmtyId = this->NIdCmtyIdH.GetDat(NId);
  // get local copies of community weights
  TEdgeW NIdW = this->NIdWH.GetDat(NId);
  TEdgeW OldCmtyInW = this->CmtyInWV[OldCmtyId];
  TEdgeW OldCmtyOutW = this->CmtyOutWV[OldCmtyId];
  TEdgeW OldCmtyW = this->CmtyWV[OldCmtyId];
  TEdgeW NewCmtyInW = this->CmtyInWV[NewCmtyId];
  TEdgeW NewCmtyOutW = this->CmtyOutWV[NewCmtyId];
  TEdgeW NewCmtyW = this->CmtyWV[NewCmtyId];
  // compute old modularity
  double old_mod = OldCmtyW - ((double) OldCmtyOutW*OldCmtyInW) / ((double) this->TotalW)
                 + NewCmtyW - ((double) NewCmtyOutW*NewCmtyInW) / ((double) this->TotalW);
  // move edge weights
  // OldCmtyW -= NIdW;
  // NewCmtyW += NIdW;
  for (int e = 0; e < NI.GetOutDeg(); e++) {
    int DstNId = NI.GetOutNId(e);
    TEdgeW W = NI.GetOutEW(e);
    if (this->NIdCmtyIdH.GetDat(DstNId) == OldCmtyId) {
      OldCmtyW -= W;
      // OldCmtyInW += W;
      OldCmtyOutW -= W;
      NewCmtyOutW += W;
    } else {
      OldCmtyOutW -= W;
      NewCmtyOutW += W;
      if (this->NIdCmtyIdH.GetDat(DstNId) == NewCmtyId) {
        // NewCmtyInW -= W;
        NewCmtyW += W;
      }
    }
  }
  OldCmtyInW -= NIdW;
  OldCmtyOutW -= NIdW;
  OldCmtyW -= NIdW;
  NewCmtyInW += NIdW;
  NewCmtyOutW += NIdW;
  NewCmtyW += NIdW;
  for (int e = 0; e < NI.GetInDeg(); e++) {
    int SrcNId = NI.GetInNId(e);
    TEdgeW W = NI.GetInEW(e);
    if (this->NIdCmtyIdH.GetDat(SrcNId) == OldCmtyId) {
      OldCmtyW -= W;
      // OldCmtyOutW += W;
      OldCmtyInW -= W;
      NewCmtyInW += W;
    } else {
      OldCmtyInW -= W;
      NewCmtyInW += W;
      if (this->NIdCmtyIdH.GetDat(SrcNId) == NewCmtyId) {
        // NewCmtyOutW -= W;
        NewCmtyW += W;
      }
    }
  }
  // compute new modularity
  double new_mod = OldCmtyW - ((double) OldCmtyOutW*OldCmtyInW) / ((double) this->TotalW)
                 + NewCmtyW - ((double) NewCmtyOutW*NewCmtyInW) / ((double) this->TotalW);
  // return normalized change in modularity
  return (new_mod - old_mod) / ((double) this->TotalW);
}

template <class TEdgeW>
double ModularityCommunity<TEdgeW>::quality() {
  typename TIntIntH::TIter HI;
  double mod = 0.0;
  int CmtyId;
  for (CmtyId = 0; CmtyId < this->NCmty; CmtyId++) {
    if (this->CmtySizeV[CmtyId] > 0) {
      TEdgeW CmtyW = this->CmtyWV[CmtyId];
      TEdgeW CmtyOutW = this->CmtyOutWV[CmtyId];
      TEdgeW CmtyInW = this->CmtyInWV[CmtyId];
      // compute modularity
      mod += CmtyW - ((double) CmtyOutW*CmtyInW) / ((double) this->TotalW);
    }
  }
  return mod / ((double) this->TotalW);
}

}

namespace TSnap {

template <class Community, class TEdgeW>
double LouvainMethod(const TPt<TWNGraph<TEdgeW> >& Graph, TIntIntVH& CmtyVH, const TEdgeDir& Dir, const double& eps, const double& delta, const int& max_iters) {
  
  printf("starting Louvain method\n");
  
  TPt<TWNGraph<TEdgeW> > CmtyGraphCopy, CmtyCmtyGraphCopy = TWNGraph<TEdgeW>::New();
  THash<TInt, TEdgeW> NIdWH;
  typename TWNGraph<TEdgeW>::TNodeI NI;
  TIntIntH::TIter HI;
  TIntIntVH::TIter VHI;
  TIntV::TIter VI;
  int min_moves = delta*Graph->GetNodes();
  
  int phases = 0;
  double phase_improv = 0.0;
  bool phase_init = true;
  
  CmtyGraphCopy = Graph; // pointer copy
  CmtyVH.Clr();
  for (NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    int NId = NI.GetId();
    CmtyVH.AddKey(NId);
    CmtyVH.GetDat(NId).Add(NId);
    NIdWH.AddDat(NId, 0);
  }
  
  Community Cmty(CmtyGraphCopy, NIdWH);
  
  printf("initial quality: %f\n", Cmty.quality());
  
  while (phase_init || phase_improv > eps) { // quality improving
    phase_init = false; // TODO: deprecate this with a single initial phase 1
    phase_improv = 0.0;
    
    printf("phase: %d\n", phases);
    
    // printf("graph edges:\n");
    // for (typename TWNGraph<TEdgeW>::TEdgeI EI = CmtyGraphCopy->BegEI(); EI < CmtyGraphCopy->EndEI(); EI++) {
    //   printf("(%d, %d) : %f\n", EI.GetSrcNId(), EI.GetDstNId(), (double) EI.GetW());
    // }
    
    // printf("communities:\n"); 
    // for (HI = Cmty.NIdCmtyIdH.BegI(); HI < Cmty.NIdCmtyIdH.EndI(); HI++) {
    //   printf("%d: %d (w: %f)\n", (int) HI.GetKey(), (int) HI.GetDat(), (double) Cmty.NIdWH.GetDat(HI.GetKey()));
    // }
    
    // printf("community statistics:\n");
    // for (int CmtyId = 0; CmtyId < Cmty.NCmty; CmtyId++) {
    //   printf("%d: (size: %d, in: %f, out: %f, w: %f\n", CmtyId, (int) Cmty.CmtySizeV[CmtyId], (double) Cmty.CmtyInWV[CmtyId], (double) Cmty.CmtyOutWV[CmtyId], (double) Cmty.CmtyWV[CmtyId]);
    // }
    
    // printf("quality: %f\n", Cmty.quality());
    
    // Phase 1 : MoveNode<Community>(Graph, Q, NId)
    // -------
    
    int moves = 0, iters = 0;
    double iter_improv = 0.0;
    bool iter_init = true;
    
    while (iter_init || (iter_improv > eps && moves > min_moves && iters < max_iters)) { // quality improving and parameters not exceeded
      iter_init = false; // TODO: deprecate this with an initial phase 1
      iter_improv = 0.0;
      
      printf("iter: %d\n", iters);
      
      // TODO: randomly permute nodes (?)
      
      for (NI = CmtyGraphCopy->BegNI(); NI < CmtyGraphCopy->EndNI(); NI++) { // nodes, randomly permuted
        
        double candidate_move_improv = 0.0, best_move_improv = 0.0;
        int NId = NI.GetId();
        int OldCmtyId, NewCmtyId, BestCmtyId = -1;
        
        // TODO: implement switching on (a) all, (b) neighbors, (c) random communities
        
        // Only iterate over the neighbors for the direction specified
        for (int e = 0; e < NI.GetDeg(Dir); e++) {
          OldCmtyId = Cmty.NIdCmtyIdH.GetDat(NId);
          NewCmtyId = Cmty.NIdCmtyIdH.GetDat(NI.GetNbrNId(e, Dir));
          if (NewCmtyId != OldCmtyId) {
            candidate_move_improv = Cmty.move_node_diff(NId, NewCmtyId);
            // printf("diff: %f\n", candidate_move_improv);
            if (candidate_move_improv > best_move_improv) {
              BestCmtyId = NewCmtyId;
              best_move_improv = candidate_move_improv;
            }
          }
        }
        
        if (best_move_improv > 0.0 && BestCmtyId != -1) {
          // printf("best diff: %f (%d: %d ->", best_move_improv, NId, (int) Cmty.NIdCmtyIdH.GetDat(NId));
          Cmty.move_node(NId, BestCmtyId);
          // printf(" %d: %d)\n", NId, (int) Cmty.NIdCmtyIdH.GetDat(NId));
          iter_improv += best_move_improv;
          moves++;
        }
        
      }
      
      iters++;
      
      printf("iter improv: %f (%f)\n", iter_improv, eps);
      // printf("moves: %d (%d)\n", moves, min_moves);
      // printf("iters: %d (%d)\n", iters, max_iters);
      
      phase_improv += iter_improv;
      
    }
    
    // -------
    
    printf("phase improv: %f\n", phase_improv);
    
    // printf("quality: %f\n", Cmty.quality());
    
    if (phase_improv == 0.0) {
      break; // break if not improved (as no singleton communities will have changed)
    }
    
    // printf("communities:\n"); 
    // for (HI = Cmty.NIdCmtyIdH.BegI(); HI < Cmty.NIdCmtyIdH.EndI(); HI++) {
    //   printf("%d: %d (w: %f)\n", (int) HI.GetKey(), (int) HI.GetDat(), (double) Cmty.NIdWH.GetDat(HI.GetKey()));
    // }
    
    // printf("community statistics:\n");
    // for (int CmtyId = 0; CmtyId < Cmty.NCmty; CmtyId++) {
    //   printf("%d: (size: %d, in: %f, out: %f, w: %f\n", CmtyId, (int) Cmty.CmtySizeV[CmtyId], (double) Cmty.CmtyInWV[CmtyId], (double) Cmty.CmtyOutWV[CmtyId], (double) Cmty.CmtyWV[CmtyId]);
    // }
    
    Cmty.clean_up();
    
    for (VHI = CmtyVH.BegI(); VHI < CmtyVH.EndI(); VHI++) {
      TIntV& CmtyV = VHI.GetDat();
      CmtyV.Add(Cmty.NIdCmtyIdH.GetDat(CmtyV[phases]));
    }
    
    // printf("communities:\n"); 
    // for (HI = Cmty.NIdCmtyIdH.BegI(); HI < Cmty.NIdCmtyIdH.EndI(); HI++) {
    //   printf("%d: %d (w: %f)\n", (int) HI.GetKey(), (int) HI.GetDat(), (double) Cmty.NIdWH.GetDat(HI.GetKey()));
    // }
    
    // printf("community statistics:\n");
    // for (int CmtyId = 0; CmtyId < Cmty.NCmty; CmtyId++) {
    //   printf("%d: (size: %d, in: %f, out: %f, w: %f\n", CmtyId, (int) Cmty.CmtySizeV[CmtyId], (double) Cmty.CmtyInWV[CmtyId], (double) Cmty.CmtyOutWV[CmtyId], (double) Cmty.CmtyWV[CmtyId]);
    // }
    
    printf("quality: %f\n", Cmty.quality());
    
    // Phase 2 : GetCmtyGraph(Graph, CmtyGraph, NIdCmtyIdH);
    // -------
    
    TSnap::GetCmtyGraph(CmtyGraphCopy, Cmty);
    
    // -------
    
    phases++;
    
  }
  
  // for (VHI = CmtyVH.BegI(); VHI < CmtyVH.EndI(); VHI++) {
  //   printf("%d: ", (int) VHI.GetKey());
  //   for (VI = VHI.GetDat().BegI(); VI < VHI.GetDat().EndI(); VI++) {
  //     printf("%d, ", VI->Val);
  //   }
  //   printf("END)\n");
  // }
  
  printf("number of communities: %d\n", Cmty.NCmty);
  
  return Cmty.quality();
  
}

} // namespace TSnap


#endif
