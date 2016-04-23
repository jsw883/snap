wdistance (subset connectivity)
-----------------------------

Computes neighborhood node counts, radius, and diameter for a subset of nodes
specified, and optionally computes the same for a random subset of nodes for
a comparison (where the random subset is disjoint from the subset specified).

Alternately, computes the same for every node, exhaustively, so that wdistance
can be selected from the output afterward.

  * Measures computed
    * Exact individual neighborhood function (exhaustive)
    * Shortest paths to destination subset nodes (exhaustive) 
    * Summary
      * Node counts
      * Radius (average path length)
      * Diameter

Effectively, this computes the individual neighborhood function for each node
in the subset (although this could possibly be changed to use a faster edge
iteration / bit string approximation method), and then computes the quantiles
for the radius (average path length) and diameter.

As this computation is exhaustive, the cost is O(|U|(|V| + |E|)) for U in V.

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
    --dir           direction of traversal: in = 1, out = 2, undirected = 3
                        (default: 3)
    --exhaustive    compute for every node (overrides -s, --compare): T / F
                        (default: F)
    --collate       collate properties into matrix: T / F (default: F)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=../../datasets/USairport2010
rm -rf $DATASET/wdistance
mkdir $DATASET/wdistance
./wdistance -i:$DATASET/USairport2010.snap \
            -s:$DATASET/SrcNIdV.TIntV \
            -d:$DATASET/DstNIdV.TIntV \
            -o:$DATASET/wdistance/USairport2010 \
            -k:10 --tol:0 --dir:3
```

gdb --args ./wdistance -i:$DATASET/USairport2010.snap \
            -s:$DATASET/SrcNIdV.TIntV \
            -d:$DATASET/DstNIdV.TIntV \
            -o:$DATASET/wdistance/USairport2010 \
            -k:10 --tol:0 --dir:3
