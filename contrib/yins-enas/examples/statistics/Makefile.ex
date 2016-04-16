#
#	configuration variables for the example
#

MAIN = statistics
DEPH = \
    $(EXCORE)/galg.h \
    $(EXCORE)/galgfixedmemory.h \
    $(EXCORE)/stats.h \
    $(EXCORE)/io.h
DEPCPP = \
    $(EXCORE)/galg.cpp \
    $(EXCORE)/galgfixedmemory.cpp \
    $(EXCORE)/stats.cpp \
    $(EXCORE)/io.cpp