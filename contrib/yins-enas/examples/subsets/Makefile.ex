#
#	configuration variables for the example
#

MAIN = subsets
DEPH = \
    $(EXCORE)/galg.h \
    $(EXCORE)/galgfixedmemory.h \
    $(EXCORE)/galgconnectivity.h \
    $(EXCORE)/stats.h \
    $(EXCORE)/io.h \
    $(EXCORE)/rand.h
DEPCPP = \
    $(EXCORE)/galg.cpp \
    $(EXCORE)/galgfixedmemory.cpp \
    $(EXCORE)/galgconnectivity.cpp \
    $(EXCORE)/stats.cpp \
    $(EXCORE)/io.cpp \
    $(EXCORE)/rand.cpp