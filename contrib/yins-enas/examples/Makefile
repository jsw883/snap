#
# Makefile for non-Microsoft compilers
#

all: make-examples

make-examples:
	$(MAKE) -C centrality
	$(MAKE) -C community
	$(MAKE) -C connectivity
	$(MAKE) -C egonets
	$(MAKE) -C nullmodels
	$(MAKE) -C statistics
	$(MAKE) -C structures
	$(MAKE) -C subsets
	$(MAKE) -C vespignani
	$(MAKE) -C wcentrality
	$(MAKE) -C wdistance
	$(MAKE) -C wstatistics

clean:
	$(MAKE) clean -C centrality
	$(MAKE) clean -C community
	$(MAKE) clean -C connectivity
	$(MAKE) clean -C egonets
	$(MAKE) clean -C nullmodels
	$(MAKE) clean -C statistics
	$(MAKE) clean -C structures
	$(MAKE) clean -C subsets
	$(MAKE) clean -C vespignani
	$(MAKE) clean -C wcentrality
	$(MAKE) clean -C wdistance
	$(MAKE) clean -C wstatistics
