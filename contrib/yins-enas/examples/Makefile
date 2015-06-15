#
# Makefile for non-Microsoft compilers
#

all: make-examples

make-examples:
	$(MAKE) -C centrality
	$(MAKE) -C connectivity
	$(MAKE) -C statistics
	$(MAKE) -C wcentrality
	$(MAKE) -C wstatistics

clean:
	$(MAKE) clean -C centrality
	$(MAKE) clean -C connectivity
	$(MAKE) clean -C statistics
	$(MAKE) clean -C wcentrality
	$(MAKE) clean -C wstatistics
