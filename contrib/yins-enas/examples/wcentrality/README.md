wcentrality (weighted vertex distributions)
-------------------------------------------

Computes weighted node centrality distributions using the new implementation
of weighted graphs: weighted first degree, weighted 1:k degree, weighted degree
centrality, eigenvector centrality, and PageRank centrality. All centrality
measures other than PageRank centrality are computed on both the directed and
undirected graphs (in / out / undirected), and PageRank centrality is computed
on the directed graph only, as it is formulated.

Alpha centrality has been added, as defined by Bonacich and Lloyd (2001). This
measure of centrality is better for asymmetric graphs, and comes from the same
family as eigenvector centrality. Also, it is equivalent to Katz centrality
when no exogenous source of centrality is provided, as the default source of 1
gives Katz centrality. For the algorithm to converge, alpha must be strictly 
less than the inverse of the leading eigenvalue of the graph adjacency matrix,
which is displayed in the example log file for reference (run once, check the
log, and run again with a new alpha value as needed).

Weighted node centrality measures / distributions (in / out / undirected):

  - weighted first degrees
  - weighted 1:k degrees
  - weighted degree centrality
  - eigenvector centrality
  - alpha centrality

Other centrality measures:

  - pagerank centrality

Here the weighted first degree centrality takes into account both number and
weight of edges using the measure proposed by Opsahl, Agneessens, and Skvoretz
in [Node centrality in weighted networks, 2010](http://ac.els-cdn.com/S0378873310000183/1-s2.0-S0378873310000183-main.pdf?_tid=10ae60fc-7cb3-11e4-ab20-00000aab0f6c&acdnat=1417807104_e3b5a51625a02033cdc37883b5069258).

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./wcentrality -i:<input network> -o:<output prefix> [Options]
Options:
    -i          input network (tab separated list of edges)
    -e          exogenous source of centrality (tab separated node mapping)
                    (default: 1)
    -o          output prefix (filename extensions added)
    -k          depth of degree traversal (default: 1)
    -c          personalization parameter for PageRank (default: 0.85)
    -r          ratio of endogenous parameter to dominant eigenvalue
                    (alpha centrality) (default: 0.5)
    --eps       precision for power method convergence (default: 1.0e-4)
    --iters     maximum number of iterations (default 1.0e+3)
    --collate   collate properties into matrix: T / F (default: F)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=USairport2010
EXT=snap
EXAMPLE=wcentrality
ROOT=../../datasets/$DATASET
rm -rf $ROOT/$EXAMPLE
mkdir $ROOT/$EXAMPLE
./$EXAMPLE -i:$ROOT/$DATASET.$EXT \
           -e:$ROOT/exogenous.status.TIntFltH \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -k:3 -c:0.85 -r:0.5 \
           --eps:1.0e-5 --iters:1e+4 \
           --collate:F
```