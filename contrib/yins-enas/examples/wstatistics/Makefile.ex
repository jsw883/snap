#
#	configuration variables for the example
#

MAIN = wstatistics
DEPH = \
    $(EXCORE)/wgraph.h \
    $(EXCORE)/wgio.h \
    $(EXCORE)/wstats.h \
    $(EXCORE)/io.h
DEPCPP = \
    $(EXCORE)/wgraph.cpp \
    $(EXCORE)/wgio.cpp \
    $(EXCORE)/wstats.cpp \
    $(EXCORE)/io.cpp