// Headers (?)

#include "stdafx.h"
#include "galg.h"

void GetWccStats(const TIntPrV& WccSzCnt, int& numWcc, int& mnWccSz, double& medWccSz, double& meanWccSz, int& mxWccSz) {
  TIntPrV::TIter WccSzCntI;
  int numNodes = 0;
  numWcc = 0;
  for (WccSzCntI = WccSzCnt.BegI(); WccSzCntI < WccSzCnt.EndI(); WccSzCntI++) {
    numNodes += WccSzCntI->Val1 * WccSzCntI->Val2;
    numWcc += WccSzCntI->Val2;
  }
  int medWcc = 0;
  for (WccSzCntI = WccSzCnt.BegI(); WccSzCntI < WccSzCnt.EndI(); WccSzCntI++) {
    medWcc += WccSzCntI->Val2;
    if (medWcc >= (numWcc + 1) / 2) {
      if (medWcc == (numWcc + 1) / 2 && numWcc % 2 == 0) {
        medWccSz = WccSzCntI->Val1; WccSzCntI++; medWccSz += WccSzCntI->Val1;
        medWccSz /= 2;
      } else {
        medWccSz = WccSzCntI->Val1;
      }
      break;
    }
  }
  mnWccSz = WccSzCnt[0].Val1;
  meanWccSz = (double) numNodes / (double) numWcc;
  mxWccSz = WccSzCnt.Last().Val1;
}
