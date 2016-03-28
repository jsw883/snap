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
    -i              input network (tab separated list of edges)
    -o              output prefix (filename extensions added)
    -d              direction of traversal: in = 1, out = 2, undirected = 3
                        (default: 3)
    -p              percolation probability (default: -1)
    --lowerbound    lower bound for percolation threshold (default: 0.0)
    --upperbound    upper bound for percolation threshold (default: 1.0)
    --tolerance     tolerance for percolation threshold (default: 1e-4)
    --smoothing     repetitions for binary search for percolation threshold 
                        (default: 100)
    --iters         number of iterations to average results (default: 10)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=../../datasets/USairport2010
rm -rf $DATASET/structures
mkdir $DATASET/structures
./structures -i:$DATASET/USairport2010.snap \
             -o:$DATASET/structures/USairport2010 \
             -p:0.5 -d:2 --iters:10
```