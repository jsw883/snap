// Headers (?)

#include "stdafx.h"
#include "cncomnew.h"

void TSnap::printCnComVSummary(const TCnComV& CnComV, const TStr& Desc) {
  typename TCnComV::TIter VI;
  printf("\n%s\n", Desc.CStr());
  int i = 0;
  for (VI = CnComV.BegI(); VI < CnComV.EndI(); VI++) {
    printf("%d: %d\n", i, VI->Len());
    i++;
  }
}
