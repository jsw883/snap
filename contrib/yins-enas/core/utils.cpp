// Headers (?)

#include "stdafx.h"
#include "utils.h"

//#//////////////////////////////////////////////
/// Colors

// Hex

void ConvertHexToRGB(const TStr& Hex, TFltTr& RGB) {
  RGB.Val1 = (double) Hex.GetSubStr(0, 1).GetHexInt() / 255;
  RGB.Val2 = (double) Hex.GetSubStr(2, 3).GetHexInt() / 255;
  RGB.Val3 = (double) Hex.GetSubStr(4, 5).GetHexInt() / 255;
}

void ConvertHexToRGB(const TStrV& HexV, TFltTrV& RGBV) {
  RGBV.Clr();
  for (int i = 0; i < HexV.Len(); i++) {
    RGBV.Add();
    ConvertHexToRGB(HexV[i], RGBV[i]);
  }
}

void ConvertRGBToHex(const TFltTr& RGB, TStr& Hex) {
  int R = RGB.Val1 * 255 + 0.5, G = RGB.Val2 * 255 + 0.5, B = RGB.Val3 * 255 + 0.5;
  Hex = "";
  Hex += (R < 16 ? "0" : "") + TInt::GetHexStr(R);
  Hex += (G < 16 ? "0" : "") + TInt::GetHexStr(G);
  Hex += (B < 16 ? "0" : "") + TInt::GetHexStr(B);
}

void ConvertRGBToHex(const TFltTrV& RGBV, TStrV& HexV) {
  HexV.Clr();
  for (int i = 0; i < RGBV.Len(); i++) {
    HexV.Add();
    ConvertRGBToHex(RGBV[i], HexV[i]);
  }
}

// HSV

void ConvertHSVToRGB(const TFltTr& HSV, TFltTr& RGB) {
  double H = HSV.Val1, S = HSV.Val2, V = HSV.Val3;
  double R = 0.0, G = 0.0, B = 0.0;
  if (S == 0.0) {
    R = V;
    G = V;
    B = V;
  } else {
    double l = V;
    double f = H - (double) floor(H);
    double p = V * (1.0 - S);
    double q = V * (1.0 - (S * f));
    double t = V * (1.0 - (S * (1.0 - f)));
    switch ((int) (f * 6.0)) {
      case 0:
        R = l; G = t; B = p;
        break;
      case 1:
        R = q; G = l; B = p;
        break;
      case 2:
        R = p; G = l; B = t;
        break;
      case 3:
        R = p; G = q; B = l;
        break;
      case 4:
        R = t; G = p; B = l;
        break;
      case 5:
        R = l; G = p; B = q;
        break;
    }
  }
  RGB.Val1 = R, RGB.Val2 = G, RGB.Val3 = B;
}

void ConvertHSVToRGB(const TFltTrV& HSVV, TFltTrV& RGBV) {
  RGBV.Clr();
  for (int i = 0; i < HSVV.Len(); i++) {
    RGBV.Add();
    ConvertHSVToRGB(HSVV[i], RGBV[i]);
  }
}

void ConvertHSVToRGB(TFltTrV& ColV) {
  TFltTr RGB;
  for (int i = 0; i < ColV.Len(); i++) {
    ConvertHSVToRGB(ColV[i], RGB);
    ColV[i] = RGB;
  }
}

void ConvertRGBToHSV(const TFltTr& RGB, TFltTr& HSV) {
  double R = RGB.Val1, G = RGB.Val2, B = RGB.Val3;
  double H, S, V;
  double cmin, cmax;
  cmax = (R > G) ? R : G;
  if (B > cmax) cmax = B;
  cmin = (R < G) ? R : G;
  if (B < cmin) cmin = B;
  V = cmax;
  if (cmax != 0.0) {
    S = 1.0 - cmin / cmax;
  } else {
    S = 0.0;
  }
  if (S == 0.0) {
    H = 0.0;
  } else {
    double RC = (cmax - R) / (cmax - cmin);
    double GC = (cmax - G) / (cmax - cmin);
    double BC = (cmax - B) / (cmax - cmin);
    if (R == cmax) {
      H = BC - GC;
    } else if (G == cmax) {
      H = 2.0 + RC - BC;
    } else {
      H = 4.0 + GC - RC;
    }
    H = H / 6.0;
    if (H < 0.0) {
      H = H + 1.0;
    }
  }
  HSV.Val1 = H, HSV.Val2 = S, HSV.Val3 = V;
}

void ConvertRGBToHSV(const TFltTrV& RGBV, TFltTrV& HSVV) {
  HSVV.Clr();
  for (int i = 0; i < HSVV.Len(); i++) {
    HSVV.Add();
    ConvertHSVToRGB(RGBV[i], HSVV[i]);
  }
}

void ConvertRGBToHSV(TFltTrV& ColV) {
  TFltTr HSV;
  for (int i = 0; i < ColV.Len(); i++) {
    ConvertRGBToHSV(ColV[i], HSV);
    ColV[i] = HSV;
  }
}

// HSL

double HToRGB(double V1, double V2, double H) {
  if (H < 0.0) H += 1.0;
  if (H > 1.0) H -= 1.0;
  if (6.0 * H < 1.0) {
    return V1 + (V2 - V1) * 6 * H;
  } else if (6.0 * H < 3.0) {
    return V2;
  } else if (6.0 * H < 4.0) {
    return V1 + (V2 - V1) * 6.0 * (2.0 / 3.0 - H);
  } else {
    return V1;
  }
}

void ConvertHSLToRGB(const TFltTr& HSL, TFltTr& RGB) {
  double H = HSL.Val1, S = HSL.Val2, L = HSL.Val3;
  double R, G, B;
  if (S == 0.0) {
    R = L;
    G = L;
    B = L;
  } else {
    double V1, V2;
    if (L < 0.5) {
      V2 = L * (1.0 + S);
    } else {
      V2 = (L + S) - (S * L);
    }
    V1 = 2 * L - V2;
    R = HToRGB(V1, V2, H + (1.0 / 3.0));
    G = HToRGB(V1, V2, H);
    B = HToRGB(V1, V2, H - (1.0 / 3.0));
  }
  RGB.Val1 = R, RGB.Val2 = G, RGB.Val3 = B;
}

void ConvertHSLToRGB(const TFltTrV& HSLV, TFltTrV& RGBV) {
  RGBV.Clr();
  for (int i = 0; i < HSLV.Len(); i++) {
    RGBV.Add();
    ConvertHSLToRGB(HSLV[i], RGBV[i]);
  }
}

template <class Array>
void ConvertHSLToRGB(Array& ColArray) {
  TFltTr RGB;
  for (int i = 0; i < ColArray.Len(); i++) {
    ConvertHSLToRGB(ColArray[i], RGB);
    ColArray[i] = RGB;
  }
}

void ConvertRGBToHSL(const TFltTr& RGB, TFltTr& HSL) {
  double R = RGB.Val1, G = RGB.Val2, B = RGB.Val3;
  double H, S, L;
  double var_min, var_max, dmax;
  var_min = std::min(std::min(R, G), B);
  var_max = std::max(std::max(R, G), B);
  dmax = var_max - var_min;
  L = (var_max + var_min) / 2.0;
  if (dmax == 0.0) {
     H = 0.0;
     S = 0.0;
  } else {
    if (L < 0.5) {
      S = dmax / (var_max + var_min);
    } else {
      S = dmax / (2.0 - var_max - var_min);
    }
    double dR = (((var_max - R) / 6.0) + (dmax / 2.0)) / dmax;
    double dG = (((var_max - G) / 6.0) + (dmax / 2.0)) / dmax;
    double dB = (((var_max - B) / 6.0) + (dmax / 2.0)) / dmax;
    if (R == var_max) {
      H = dB - dG;
    } else if (G == var_max) {
      H = (1.0 / 3.0) + dR - dB;
    } else {
      H = (2.0 / 3.0) + dG - dR;
    }
    if (H < 0) H += 1.0;
    if (H > 1) H -= 1.0;
  }
  HSL.Val1 = H, HSL.Val2 = S, HSL.Val3 = L;
}

void ConvertRGBToHSL(const TFltTrV& RGBV, TFltTrV& HSLV) {
  HSLV.Clr();
  for (int i = 0; i < RGBV.Len(); i++) {
    HSLV.Add();
    ConvertRGBToHSL(RGBV[i], HSLV[i]);
  }
}

void ConvertRGBToHSL(TFltTrV& ColV) {
  TFltTr HSL;
  for (int i = 0; i < ColV.Len(); i++) {
    ConvertRGBToHSL(ColV[i], HSL);
    ColV[i] = HSL;
  }
}

// Gen

void GenHSLBasedRGB(const int& N, const double& S, const double& L, TFltTrV& ColV, double HDiff) {
  ColV.Clr();
  ColV.Reserve(N);
  if (HDiff == 0.0) HDiff = 1.0 / N;
  double H = 0;
  for (int i = 0; i < N; i++) {
    ColV.Add(TFltTr(H, S, L));
    H += HDiff;
  }
  ConvertHSLToRGB(ColV);

}

void GenHSLBasedRGB(const int& N, const double& S, const double& L, TIntFltTrH& ColH, double HDiff) {
  ColH.Clr();
  ColH.Gen(N);
  if (HDiff == 0.0) HDiff = 1.0 / N;
  double H = 0;
  for (int i = 0; i < N; i++) {
    ColH.AddDat(i, TFltTr(H, S, L));
    H += HDiff;
  }
  ConvertHSLToRGB(ColH);
}
