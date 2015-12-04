structures (percolation algorithms)
-----------------------------------

Applies edge percolation algorithms to explore the topology of networks
and star or chain structures that arise near the percolation threshold.

**This is currently under active delopment and is not yet completed.**

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./structures -i:<input network> -o:<output prefix> [Options]
Options:
    -i          input network (tab separated list of edges)
    -o          output prefix (filename extensions added)
    -p          percolation probability
    -d          direction of traversal: in = 1, out = 2, undirected = 3
                    (default: 3)
    --iters     number of iterations to average results (default: 10)
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