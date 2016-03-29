#
# configuration variables for the example
#

MAIN = wsubsets
DEPH = \
    $(EXCORE)/wgraph.h \
    $(EXCORE)/wgio.h \
    $(EXCORE)/io.h \
    $(EXCORE)/rand.h
#include "io.h"
DEPCPP = \
    $(EXCORE)/wgraph.cpp \
    $(EXCORE)/wgio.cpp \
    $(EXCORE)/io.cpp \
    $(EXCORE)/rand.cpp
