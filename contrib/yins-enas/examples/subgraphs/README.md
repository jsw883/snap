subgraphs (egonet)
------------------

Computes the subgraph corresponding to the egonets of either the top N
nodes in the network sorted by degree or weighted degree, or the external
list of nodes provided.

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./subgraphs -i:<input network> -o:<output prefix> [Options]
Options:
    -i          input network (tab separated list of edges with edge weights)
    -o          output prefix (filename extensions added)
    -d          direction of ego traversal: in = 1, out = 2, undirected = 3
                    (default: 3)
    -k          depth of ego traversal (default: 1)
    --n         number of nodes to include (default: 10)
    --measure   measure used to order nodes (default: degree)
    --egov      subset of nodes to compute egonets (overrides -n)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=USairport2010
EXT=snap
EXAMPLE=subgraphs
ROOT=../../datasets/$DATASET
rm -rf $ROOT/$EXAMPLE
mkdir $ROOT/$EXAMPLE
./$EXAMPLE -i:$ROOT/$DATASET.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -d:3 -k:3 --collate:T
```