#ifndef core_io
#define core_io

// Headers (?)

// #include "Snap.h"

namespace TSnap {

//#//////////////////////////////////////////////
/// Generic SaveTxt methods

/// Generic SaveTxt and LoadTxt for TVec template class using GetStr()
template<class TVal>
void SaveTxt(const TVec<TVal>& GenV, const TStr& FNm, const TStr& Desc = TStr(), const TStr& ValNm = "Val") {
  typename TVec<TVal>::TIter VI;
  FILE *F = fopen(FNm.CStr(), "wt");
  if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
  fprintf(F, "# %ss:\t%d\n", ValNm.CStr(), GenV.Len());
  fprintf(F, "# %s\n", ValNm.CStr());
  for (VI = GenV.BegI(); VI < GenV.EndI(); VI++) {
    fprintf(F, "%s", VI->GetStr().CStr());
    fprintf(F, "\n");
  }
  fclose(F);
}

TIntV LoadTxtIntV(const TStr& FNm);
TFltV LoadTxtFltV(const TStr& FNm);

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

TIntIntH LoadTxtIntIntH(const TStr& FNm);
TIntFltH LoadTxtIntFltH(const TStr& FNm);

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

// TODO: implement LoadTxtIntIntVH and LoadTxtIntFltVH

/// Specialized SaveTxt for THash template class with TFlt values using GetStr()
template<class TKey>
void SaveTxt(const THash<TKey, TFlt>& GenH, const TStr& FNm, const TStr& Desc = TStr(), const TStr& KeyNm = "Key", const TStr& ValNm = "Val", const int& Width = -1, const int& Prec = -1) {
  typename THash<TKey, TFlt>::TIter GenI;
  FILE *F = fopen(FNm.CStr(), "wt");
  if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
  fprintf(F, "# %ss:\t%d\n", KeyNm.CStr(), GenH.Len());
  fprintf(F, "# %s\t%s\n", KeyNm.CStr(), ValNm.CStr());
  for (GenI = GenH.BegI(); GenI < GenH.EndI(); GenI++) {
    const TKey Key = GenI.GetKey();
    const TFlt Val = GenI.GetDat();
    fprintf(F, "%s\t%s", Key.GetStr().CStr(), TFlt::GetStr(Val, Width, Prec).CStr());
    fprintf(F, "\n");
  }
  fclose(F);
}

/// Specialized SaveTxt for THash template class with TFlt values using GetStr()
template<class TKey>
void SaveTxt(const THash<TKey, TFltV>& GenVH, const TStr& FNm, const TStr& Desc = TStr(), const TStr& KeyNm = "Key", const TStr& ValNm = "Val", const int& Width = -1, const int& Prec = -1) {
  typename THash<TKey, TFltV>::TIter GenVI;
  typename TFltV::TIter DI;
  FILE *F = fopen(FNm.CStr(), "wt");
  if (! Desc.Empty()) { fprintf(F, "# %s\n", Desc.CStr()); }
  fprintf(F, "# %ss:\t%d\n", KeyNm.CStr(), GenVH.Len());
  fprintf(F, "# %s\t%s\n", KeyNm.CStr(), ValNm.CStr());
  for (GenVI = GenVH.BegI(); GenVI < GenVH.EndI(); GenVI++) {
    const TKey Key = GenVI.GetKey();
    fprintf(F, "%s", Key.GetStr().CStr());
    const TFltV ValV = GenVI.GetDat();
    for (DI = ValV.BegI(); DI < ValV.EndI(); DI++) {
      fprintf(F, "\t%s", TFlt::GetStr(DI->Val, Width, Prec).CStr());
    }
    fprintf(F, "\n");
  }
  fclose(F);
}


} // namespace TSnap

//#//////////////////////////////////////////////
/// Specific SaveTxt methods

namespace TSnap {

// Specific SaveTxt for TIntV
// void SaveTxtTIntV(const TIntV& NIdV, const TStr& FNm, const TStr& Desc = TStr());

// Specific SaveTxt for TIntIntVH, TIntFltVH
// void SaveTxtTIntIntVH(const TIntIntVH& IntIntVH, const TStr& FNm, const TStr& Desc = TStr());
// void SaveTxtTIntFltVH(const TIntFltVH& IntFltVH, const TStr& FNm, const TStr& Desc = TStr());

// Specific SaveTxt for TStrFltH
// void SaveTxtTStrFltH(const TStrFltH& StrFltH, const TStr& FNm, const TStr& Desc = TStr());

/// Specialized SaveTxt for TFltV using GetStr()
void SaveTxt(const TFltV& GenV, const TStr& FNm, const TStr& Desc = TStr(), const TStr& ValNm = "Val", const int& Width = -1, const int& Prec = -1);

// Modified SaveTxt for TCnComV
void SaveTxt(const TCnComV& CnComV, const TStr& FNm, const TStr& Desc = TStr());

// Specific SaveTxt for TIntFltKdV
void SaveTxtTIntFltKdV(const TIntFltKdV& IntFltKdV, const TStr& FNm, const TStr& Desc = TStr(), const TStr& KeyNm = "Key", const TStr& ValNm = "Val");

} // namespace TSnap

//#//////////////////////////////////////////////
/// Summary methods

namespace TSnap {

void printValVSummary(const TFltV& ValV, const TStr& Desc = "ValV Summary\n------------");

template<class TVal>
void printDataVSummary(const TVec<TVal>& V, const TStr& Desc = "DataV Summary\n-------------") {
  typename TVec<TVal>::TIter VI;
  TFltV ValV;
  for (VI = V.BegI(); VI < V.EndI(); VI++) {
    ValV.Add((double) VI->Val);
  }
  printValVSummary(ValV, Desc);
}

template<class TKey, class TVal>
void printDataHSummary(const THash<TKey, TVal>& H, const TStr& Desc = "DataH Summary\n-------------") {
  typename THash<TKey, TVal>::TIter HI;
  TFltV ValV;
  for (HI = H.BegI(); HI < H.EndI(); HI++) {
    ValV.Add((double) HI.GetDat());
  }
  printValVSummary(ValV, Desc);
}

template<class TKey, class TVal>
void printCategoryVHSummary(const THash<TKey, TVec<TVal> >& VH, const TStr& Desc = "CategoryVH Summary\n------------------") {
  typename THash<TKey, TVec<TVal> >::TIter HI;
  TVec<TFltV> ValVV(VH[0].Len());
  for (HI = VH.BegI(); HI < VH.EndI(); HI++) {
    const TVec<TVal>& V = HI.GetDat();
    for (int i = 0; i < V.Len(); i++) {
      ValVV[i].Add((double) V[i]);
    }
  }
  for (int i = 0; i < ValVV.Len(); i++) {
    printf("\n(%d)", i);
    printValVSummary(ValVV[i], Desc);
  }
}

template<class TKey, class TVal>
void printDataVHSummary(const THash<TKey, TVec<TVal> >& VH, const TStr& Desc = "CategoryVH Summary\n------------------") {
  typename THash<TKey, TVec<TVal> >::TIter HI;
  typename TVec<TVal>::TIter VI;
  THash<TKey, TFltV> CategoryVH;
  TFltV ValV, CategoryV;
  for (HI = VH.BegI(); HI < VH.EndI(); HI++) {
    const TVec<TVal>& V = HI.GetDat();
    ValV.Clr(false);
    for (VI = V.BegI(); VI < V.EndI(); VI++) {
      ValV.Add((double) VI->Val);
    }
    TMom Summary(ValV);
    CategoryV.Clr(false);
    CategoryV.Add(Summary.GetMn());
    CategoryV.Add(Summary.GetMx());
    CategoryV.Add(Summary.GetQuart1());
    CategoryV.Add(Summary.GetMedian());
    CategoryV.Add(Summary.GetQuart3());
    CategoryV.Add(Summary.GetMean());
    CategoryV.Add(Summary.GetSDev());
    CategoryVH.AddDat(HI.GetKey(), CategoryV);
  }
  printf("\n%s\n", Desc.CStr());
  printf("\nKey\n---\n");
  printf("(0) Min\n");
  printf("(1) Max\n");
  printf("(2) Q1\n");
  printf("(3) Med\n");
  printf("(4) Q3\n");
  printf("(5) Mean\n");
  printf("(6) SDev\n");
  printCategoryVHSummary(CategoryVH, "");
}

template<class TVal>
void printDataV(const TVec<TVal>& GenV, const bool& NumberRows=false, const TStr& Desc = "DataV\n-----") {
  typename TVec<TVal>::TIter VI;
  printf("\n%s\n", Desc.CStr());
  int i = 0;
  for (VI = GenV.BegI(); VI < GenV.EndI(); VI++) {
    if (NumberRows) {
      printf("%i: ", i);
      i++;
    }
    printf("%s\n", VI->GetStr().CStr());
  }
}

template<class PGraph>
void printGraphSummary(const PGraph& Graph, const TStr& Desc = "Graph Summary\n-------------") {
  int Nodes = Graph->GetNodes();
  int Edges = Graph->GetEdges();
  double AvDeg = 2*double(Edges)/double(Nodes);
  double Density = double(Edges)/(pow(double(Nodes), 2) - 1);
  printf("\n%s\n", Desc.CStr());
  printf("Nodes: %d\n", Nodes);
  printf("Edges: %d\n", Edges);
  printf("AvDeg: %f\n", AvDeg);
  printf("Density: %f\n", Density);
}

} // namespace TSnap

#endif