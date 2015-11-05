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
	wgraph-tests.cpp

TEST_OBJS = $(TEST_SRCS:.cpp=.o)

all: $(MAIN)

run: $(MAIN)
	./$(MAIN)

$(MAIN): $(MAIN).cpp $(TEST_SRCS) $(CSNAP)/Snap.o $(wildcard $(CCORE)/*)
	$(CC) $(CXXFLAGS) $(CXXOPENMP) -o $(MAIN) $(MAIN).cpp $(TEST_SRCS) $(CSNAP)/Snap.o -I$(CSNAP) -I$(CGLIB) -I$(CCORE) $(LIBS)

$(CSNAP)/Snap.o:
	$(MAKE) -C $(CSNAP)

clean:
	rm -f *.o $(MAIN) $(MAIN).exe
	rm -rf Debug Release
	rm -rf test*.dat *.Err

