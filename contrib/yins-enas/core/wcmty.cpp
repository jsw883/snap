// Headers (?)

#include "stdafx.h"
#include "wcmty.h"

/// Community heirarchy summary method
void TSnap::CmtyHierarchySummary(const TIntIntVH& CmtyVH, const int& PhaseThreshold, const int& NCmtyThreshold, const TStr& Desc) {
  TIntIntVH::TIter HI;
  TIntV::TIter VI;
  TIntV CmtySizeV;
    
  int CmtyId, NCmty;
  int phase, phases = CmtyVH.BegI().GetDat().Len();
  
  printf("%s\n", Desc.CStr());
  
  for (phase = PhaseThreshold; phase < phases; phase++) {
    TIntIntH CmtySizeH;
    NCmty = 0;
    for (HI = CmtyVH.BegI(); HI < CmtyVH.EndI(); HI++) {
      CmtyId = HI.GetDat()[phase];
      if (CmtySizeH.IsKey(CmtyId)) {
        CmtySizeH.GetDat(CmtyId)++;
      } else {
        CmtySizeH.AddDat(CmtyId, 1);
      }
      NCmty = NCmty > CmtyId + 1 ? NCmty : CmtyId + 1;
    }
    printf("\nPhase %d (communities: %d, maximum size: %d)\n---\n", phase, NCmty, (int) CmtySizeH.GetDat(0));
    if (NCmtyThreshold == -1 || NCmty < NCmtyThreshold) {
      for (CmtyId = 0; CmtyId < NCmty; CmtyId++) {
        printf("%d: %d\n", CmtyId, (int) CmtySizeH.GetDat(CmtyId));
      }
    }
  }
  // if (CmtyVH.Len() < NCmtyThreshold) {
  //   printf("Summary:\n");
  //   for (HI = CmtyVH.BegI(); HI < CmtyVH.EndI(); HI++) {
  //     printf("%d:", (int) HI.GetKey());
  //     for (VI = HI.GetDat().BegI(); VI < HI.GetDat().EndI(); VI++) {
  //       printf(" %d", VI->Val);
  //     }
  //     printf("\n");
  //   }
  // }
  printf("\n");
}
