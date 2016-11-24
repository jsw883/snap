structures (percolation algorithms)
-----------------------------------

Applies edge percolation algorithms to explore the topology of networks
and star or chain structures that arise near the percolation threshold.

The method removes edges from the graph randomly with uniform probability,
and then computes the following topological statistics for the remaining
connected components:

  * *Giant components (can be used to estimate percolation threshold)*
    * Giant size (largest component size)
    * Giant ratio (largest component size : next largest component size)
  * *Averages*
    * Size
    * Radius (average path length)
    * Radius / size (ratio)
    * Diameter
    * Diameter / size (ratio) ***

**This is currently under active delopment and is not yet completed.**

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./structures -i:<input network> -o:<output prefix> [Options]
Options:
    -i                  input network (tab separated list of edges)
    -o                  output prefix (filename extensions added)
    -d                  direction of traversal: in = 1, out = 2, undirected = 3
                            (default: 3)
    --loweralphabound   lower bound for alpha (default: 0.0)
    --upperalphabound   upper bound for alpha (default: 1.0)
    --step              alpha step size (default: 1e-4)
    --lowersizebound    lower bound for weakly connected component sizes
    --uppersizeratio    upper bound for weakly connected component sizes
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=USairport2010
EXT=snap
EXAMPLE=structures
ROOT=../../datasets/$DATASET
rm -rf $ROOT/$EXAMPLE
mkdir $ROOT/$EXAMPLE
./$EXAMPLE -i:$ROOT/$DATASET.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           --loweralphabound:0.0 --upperalphabound:1.0 --step:0.5 \
           --lowersizebound:3 --uppersizeratio:0.5
```
