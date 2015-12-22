centrality (vertex distributions)
---------------------------------

Computes node centrality distributions: first degree, 1:k degree, first degree
centrality, eigenvector centrality, and PageRank centrality. All centrality
measures other than PageRank centrality are computed on both the directed and
undirected graphs (in / out / undirected), and PageRank centrality is computed
on the directed graph only, as it is derived. Here first degree centrality
is simply the first degree normalised by the number of nodes in the graph, as
weights are not considered.

Alpha centrality has been added, as defined by Bonacich and Lloyd (2001). This
measure of centrality is better for asymmetric graphs, and comes from the same
family as eigenvector centrality. Also, it is equivalent to Katz centrality
when no exogenous source of centrality is provided, as the default source of 1
gives Katz centrality.

Node centrality measures / distributions (in / out / undirected):

  - first degrees
  - 1:k degrees
  - first degree centrality
  - eigenvector centrality
  - alpha centrality

Other centrality measures:

  - PageRank centrality (directed)

Note that these centrality measures are not weighted, as weighted graphs need
to be properly implemented in SNAP. For weighted centrality versions of these
measures, see wcentrality, which uses a new implementation of weighted graphs
instead.

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./centrality -i:<input network> -o:<output prefix> [Options]
Options:
    -i          input network (tab separated list of edges)
    -e          exogenous source of centrality (tab separated node mapping)
                    (default: 1)
    -o          output prefix (filename extensions added)
    -k          depth of degree traversal (default: 1)
    -c          personalization parameter for PageRank (default: 0.85)
    -a          endogenous parameter for alpha centrality (default: 1)
    --eps       precision for power method convergence (default: 1.0e-4)
    --iters     maximum number of iterations (default 1.0e+3)
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
          -e:$DATASET/exogenous.status.TIntFltH \
          -o:$DATASET/centrality/USairport2010 \
          -k:3 -c:0.85 -a:0.5 \
          --eps:1.0e-5 --iters:1e+4 \
          --collate:T
```