egonets (properties)
--------------------

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

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./egonets -i:<input network> -o:<output prefix> [Options]
Options:
    -i          input network (tab separated list of edges with edge weights)
    -o          output prefix (filename extensions added)
    -d          direction of ego traversal: in = 1, out = 2, undirected = 3
                    (default: 3)
    -k          depth of ego traversal (default: 1)
    --collate   collate properties into matrix: T / F (default: F)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=../../datasets/USairport2010
rm -rf $DATASET/egonets
mkdir $DATASET/egonets
./egonets -i:$DATASET/USairport2010.snap \
          -o:$DATASET/egonets/USairport2010 \
          -k:3 --collate:T
```