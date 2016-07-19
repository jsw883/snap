// Headers (?)

#include "stdafx.h"
#include "utils.h"

#include <sstream>

// void ConvertRGBToHex(const TFltTr& RGB, TStr& Hex) {
//   Hex.Clr();
//   int i;
//   i = (int) RGB.Val1 * 255; std::istringstream(Hex.CStr()) << std::hex << i;
//   // i = (int) RGB.Val2 * 255; std::istringstream(Hex.GetSubStr(2, 3).CStr()) << std::hex << i;
//   // i = (int) RGB.Val3 * 255; std::istringstream(Hex.GetSubStr(4, 5).CStr()) << std::hex << i;
// }

void ConvertHexToRGB(const TStr& Hex, TFltTr& RGB) {
  int i;
  std::istringstream(Hex.GetSubStr(0, 1).CStr()) >> std::hex >> i; RGB.Val1 = (double) i / 255;
  std::istringstream(Hex.GetSubStr(2, 3).CStr()) >> std::hex >> i; RGB.Val2 = (double) i / 255;
  std::istringstream(Hex.GetSubStr(4, 5).CStr()) >> std::hex >> i; RGB.Val3 = (double) i / 255;

  // RGB((double) Hex.GetSubStr(0, 1).GetHexInt() / 255, );
}

void ConvertHexToRGB(const TStrV& HexV, TFltTrV& RGBV) {
  RGBV.Clr();
  int i;
  for (i = 0; i < HexV.Len(); i++) {
    RGBV.Add();
    ConvertHexToRGB(HexV[i], RGBV[i]);
  }
}
