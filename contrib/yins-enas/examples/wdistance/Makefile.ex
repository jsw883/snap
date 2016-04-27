#
#	configuration variables for the example
#

MAIN = wdistance
DEPH = \
    $(EXCORE)/wgraph.h \
    $(EXCORE)/wgio.h \
    $(EXCORE)/galg.h \
    $(EXCORE)/galgfixedmemory.h \
    $(EXCORE)/wd.h \
    $(EXCORE)/stats.h \
    $(EXCORE)/io.h \
    $(EXCORE)/rand.h
DEPCPP = \
    $(EXCORE)/wgraph.cpp \
    $(EXCORE)/wgio.cpp \
    $(EXCORE)/galg.cpp \
    $(EXCORE)/galgfixedmemory.cpp \
    $(EXCORE)/wd.cpp \
    $(EXCORE)/stats.cpp \
    $(EXCORE)/io.cpp \
    $(EXCORE)/rand.cpp