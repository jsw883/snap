#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Colors. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  
  // Config
  
  const int n = Env.GetIfArgPrefixInt("-n:", 30, "size of color scale");
  const TStr type = Env.GetIfArgPrefixStr("--type:", "quantitative", "type of color scale (quantitative)");
  
  // Variables
  
  TStr Name;

  TFltTr Col1(0.0, 0.0, 0.0), Col2(1.0, 0.5, 0.5), Col3(0.5, 0.0, 0.0);
  
  TStr Hex1, Hex2, Hex3;
  TFltTr RGB1, RGB2, RGB3;
  TFltTr HSV1, HSV2, HSV3;
  TFltTr HSL1, HSL2, HSL3;

  // Hex

  ConvertRGBToHex(Col1, Hex1);
  ConvertRGBToHex(Col2, Hex2);
  ConvertRGBToHex(Col3, Hex3);

  printf("\n");
  printf("RGB(%f, %f, %f) --> Hex(%s)\n", (double) Col1.Val1, (double) Col1.Val2, (double) Col1.Val3, Hex1.CStr());
  printf("RGB(%f, %f, %f) --> Hex(%s)\n", (double) Col2.Val1, (double) Col2.Val2, (double) Col2.Val3, Hex2.CStr());
  printf("RGB(%f, %f, %f) --> Hex(%s)\n", (double) Col3.Val1, (double) Col3.Val2, (double) Col3.Val3, Hex3.CStr());
  
  ConvertHexToRGB(Hex1, RGB1);
  ConvertHexToRGB(Hex2, RGB2);
  ConvertHexToRGB(Hex3, RGB3);

  printf("\n");
  printf("Hex(%s) --> RGB(%f, %f, %f)\n", Hex1.CStr(), (double) RGB1.Val1, (double) RGB1.Val2, (double) RGB1.Val3);
  printf("Hex(%s) --> RGB(%f, %f, %f)\n", Hex2.CStr(), (double) RGB2.Val1, (double) RGB2.Val2, (double) RGB2.Val3);
  printf("Hex(%s) --> RGB(%f, %f, %f)\n", Hex3.CStr(), (double) RGB3.Val1, (double) RGB3.Val2, (double) RGB3.Val3);
  
  // HSV

  ConvertRGBToHSV(Col1, HSV1);
  ConvertRGBToHSV(Col2, HSV2);
  ConvertRGBToHSV(Col3, HSV3);

  printf("\n");
  printf("RGB(%f, %f, %f) --> HSV(%f, %f, %f)\n", (double) Col1.Val1, (double) Col1.Val2, (double) Col1.Val3, (double) HSV1.Val1, (double) HSV1.Val2, (double) HSV1.Val3);
  printf("RGB(%f, %f, %f) --> HSV(%f, %f, %f)\n", (double) Col2.Val1, (double) Col2.Val2, (double) Col2.Val3, (double) HSV2.Val1, (double) HSV2.Val2, (double) HSV2.Val3);
  printf("RGB(%f, %f, %f) --> HSV(%f, %f, %f)\n", (double) Col3.Val1, (double) Col3.Val2, (double) Col3.Val3, (double) HSV3.Val1, (double) HSV3.Val2, (double) HSV3.Val3);
  
  ConvertHSVToRGB(HSV1, RGB1);
  ConvertHSVToRGB(HSV2, RGB2);
  ConvertHSVToRGB(HSV3, RGB3);

  printf("\n");
  printf("HSV(%f, %f, %f) --> RGB(%f, %f, %f)\n", (double) HSV1.Val1, (double) HSV1.Val2, (double) HSV1.Val3, (double) RGB1.Val1, (double) RGB1.Val2, (double) RGB1.Val3);
  printf("HSV(%f, %f, %f) --> RGB(%f, %f, %f)\n", (double) HSV2.Val1, (double) HSV2.Val2, (double) HSV2.Val3, (double) RGB2.Val1, (double) RGB2.Val2, (double) RGB2.Val3);
  printf("HSV(%f, %f, %f) --> RGB(%f, %f, %f)\n", (double) HSV3.Val1, (double) HSV3.Val2, (double) HSV3.Val3, (double) RGB3.Val1, (double) RGB3.Val2, (double) RGB3.Val3);
  
  // HSL

  ConvertRGBToHSL(Col1, HSL1);
  ConvertRGBToHSL(Col2, HSL2);
  ConvertRGBToHSL(Col3, HSL3);

  printf("\n");
  printf("RGB(%f, %f, %f) --> HSL(%f, %f, %f)\n", (double) Col1.Val1, (double) Col1.Val2, (double) Col1.Val3, (double) HSL1.Val1, (double) HSL1.Val2, (double) HSL1.Val3);
  printf("RGB(%f, %f, %f) --> HSL(%f, %f, %f)\n", (double) Col2.Val1, (double) Col2.Val2, (double) Col2.Val3, (double) HSL2.Val1, (double) HSL2.Val2, (double) HSL2.Val3);
  printf("RGB(%f, %f, %f) --> HSL(%f, %f, %f)\n", (double) Col3.Val1, (double) Col3.Val2, (double) Col3.Val3, (double) HSL3.Val1, (double) HSL3.Val2, (double) HSL3.Val3);
  
  ConvertHSLToRGB(HSL1, RGB1);
  ConvertHSLToRGB(HSL2, RGB2);
  ConvertHSLToRGB(HSL3, RGB3);

  printf("\n");
  printf("HSL(%f, %f, %f) --> RGB(%f, %f, %f)\n", (double) HSL1.Val1, (double) HSL1.Val2, (double) HSL1.Val3, (double) RGB1.Val1, (double) RGB1.Val2, (double) RGB1.Val3);
  printf("HSL(%f, %f, %f) --> RGB(%f, %f, %f)\n", (double) HSL2.Val1, (double) HSL2.Val2, (double) HSL2.Val3, (double) RGB2.Val1, (double) RGB2.Val2, (double) RGB2.Val3);
  printf("HSL(%f, %f, %f) --> RGB(%f, %f, %f)\n", (double) HSL3.Val1, (double) HSL3.Val2, (double) HSL3.Val3, (double) RGB3.Val1, (double) RGB3.Val2, (double) RGB3.Val3);
  
  if (type == "quantitative") {

  } else {
    IAssertR(false, "Type must be \"quantitative\".");
  }

  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
} 