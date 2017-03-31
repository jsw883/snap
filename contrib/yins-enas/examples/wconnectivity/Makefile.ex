#
#	configuration variables for the example
#

MAIN = wconnectivity
DEPH = \
    $(EXCORE)/wgraph.h \
    $(EXCORE)/wgio.h \
    $(EXCORE)/wgalgfixedmemory.h \
    $(EXCORE)/wd.h \
    $(EXCORE)/stats.h \
    $(EXCORE)/io.h \
    $(EXCORE)/rand.h
DEPCPP = \
    $(EXCORE)/wgraph.cpp \
    $(EXCORE)/wgio.cpp \
    $(EXCORE)/wgalgfixedmemory.cpp \
    $(EXCORE)/wd.cpp \
    $(EXCORE)/stats.cpp \
    $(EXCORE)/io.cpp \
    $(EXCORE)/rand.cpp