centrality (vertex distributions)
---------------------------------

Computes node centrality distributions: first degree, ~~1:k degree~~, first degree
centrality, eigenvector centrality, and PageRank centrality. All centrality
measures other than PageRank centrality are computed on both the directed and
undirected graphs (in / out / undirected), and PageRank centrality is computed
on the directed graph only, as it is derived. Here first degree centrality
is simply the first degree normalised by the number of nodes in the graph, as
weights are not considered.

Node centrality measures / distributions (in / out / undirected):

  - first degrees
  - ~~1:k degrees~~
  - first degree centrality
  - eigenvector centrality

Other centrality measures:

  - pagerank centrality

Note that these centrality measures are not weighted, as weighted graphs need
to be properly implemented in SNAP. ~~For weighted centrality versions of these
measures, see wcentrality, which uses a new implementation of weighted graphs
instead.~~

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Parameters ###

  - -i:input graph (tab separated list of edges)
  - -o:output prefix (filename extensions added)
  - ~~-k:depth of degree distributions (1 / 2 / ...)~~
  - -c:collate centralities into matrix (T / F)

### Usage ###

```bash
rm -r ../../datasets/USairport2010/USairport2010/centrality
mkdir ../../datasets/USairport2010/USairport2010/centrality
centrality -i:../../datasets/USairport2010/USairport2010.snap -o:../../datasets/USairport2010/centrality/USairport2010 -c:F
```