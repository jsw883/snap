#ifndef yins_io_h
#define yins_io_h

// Headers (?)

#include "Snap.h"

namespace TSnap {

/// Generic SaveTxt for TVec template class using GetStr()
template<class TVal>
void SaveTxt(const TVec<TVal>& GenV, const TStr& FNm, const TStr& Desc = TStr(), const TStr& ValNm = "Val") {
  typename TVec<TVal>::TIter VI;
  FILE *F = fopen(FNm.CStr(), "wt");
  if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
  fprintf(F, "# %ss:\t%d\n", ValNm.CStr(), GenV.Len());
  fprintf(F, "# %s\n", ValNm.CStr());
  for (VI = GenV.BegI(); VI < GenV.EndI(); VI++) {
    fprintf(F, "%s", VI->Val.GetStr().CStr());
    fprintf(F, "\n");
  }
  fclose(F);
}

/// Generic SaveTxt for THash template class using GetStr()
template<class TKey, class TVal>
void SaveTxt(const THash<TKey, TVal>& GenH, const TStr& FNm, const TStr& Desc = TStr(), const TStr& KeyNm = "Key", const TStr& ValNm = "Val") {
  typename THash<TKey, TVal>::TIter GenI;
  FILE *F = fopen(FNm.CStr(), "wt");
  if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
  fprintf(F, "# %ss:\t%d\n", KeyNm.CStr(), GenH.Len());
  fprintf(F, "# %s\t%s\n", KeyNm.CStr(), ValNm.CStr());
  for (GenI = GenH.BegI(); GenI < GenH.EndI(); GenI++) {
    const TKey Key = GenI.GetKey();
    const TVal Val = GenI.GetDat();
    fprintf(F, "%s\t%s", Key.GetStr().CStr(), Val.GetStr().CStr());
    fprintf(F, "\n");
  }
  fclose(F);
}

/// Generic SaveTxt for THash template class with TVec data using GetStr()
template<class TKey, class TVal>
void SaveTxt(const THash<TKey, TVec<TVal> >& GenVH, const TStr& FNm, const TStr& Desc = TStr(), const TStr& KeyNm = "Key", const TStr& ValNm = "Val") {
  typename THash<TKey, TVec<TVal> >::TIter GenVI;
  typename TVec<TVal>::TIter DI;
  FILE *F = fopen(FNm.CStr(), "wt");
  if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
  fprintf(F, "# %ss:\t%d\n", KeyNm.CStr(), GenVH.Len());
  fprintf(F, "# %s\t%s\n", KeyNm.CStr(), ValNm.CStr());
  for (GenVI = GenVH.BegI(); GenVI < GenVH.EndI(); GenVI++) {
    const TKey Key = GenVI.GetKey();
    fprintf(F, "%s", Key.GetStr().CStr());
    const TVec<TVal> ValV = GenVI.GetDat();
    for (DI = ValV.BegI(); DI < ValV.EndI(); DI++) {
      fprintf(F, "\t%s", DI->GetStr().CStr());
    }
    fprintf(F, "\n");
  }
  fclose(F);
}

} // namespace TSnap

namespace TSnap {

// Specific SaveTxt for TIntV
// void SaveTxtTIntV(const TIntV& NIdV, const TStr& FNm, const TStr& Desc = TStr());

// Specific SaveTxt for TIntIntVH, TIntFltVH
// void SaveTxtTIntIntVH(const TIntIntVH& IntIntVH, const TStr& FNm, const TStr& Desc = TStr());
// void SaveTxtTIntFltVH(const TIntFltVH& IntFltVH, const TStr& FNm, const TStr& Desc = TStr());

// Specific SaveTxt for TStrFltH, TIntFltKdV
// void SaveTxtTStrFltH(const TStrFltH& StrFltH, const TStr& FNm, const TStr& Desc = TStr());
// void SaveTxtTIntFltKdV(const TIntFltKdV& IntFltKdV, const TStr& FNm, const TStr& Desc = TStr());

// Modified SaveTxt for TCnComV
void SaveTxt(const TCnComV& CnComV, const TStr& FNm, const TStr& Desc = TStr());

} // namespace TSnap

#endif