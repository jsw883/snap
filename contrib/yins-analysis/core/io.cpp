// Headers (?)

#include "io.h"

// Specific SaveTxt for TIntV
// void TSnap::SaveTxtTIntV(const TIntV& NIdV, const TStr& FNm, const TStr& Desc) {
//   typename TIntV::TIter VI;
//   FILE *F = fopen(FNm.CStr(), "wt");
//   if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
//   fprintf(F, "# Nodes:\t%d\n", NIdV.Len());
//   fprintf(F, "# NodeId\n");
//   for (VI = NIdV.BegI(); VI < NIdV.EndI(); VI++) {
//     fprintf(F, "%d", VI->Val);
//     fprintf(F, "\n");
//   }
//   fclose(F);
// }

// Specific SaveTxt for TIntIntVH
// void TSnap::SaveTxtTIntIntVH(const TIntIntVH& IntIntVH, const TStr& FNm, const TStr& Desc) {
//   typename TIntIntVH::TIter IntIntVI;
//   typename TIntV::TIter DI;
//   FILE *F = fopen(FNm.CStr(), "wt");
//   if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
//   fprintf(F, "# Nodes:\t%d\n", IntIntVH.Len());
//   fprintf(F, "# NodeId\tValV\n");
//   for (IntIntVI = IntIntVH.BegI(); IntIntVI < IntIntVH.EndI(); IntIntVI++) {
//     const int NId = IntIntVI.GetKey();
//     fprintf(F, "%d", NId);
//     const TIntV ValV = IntIntVI.GetDat();
//     for (DI = ValV.BegI(); DI < ValV.EndI(); DI++) {
//       fprintf(F, "\t%d", DI->Val);
//     }
//     fprintf(F, "\n");
//   }
//   fclose(F);
// }

// Specific SaveTxt for TIntFltVH
// void TSnap::SaveTxtTIntFltVH(const TIntFltVH& IntFltVH, const TStr& FNm, const TStr& Desc) {
//   typename TIntFltVH::TIter IntFltVI;
//   typename TFltV::TIter DI;
//   FILE *F = fopen(FNm.CStr(), "wt");
//   if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
//   fprintf(F, "# Nodes:\t%d\n", IntFltVH.Len());
//   fprintf(F, "# NodeId\tValV\n");
//   for (IntFltVI = IntFltVH.BegI(); IntFltVI < IntFltVH.EndI(); IntFltVI++) {
//     const int NId = IntFltVI.GetKey();
//     fprintf(F, "%d", NId);
//     const TFltV ValV = IntFltVI.GetDat();
//     for (DI = ValV.BegI(); DI < ValV.EndI(); DI++) {
//       fprintf(F, "\t%f", DI->Val);
//     }
//     fprintf(F, "\n");
//   }
//   fclose(F);
// }

// Specific SaveTxt for TStrFltH
// void TSnap::SaveTxtTStrFltH(const TStrFltH& StrFltH, const TStr& FNm, const TStr& Desc) {
//   typename TStrFltH::TIter StrFltI;
//   FILE *F = fopen(FNm.CStr(), "wt");
//   if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
//   fprintf(F, "# Keys:\t%d\n", StrFltH.Len());
//   fprintf(F, "# Key\tVal\n");
//   for (StrFltI = StrFltH.BegI(); StrFltI < StrFltH.EndI(); StrFltI++) {
//     const double Val = StrFltI.GetDat();
//     fprintf(F, "%s\t%f", StrFltI.GetKey().CStr(), Val);
//     fprintf(F, "\n");
//   }
//   fclose(F);
// }

// Specific SaveTxt for TIntFltKdV
// void TSnap::SaveTxtTIntFltKdV(const TIntFltKdV& IntFltKdV, const TStr& FNm, const TStr& Desc) {
//   typename TIntFltKdV::TIter IntFltKdI;
//   FILE *F = fopen(FNm.CStr(), "wt");
//   if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
//   fprintf(F, "# Keys:\t%d\n", IntFltKdV.Len());
//   fprintf(F, "# Key\tVal\n");
//   for (IntFltKdI = IntFltKdV.BegI(); IntFltKdI < IntFltKdV.EndI(); IntFltKdI++) {
//     const int Key = IntFltKdI->Key;
//     const int Val = IntFltKdI->Dat;
//     fprintf(F, "%d\t%d", Key, Val);
//     fprintf(F, "\n");
//   }
//   fclose(F);
// }

// Modified SaveTxt for TCnComV
void TSnap::SaveTxt(const TCnComV& CnComV, const TStr& FNm, const TStr& Desc) {
  typename TIntV::TIter VI;
  int nodes = 0;
  for (int c = 0; c < CnComV.Len(); c++) { nodes += CnComV[c].NIdV.Len(); }
  FILE *F = fopen(FNm.CStr(), "wt");
  if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
  fprintf(F, "# Components:\t%d\n", CnComV.Len());
  fprintf(F, "# Nodes:\t%d\n", nodes);
  fprintf(F, "# NodeId\tCnComId\n");
  for (int c = 0; c < CnComV.Len(); c++) {
    for (VI = CnComV[c].NIdV.BegI(); VI < CnComV[c].NIdV.EndI(); VI++) {
      fprintf(F, "%d\t%d", VI->Val, c);
      fprintf(F, "\n");
    }
  }
  fclose(F);
}

namespace TSnap {

} // namespace TSnap
