#include "stdafx.h"

int main(int argc, char* argv[]) {
  
  setbuf(stdout, NULL); // disables the buffer so that print statements are not buffered and display immediately (?)
  
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("Colors. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
  
  TExeTm ExeTm;
  
  Try
  
  const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "", "output prefix (filename extensions added)");
  
  // Config
  
  const int N = Env.GetIfArgPrefixInt("-n:", 30, "size of color scale");
  const double S = Env.GetIfArgPrefixFlt("-s:", 1.0, "saturation value (0.0 - 1.0)");
  const double L = Env.GetIfArgPrefixFlt("-l:", 0.5, "lightness value (0.0 - 1.0)");
  const double HDiff = Env.GetIfArgPrefixFlt("--hdiff:", 0.0, "hue diff (0 - 360) (default: inferred by -n to use the hue scale exhaustively)") / 360;
  
  // Variables
  
  TStr Name;

  TIntFltTrH RGBH;
  TStr Hex;
  
  // Colors

  GenHSLBasedRGB(N, S, L, RGBH, HDiff);

  printf("\nGenHSLBasedRGB(%d, %f, %f, RGBV)\n", N, S, L);
  for (int i = 0; i < RGBH.Len(); i++) {
    const TInt& Key = RGBH.GetKey(i);
    const TFltTr& RGB = RGBH[i];
    ConvertRGBToHex(RGB, Hex);
    printf("%d: (%f, %f, %f) [%s]\n", (int) Key, (double) RGB.Val1, (double) RGB.Val2, (double) RGB.Val3, Hex.CStr());
  }

  Catch
  
  printf("\nTotal run time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  return 0;
  
} 