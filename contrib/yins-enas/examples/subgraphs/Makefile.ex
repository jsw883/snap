#
#	  configuration variables for the example
#

MAIN = subgraphs
DEPH = \
    $(EXCORE)/wgraph.h \
    $(EXCORE)/ego.h \
    $(EXCORE)/deg.h \
    $(EXCORE)/wdeg.h \
    $(EXCORE)/wgio.h \
    $(EXCORE)/io.h \
    $(EXCORE)/enums.h \
    $(EXCORE)/utils.h
    
DEPCPP = \
    $(EXCORE)/wgraph.cpp \
    $(EXCORE)/ego.cpp \
    $(EXCORE)/deg.cpp \
    $(EXCORE)/wdeg.cpp \
    $(EXCORE)/wgio.cpp \
    $(EXCORE)/io.cpp \
    $(EXCORE)/enums.cpp \
    $(EXCORE)/utils.cpp