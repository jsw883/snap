#
# Makefile for non-Microsoft compilers
#

all: make-examples

make-examples:
	$(MAKE) -C centrality
	$(MAKE) -C connectivity
	$(MAKE) -C drawing
	$(MAKE) -C egonets
	$(MAKE) -C generators
	$(MAKE) -C statistics
	$(MAKE) -C structures
	$(MAKE) -C subgraphs
	$(MAKE) -C subsets
	$(MAKE) -C vespignani
	$(MAKE) -C wcentrality
	$(MAKE) -C wcommunity
	$(MAKE) -C wdistance
	$(MAKE) -C wstatistics

clean:
	$(MAKE) clean -C centrality
	$(MAKE) clean -C connectivity
	$(MAKE) clean -C drawing
	$(MAKE) clean -C egonets
	$(MAKE) clean -C generators
	$(MAKE) clean -C statistics
	$(MAKE) clean -C structures
	$(MAKE) clean -C subgraphs
	$(MAKE) clean -C subsets
	$(MAKE) clean -C vespignani
	$(MAKE) clean -C wcentrality
	$(MAKE) clean -C wcommunity
	$(MAKE) clean -C wdistance
	$(MAKE) clean -C wstatistics
