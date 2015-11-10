#ifndef core_utils
#define core_utils

// Headers (?)

class Progress {
private:
  TExeTm ExeTm;
  int stepsCompleted, stepsRequired, percentageCompleted, percentageThreshold;
public:
  Progress(const TExeTm& ExeTm, const int& stepsRequired, const int& percentageThreshold, const char *Message = "Computing") : ExeTm(ExeTm), stepsCompleted(0), stepsRequired(stepsRequired), percentageCompleted(0), percentageThreshold(percentageThreshold) {
    setup(Message);
  }
  void setup(const char *Message) {
    printf("\n----------------------------------------\n");
    printf(Message);
    printf("\n----------------------------------------\n");
    display();
  }
  Progress& operator++ () { step(1); return *this; }
  Progress& operator++ (int) { step(1); return *this; }
  void step(const int& steps) {
    stepsCompleted += steps;
    int percentage = floor(double(stepsCompleted) / double(stepsRequired) * 100);
    if (percentage >= percentageCompleted + percentageThreshold) {
      percentageCompleted = percentageCompleted + percentageThreshold;
      display();
    }
    if (percentageCompleted == 100) {
      done();
    }
  }
  void display() {
    printf("%03d%% : %s (%s)\n", percentageCompleted, ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
  }
  void done() {
    printf("----------------------------------------\n");
  }
};

#endif