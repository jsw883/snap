// Headers (?)

#include "stdafx.h"
#include "utils.h"

#include <sstream>

void ConvertHexToRGB(const TStr& Hex, TFltTr& RGB) {
  
  int i;
  
  std::istringstream(Hex.GetSubStr(0, 1).CStr()) >> std::hex >> i; RGB.Val1 = (double) i / 255;
  std::istringstream(Hex.GetSubStr(2, 3).CStr()) >> std::hex >> i; RGB.Val2 = (double) i / 255;
  std::istringstream(Hex.GetSubStr(4, 5).CStr()) >> std::hex >> i; RGB.Val3 = (double) i / 255;
  
}