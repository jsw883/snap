#
#	configuration variables for the example
#

MAIN = statistics
DEPH = \
    $(EXCORE)/galg.h \
    $(EXCORE)/galgfixedmemory.h \
    $(EXCORE)/stats.h \
    $(EXCORE)/io.h \
    $(EXCORE)/deg.h
DEPCPP = \
    $(EXCORE)/galg.cpp \
    $(EXCORE)/galgfixedmemory.cpp \
    $(EXCORE)/stats.cpp \
    $(EXCORE)/io.cpp \
    $(EXCORE)/deg.cpp