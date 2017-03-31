#
# configuration variables for the example
#

MAIN = drawing

EXCXXFLAGS = -std=c++11

DEPH = \
    $(EXCORE)/wgraph.h \
    $(EXCORE)/wgio.h \
    $(EXCORE)/wcmty.h \
    $(EXCORE)/deg.h \
    $(EXCORE)/utils.h \
    $(EXCORE)/io.h \
    $(EXCORE)/layout.h
    

DEPCPP = \
    $(EXCORE)/wgraph.cpp \
    $(EXCORE)/wgio.cpp \
    $(EXCORE)/wcmty.cpp \
    $(EXCORE)/deg.cpp \
    $(EXCORE)/utils.cpp \
    $(EXCORE)/io.cpp \
    $(EXCORE)/layout.cpp

LIBS = \
    -I/usr/include/cairomm-1.0 \
    -I/usr/lib/cairomm-1.0/include \
    -I/usr/include/cairo \
	-I/usr/lib/sigc++-2.0/include \
    -I/usr/lib/glib-2.0/include \
	\
    -I/usr/local/include/cairomm-1.0 \
    -I/usr/local/lib/cairomm-1.0/include \
    -I/usr/local/include/cairo \
    -I/usr/include/sigc++-2.0 \
    -I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include \
    -I/usr/include/glib-2.0 \
    -I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
    -I/usr/include/pixman-1 \
    -I/usr/include/freetype2 \
    -I/usr/include/libpng12 \
    -L/usr/local/lib \
    -lcairomm-1.0 \
    -lcairo \
    -lsigc-2.0
