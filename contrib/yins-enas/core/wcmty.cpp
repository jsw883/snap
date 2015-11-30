// Headers (?)

#include "stdafx.h"
#include "wcmty.h"

/// Community heirarchy summary method
void TSnap::CmtyHierarchySummary(const TIntIntVH& CmtyVH, const int& NCmtyThreshold) {
  TIntIntVH::TIter HI;
  TIntV::TIter VI;
  TIntV CmtySizeV;
    
  int CmtyId, NCmty;
  int phase, phases = CmtyVH.BegI().GetDat().Len();
  
  printf("\nCommunity hierarchy:\n");
  for (phase = 0; phase < phases; phase++) {
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
    printf("Phase %d (communities: %d, maximum size: %d)\n", phase, NCmty, (int) CmtySizeH.GetDat(0));
    if (NCmty < NCmtyThreshold) {
      for (CmtyId = 0; CmtyId < NCmty; CmtyId++) {
        printf("%d: %d\n", CmtyId, (int) CmtySizeH.GetDat(CmtyId));
      }
    }
  }
  if (CmtyVH.Len() < NCmtyThreshold) {
    printf("Summary:\n");
    for (HI = CmtyVH.BegI(); HI < CmtyVH.EndI(); HI++) {
      printf("%d:", (int) HI.GetKey());
      for (VI = HI.GetDat().BegI(); VI < HI.GetDat().EndI(); VI++) {
        printf("\t%d", VI->Val);
      }
      printf("\n");
    }
  }
  printf("\n");
}
