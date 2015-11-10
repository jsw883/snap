#ifndef core_rand
#define core_rand

// Headers (?)

// #include "Snap.h"

namespace TSnap {

template<class TVal>
TVec<TVal> GetRndSubV(const TVec<TVal>& ValV, const int& size, TRnd& Rnd) {
  TVec<TVal> SubV(ValV);
  SubV.Shuffle(Rnd);
  SubV.Trunc(size);
  return SubV;
}

} // namespace TSnap

#endif