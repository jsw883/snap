Yale Institute for Network Science - Economic Network Analysis Suite (YINS-ENAS)
--------------------------------------------------------------------------------

### Overview ###

*ENAS* is being developed by [YINS](http://yins.yale.edu/) as a comprehensive
suite of examples for analysing economic (or any) networks, notably weighted,
directed networks. As SNAP does not provide support for weighted networks, we
have fully implemented a weighted, directed network class as well as a fairly
complete set of weighted degree distributions, weighted centrality measures,
and auxiliary functions (reading, writing, etc...).

### Building ###

Makefiles are provided for compilation under Windows with Cygwin and gcc, and
under Mac OS X, Linux, and other Unix operating systems with gcc as well. For
makefiles, compile the code with `make all`, either from the root [examples](examples)
directory or in each example directory.

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

#### centrality (vertex distributions) ####

Computes node centrality distributions: first degree, 1:k degree, first degree
centrality, eigenvector centrality, and PageRank centrality. All centrality
measures other than PageRank centrality are computed on both the directed and
undirected graphs (in / out / undirected), and PageRank centrality is computed
on the directed graph only, as it is derived. Here first degree centrality
is simply the first degree normalised by the number of nodes in the graph, as
weights are not considered.

Node centrality measures / distributions (in / out / undirected):

  - first degrees
  - 1:k degrees
  - first degree centrality
  - eigenvector centrality

Other centrality measures:

  - pagerank centrality

#### connectivity (bowtie structure) ####

Computes connectivity structures, namely weakly connected components (WCCS),
strongly connected cores (SCCS), components connecting in to the SCCS
(INS), components connecting out from the SCCS (OUTS), and tendrils from the
INS and to the OUTS (TES).

Connectivity structures (giant only):

  - giant weakly connected component (GWCC)
  - giant strongly connected component (GSCC)
  - components connecting in to the GSCC (IN)
  - components connected out from the GSCC (OUT)
  - tendrils from the IN and to the OUT (TE)

Connectivity structures (each weak):

  - weakly connected components (WCCS)
  - strongly connected cores (SCCS)
  - components connecting in to the SCCS (INS)
  - components connected out from the SCCS (OUTS)
  - tendrils from the INS and to the OUTS (TES)

Note that for each weak the SCCS, INS, OUTS, and TES are for each weakly
connected component and as such the output files should have the same
number of connected components. Hence instead of obtaining all strongly
connected components, merely the strongly connected core of each weakly
connected component is obtained.

#### egonets (properties) ####

Computes egonet properties at increasing depths: number of nodes, number
of edges, edge density, GINI coefficient (concentration), weight of edges
(bag of money), distribution of weights between horizontal and vertical
edges (edges between nodes the same distance from the ego and edges to or
from the ego), and egonet dominant eigenvalues (in / out / undirected).

  - number of nodes
  - number of edges
  - edge density
  - GINI coefficient (concentration)
  - weight of edges (bag of money / horizontal / vertical)
  - egonet dominant eigenvalues (in / out / undirected)

Here horizontal edges are defined as edges between nodes that are the same
distance from the ego, and vertical edges are defined as edges that are on
paths leading to or coming from the ego). Currently, for efficiency, some
edges that are not on a directed path are also included, as vertical edges
are implemented as simply not horizontal edges (avoiding having "floating"
edges). As this is also computationally expensive, a parameter is provided
to enable / disable this (which avoids numerous subgraph invocations).

Note that the GINI coefficient is computed on the egonet as a child of the
parent graph, such that edges into and out of the egonet are considered in
the calculation of "income" or weighted in degree.

For now, dominant eigenvalues are computed using the in / out / undirected
egonets (using in / out / undirected edge traversal for each node) however
this is almost certainly duplicate as it is equivalent to transposing the
weighted adjacency matrix (or symmetrising).

#### statistics (small world properties) ####

Computes network statistics / small world properties: nodes, edges, average
degree, density, maximum degree, diameter, average path length, average and
global clustering coefficients / transitivity.

Statistics:

  - nodes |V|, edges |E|, average degree 2*|E|/|V|, density |E|/|V|^2
  - maximum degree (in / out / undirected)
  - diameter (effective / approximate)
  - average path length
  - clustering coefficient (average / global)

Curves:

  - approximate neighbourhood function / shortest path cumulative density

Note that diameter and average path length are found using the approximate
neighbourhood function / shortest path cumulative density as described in
the paper [ANF: A Fast and Scalable Tool for Data Mining in Massive Graphs](http://www.cs.cmu.edu/~christos/PUBLICATIONS/kdd02-anf.pdf)
by C. R. Palmer, P. B. Gibbons, and C. Faloutsos and implemented in SNAP.

#### wcentrality (weighted vertex distributions) ####

Computes weighted node centrality distributions using the new implementation
of weighted graphs: weighted first degree, weighted 1:k degree, weighted degree
centrality, eigenvector centrality, and PageRank centrality. All centrality
measures other than PageRank centrality are computed on both the directed and
undirected graphs (in / out / undirected), and PageRank centrality is computed
on the directed graph only, as it is formulated.

Weighted node centrality measures / distributions (in / out / undirected):

  - weighted first degrees
  - weighted 1:k degrees
  - weighted degree centrality
  - eigenvector centrality

Other centrality measures:

  - pagerank centrality

Here the weighted first degree centrality takes into account both number and
weight of edges using the measure proposed by Opsahl, Agneessens, and Skvoretz
in [Node centrality in weighted networks, 2010](http://ac.els-cdn.com/S0378873310000183/1-s2.0-S0378873310000183-main.pdf?_tid=10ae60fc-7cb3-11e4-ab20-00000aab0f6c&acdnat=1417807104_e3b5a51625a02033cdc37883b5069258).

#### wstatistics (wegihted small world properties) ####

Computes weighted network statistics / small world properties: nodes, edges,
sum of weights, average weighted degree, and maximum weighted degree.

Statistics:

  - nodes |V|, edges |E|, sum of weights, average weighted degree
  - maximum weighted degree (in / out / undirected)
