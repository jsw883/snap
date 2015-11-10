wcentrality (weighted vertex distributions)
-------------------------------------------

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

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./wcentrality -i:<input graph> -o:<output prefix> [Options]
Options:
    -i          input graph (tab separated list of edges with edge weights)
    -o          output prefix (filename extensions added)
    -k          depth of degree traversal (default: 1)
    --collate   collate properties into matrix: T / F (default: F)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=../../datasets/USairport2010
rm -rf $DATASET/wcentrality
mkdir $DATASET/wcentrality
./wcentrality -i:$DATASET/USairport2010.snap \
              -o:$DATASET/wcentrality/USairport2010 \
              -k:3 --collate:T
```