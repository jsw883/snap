wdistance (subset distances)
----------------------------

Computes a propietary weighted distance measure between two subsets of nodes, 
computing the aggregate flow between each pair of nodes with one node in each
of the subsets, assuming uniform distribution of flow across outgoing edges,
limiting the overlapping BFS by depth and weighted distance (which makes the
otherwise factorial complexity slightly more tractable).

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./wdistance -i:<input network> -o:<output prefix> [Options]
Options:
    -i              input network (tab separated list of edges)
    -o              output prefix (filename extensions added)
    -s              source subset nodes (column of nodes)
    -d              destination subset nodes (column of nodes)
    -k              depth limit for BFS (default: 10)
    --tol           weighted distance limit for BFS (default: 1e-3)
    --dir           direction of traversal: in = 1, out = 2, undirected = 3
                        (default: 3)
    --exclude       exclude other source nodes from BFS (default: true)
    --normalized    edge weights are pre-normalized (default: false)
    --exhaustive    compute for every node (overrides -s, --compare): T / F
                        (default: F)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=USairport2010
EXT=snap
EXAMPLE=wdistance
ROOT=../../datasets/$DATASET
rm -rf $ROOT/$EXAMPLE
mkdir $ROOT/$EXAMPLE
./$EXAMPLE -i:$ROOT/$DATASET.$EXT \
           -s:$ROOT/SrcNIdV.TIntV \
           -d:$ROOT/DstNIdV.TIntV \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -k:10 --tol:1e-1 --dir:3 --exclude:F
```