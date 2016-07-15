#ifndef core_utils
#define core_utils

// Headers (?)

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

void ConvertHexToRGB(const TStr& Hex, TFltTr& RGB);
void ConvertHexToRGB(const TStrV& HexV, TFltTrV& RGBV);

template <class TKey> void ConvertHexToRGB(const THash<TKey, TStr>& HexH, THash<TKey, FltTr>& RGBH);

template <class TKey>
void ConvertHexToRGB(const THash<TKey, TStr>& HexH, THash<TKey, FltTr>& RGBH) {
  THash<TKey, TStr>::TIter HI;
  RGBH.Clr();
  for (HI = HexH.BegI(); HI < HexH.EndI(); HI++) {
    TFltTr RGB& RGBH.AddDat(HI.GetKey());
    ConvertHexToRGB(HI.GetDat(), RGB);
  }
}

#endif