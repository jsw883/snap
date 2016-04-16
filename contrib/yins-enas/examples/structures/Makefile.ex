#
#	configuration variables for the example
#

MAIN = structures
DEPH = \
    $(EXCORE)/galg.h \
    $(EXCORE)/galgfixedmemory.h \
    $(EXCORE)/stats.h \
    $(EXCORE)/percolate.h \
    $(EXCORE)/io.h \
    $(EXCORE)/utils.h
DEPCPP = \
    $(EXCORE)/galg.cpp \
    $(EXCORE)/galgfixedmemory.cpp \
    $(EXCORE)/stats.cpp \
    $(EXCORE)/percolate.cpp \
    $(EXCORE)/io.cpp \
    $(EXCORE)/utils.cpp