#ifndef core_utils
#define core_utils

// Headers (?)

#include <cmath>
#include <algorithm>

//#//////////////////////////////////////////////
/// Progress

class Progress {
private:
  TExeTm ExeTm;
  int stepsCompleted, stepsRequired, percentageCompleted, percentageThreshold;
  bool showPercentage;
  const char *Message;
public:
  Progress(const TExeTm& ExeTm, const int& stepsRequired, const int& percentageThreshold, const char *Message = "Computing", const bool& showPercentage = true) : ExeTm(ExeTm), stepsCompleted(0), stepsRequired(stepsRequired), percentageCompleted(0), percentageThreshold(percentageThreshold), showPercentage(showPercentage), Message(Message) {
    // start(Message);
  }
  void start() {
    printf("\n----------------------------------------\n");
    printf("%s", Message);
    printf("\n----------------------------------------\n");
    if (showPercentage) {
      display();
    }
  }
  Progress& operator++ () { step(1); return *this; }
  Progress& operator++ (int) { step(1); return *this; }
  void step(const int& steps) {
    stepsCompleted += steps;
    int newPercentageCompleted = floor(double(stepsCompleted) / double(stepsRequired) * 100);
    if (newPercentageCompleted >= percentageCompleted + percentageThreshold) {
      percentageCompleted = newPercentageCompleted;
      if (showPercentage) {
        display();
      }
    }
    if (percentageCompleted == 100) {
      done();
    }
  }
  void display() {
    printf("%03d%% : %s (%s)\n", (percentageCompleted / percentageThreshold) * percentageThreshold, ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  }
  void done() {
    printf("----------------------------------------\n");
  }
};

//#//////////////////////////////////////////////
/// Colors

// Hex

void ConvertHexToRGB(const TStr& Hex, TFltTr& RGB);
void ConvertHexToRGB(const TStrV& HexV, TFltTrV& RGBV);

void ConvertRGBToHex(const TFltTr& RGB, TStr& Hex);
void ConvertRGBToHex(const TFltTrV& RGBV, TStrV& HexV);

template <class TKey> void ConvertHexToRGB(const THash<TKey, TStr>& HexH, THash<TKey, TFltTr>& RGBH);

template <class TKey>
void ConvertHexToRGB(const THash<TKey, TStr>& HexH, THash<TKey, TFltTr>& RGBH) {
  typename THash<TKey, TStr>::TIter HI;
  RGBH.Clr();
  for (HI = HexH.BegI(); HI < HexH.EndI(); HI++) {
    TFltTr& RGB = RGBH.AddDat(HI.GetKey());
    ConvertHexToRGB(HI.GetDat(), RGB);
  }
}

template <class TKey> void ConvertRGBToHex(const THash<TKey, TFltTr>& RGBH, THash<TKey, TStr>& HexH);

template <class TKey>
void ConvertRGBToHex(const THash<TKey, TFltTr>& RGBH, THash<TKey, TStr>& HexH) {
  typename THash<TKey, TFltTr>::TIter HI;
  HexH.Clr();
  for (HI = RGBH.BegI(); HI < RGBH.EndI(); HI++) {
    TStr& Hex = HexH.AddDat(HI.GetKey());
    ConvertRGBToHex(HI.GetDat(), Hex);
  }
}

// HSV

void ConvertHSVToRGB(const TFltTr& HSV, TFltTr& RGB);
void ConvertHSVToRGB(const TFltTrV& HSVV, TFltTrV& RGBV);
void ConvertHSVToRGB(TFltTrV& ColV);

void ConvertRGBToHSV(const TFltTr& RGB, TFltTr& HSV);
void ConvertRGBToHSV(const TFltTrV& RGBV, TFltTrV& HSVV);
void ConvertRGBToHSV(TFltTrV& ColV);

// HSL

void ConvertHSLToRGB(const TFltTr& HSL, TFltTr& RGB);
void ConvertHSLToRGB(const TFltTrV& HSLV, TFltTrV& RGBV);
template <class Array> void ConvertHSLToRGB(Array& ColArray);

void ConvertRGBToHSL(const TFltTr& RGB, TFltTr& HSL);
void ConvertRGBToHSL(const TFltTrV& RGBV, TFltTrV& HSLV);
void ConvertRGBToHSL(TFltTrV& ColV);

// Gen

void GenHSLBasedRGB(const int& N, const double& S, const double& L, TFltTrV& ColV, double HDiff = 0.0);
void GenHSLBasedRGB(const int& N, const double& S, const double& L, TIntFltTrH& ColH, double HDiff = 0.0);

#endif