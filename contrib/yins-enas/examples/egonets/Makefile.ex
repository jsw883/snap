#
#	  configuration variables for the example
#

MAIN = egonets
DEPH = \
    $(EXCORE)/wgraph.h \
    $(EXCORE)/wgio.h \
    $(EXCORE)/wego.h \
    $(EXCORE)/io.h \
    $(EXCORE)/enums.h \
    $(EXCORE)/utils.h
    
DEPCPP = \
    $(EXCORE)/wgraph.cpp \
    $(EXCORE)/wgio.cpp \
    $(EXCORE)/wego.cpp \
    $(EXCORE)/io.cpp \
    $(EXCORE)/enums.cpp \
    $(EXCORE)/utils.cpp