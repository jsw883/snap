#
# Makefile for non-Microsoft compilers
#

all: make-examples

make-examples:
	$(MAKE) -C centrality
	# $(MAKE) -C community
	$(MAKE) -C connectivity
	$(MAKE) -C diameters
	$(MAKE) -C egonets
	$(MAKE) -C statistics
	$(MAKE) -C wcentrality
	$(MAKE) -C wstatistics
	$(MAKE) -C vespignani

clean:
	$(MAKE) clean -C centrality
	# $(MAKE) clean -C community
	$(MAKE) clean -C connectivity
	$(MAKE) clean -C diameters
	$(MAKE) clean -C egonets
	$(MAKE) clean -C statistics
	$(MAKE) clean -C wcentrality
	$(MAKE) clean -C wstatistics
	$(MAKE) clean -C vespignani
