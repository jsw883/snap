centrality (vertex distributions)
---------------------------------

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

Note that these centrality measures are not weighted, as weighted graphs need
to be properly implemented in SNAP. For weighted centrality versions of these
measures, see wcentrality, which uses a new implementation of weighted graphs
instead.

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./centrality -i:<input graph> -o:<output prefix> [Options]
Options:
    -i          input graph (tab separated list of edges)
    -o          output prefix (filename extensions added)
    -k          depth of degree traversal (default: 1)
    --collate   collate properties into matrix: T / F (default: F)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=../../datasets/USairport2010
rm -rf $DATASET/centrality
mkdir $DATASET/centrality
./centrality -i:$DATASET/USairport2010.snap \
          -o:$DATASET/centrality/USairport2010 \
          --collate:T
```