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

## Main application file
MAIN = run-core-tests

TEST_SRCS = \
	wgraph-tests.cpp

TEST_OBJS = $(TEST_SRCS:.cpp=.o)

all: $(MAIN)
run: coretests

# COMPILE
.cpp.o:
	$(CC) $(CXXFLAGS) -I$(CSNAP) -I$(CGLIB) -I$(CCORE) $(LDFLAGS) $(LIBS) -c $<

$(MAIN): $(MAIN).o $(TEST_OBJS) $(CSNAP)/Snap.o 
	$(CC) $(CXXFLAGS) -o $(MAIN) $^ -I$(CSNAP) -I$(CGLIB) $(LIBS)

$(CSNAP)/Snap.o:
	$(MAKE) -C $(CSNAP)

coretests: $(MAIN)
	./$(MAIN)

clean:
	rm -f *.o $(MAIN) $(MAIN).exe
	rm -rf Debug Release
	rm -rf test*.dat *.Err

