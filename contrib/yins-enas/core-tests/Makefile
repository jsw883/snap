#
# Makefile for non-Microsoft compilers
#
# Google Test 1.7.0 (should update this to the newer build process):
#	- download: https://code.google.com/p/googletest/
# - build it the old way: ./configure && make
# - expose the static library: sudo cp -r include/gtest /usr/include
# - expose the headers: sudo cp -r lib/.lib/* /usr/lib
#
# Compile and run all the tests:
#	make run
#

include ../../../Makefile.config
include ../Makefile.config

LIBS += -lpthread -lgtest

# Test config

MAIN = run-core-tests

TEST_SRCS = \
	stats-tests.cpp \
	wstats-tests.cpp \
	wgraph-tests.cpp \
	wdeg-tests.cpp \
	wcmty-tests.cpp

TEST_OBJS = $(TEST_SRCS:.cpp=.o)

DEPH = \
    $(CCORE)/wgraph.h \
	$(CCORE)/galg.h \
	$(CCORE)/stats.h \
    $(CCORE)/wgio.h \
    $(CCORE)/wdeg.h \
    $(CCORE)/wcmty.h \
	$(CCORE)/wstats.h
DEPCPP = \
    $(CCORE)/wgraph.cpp \
	$(CCORE)/galg.cpp \
	$(CCORE)/stats.cpp \
    $(CCORE)/wgio.cpp \
    $(CCORE)/wdeg.cpp \
    $(CCORE)/wcmty.cpp \
	$(CCORE)/wstats.cpp

all: $(MAIN)

run: $(MAIN)
	./$(MAIN)

$(MAIN): $(MAIN).cpp $(TEST_SRCS) $(DEPH) $(CSNAP)/Snap.o $(wildcard $(CCORE)/*)
	$(CC) -g $(CXXFLAGS) $(CXXOPENMP) -o $(MAIN) $(MAIN).cpp $(TEST_SRCS) $(DEPCPP) $(CSNAP)/Snap.o -I$(CSNAP) -I$(CGLIB) -I$(CCORE) $(LIBS)

$(CSNAP)/Snap.o:
	$(MAKE) -C $(CSNAP)

clean:
	rm -f *.o $(MAIN) $(MAIN).exe
	rm -rf Debug Release
	rm -rf *.dat *.Err

