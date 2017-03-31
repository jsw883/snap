#
# configuration variables for the example
#

MAIN = colors

EXCXXFLAGS = -std=c++11

DEPH = \
    $(EXCORE)/utils.h \
    $(EXCORE)/io.h
    

DEPCPP = \
    $(EXCORE)/utils.cpp \
    $(EXCORE)/io.cpp
