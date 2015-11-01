#
#	configuration variables for the example
#

MAIN = wcentrality
DEPH = \
    $(EXCORE)/wgraph.h \
    $(EXCORE)/wgio.h \
    $(EXCORE)/wdeg.h \
    $(EXCORE)/wcentr.h \
    $(EXCORE)/io.h
DEPCPP = \
    $(EXCORE)/wgraph.cpp \
    $(EXCORE)/wgio.cpp \
    $(EXCORE)/wdeg.cpp \
    $(EXCORE)/wcentr.cpp \
    $(EXCORE)/io.cpp