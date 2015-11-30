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
  
  printf("Community hierarchy:\n");
  for (phase = 0; phase < phases; phase++) {
    printf("Phase %d:\n", phase);
    TIntIntH CmtySizeH;
    NCmty = 0;
    for (HI = CmtyVH.BegI(); HI < CmtyVH.EndI(); HI++) {
      CmtyId = HI.GetDat()[phase];
      if (CmtySizeH.IsKey(CmtyId)) {
        CmtySizeH.GetDat(CmtyId)++;
      } else {
        CmtySizeH.AddDat(CmtyId, 1);
      }
      // CmtySizeV[CmtyId]++;
      NCmty = NCmty > CmtyId + 1 ? NCmty : CmtyId + 1;
    }
    if (NCmty < NCmtyThreshold) {
      printf("Community sizes:\n");
      for (CmtyId = 0; CmtyId < NCmty; CmtyId++) {
        printf("%d: %f\n", CmtyId, (double) CmtySizeH.GetDat(CmtyId));
      }
    }
    printf("Number of communities: %d\n", NCmty);
  }
}
