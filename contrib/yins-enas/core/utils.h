#ifndef core_utils
#define core_utils

// Headers (?)

class Progress {
private:
  TExeTm ExeTm;
  int stepsCompleted, stepsRequired, percentageCompleted, percentageThreshold;
  bool showPercentage;
public:
  Progress(const TExeTm& ExeTm, const int& stepsRequired, const int& percentageThreshold, const char *Message = "Computing", const bool& showPercentage = false) : ExeTm(ExeTm), stepsCompleted(0), stepsRequired(stepsRequired), percentageCompleted(0), percentageThreshold(percentageThreshold), showPercentage(showPercentage) {
    setup(Message);
  }
  void setup(const char *Message) {
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

#endif