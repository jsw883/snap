diameters (subset connectivity)
-------------------------------

Computes neighborhood diameters and node counts for a subset of nodes in the
graph, and the average diameter of the neighborhoods for the subset overall.

Effectively, this computes the extreme of the neighborhood function isolated
to each node in the subset (although this could possibly be changed to take
advantage of the edge iteration / bit string approximation method).

As this computation is exhaustive, the cost is O(|U|(|V| + |E|)) for U in V.

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./diameters -i:<input network> -o:<output prefix> [Options]
Options:
    -i          input network (tab separated list of edges)
    -s          subset nodes (column of nodes)
    -o          output prefix (filename extensions added)
    -d          direction of ego traversal: in = 1, out = 2, undirected = 3
                    (default: 3)
    --compare   compare to a random subset of nodes: T / F (default: T)
    --collate   collate properties into matrix: T / F (default: F)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=../../datasets/USairport2010
rm -rf $DATASET/diameters
mkdir $DATASET/diameters
./diameters -i:$DATASET/USairport2010.snap \
            -s:$DATASET/subset.TIntV \
            -o:$DATASET/diameters/USairport2010 \
            -d:3 --compare --collate:T
```