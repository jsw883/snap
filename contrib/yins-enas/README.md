Yale Institute for Network Science - Economic Network Analysis Suite (YINS-ENAS)
--------------------------------------------------------------------------------

### Overview ###

*ENAS* is being developed by [YINS](http://yins.yale.edu/) as a comprehensive
suite of examples for analysing economic (or any) networks, notably weighted,
directed networks. As SNAP does not provide support for weighted networks, we
have fully implemented a weighted, directed network class as well as a fairly
complete set of weighted degree distributions, weighted centrality measures,
and auxiliary functions (reading, writing, etc...).

### Installation ###

Makefiles are provided for compilation under Windows with Cygwin and gcc, and
under Mac OS X, Linux, and other Unix operating systems with gcc as well. For
makefiles, compile the code with `make all`.

### Usage ###

The usage code provided in each example README.md use the weighted, directed
USairport2010 network which can be found in the datasets directory. The snap
edgelist for USairport2010 is actually included in the repository, as it is
not large (with 1574 nodes and 28236 edges). Two large amazon co-purchasing
networks can also be found in the datasets directory, with code to download
the edgelists, convert them to have unix style line endings, and add unitary
weights. These are larger (one with 262111 nodes and 1234877 edges, and the
other with 400727 nodes and 3200440 edges) and are useful for testing suite
runtimes.

### Structure ###

As there are now multiple examples that use the same code, all .h and .cpp
files have been moved out of the example folders and into the core directory,
to make development simpler and to maintain consistency. Eventually, this
should be merged into snap-core, after sufficient testing and documenting.

### Examples ###

##### centrality #####

Computes node centrality distributions: first degree, ~~1:k degree~~, first degree
centrality, eigenvector centrality, and PageRank centrality. All centrality
measures other than PageRank centrality are computed on both the directed and
undirected graphs (in / out / undirected), and PageRank centrality is computed
on the directed graph only, as it is derived. Here first degree centrality
is simply the first degree normalised by the number of nodes in the graph, as
weights are not considered.