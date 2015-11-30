community (efficient community detection algorithms)
----------------------------------------------------

Computes clusters of nodes using efficient community detection algorithms;
Louvain method with an arbitrary quality objective (modularity objective),
*and a local clustering method with an arbitrary measure of centrality*.

Community detection algorithms currently implemented:

  - Louvain method (arbitrary quality objective)
    - Modularity

To be implemented:

  - *Local clustering method*

**This is currently under active delopment and is not yet completed.**

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Parameters ###

  - -i:input network (tab separated list of edges)
  - -o:output prefix (filename extensions added)
  - -eps:minimum quality improvement threshold
  - -iters:maximum number of iterations
  - -moves:minimum number of moves required (proportional)

### Usage ###

```bash
rm -r ../../datasets/USairport2010/community
mkdir ../../datasets/USairport2010/community
community -i:../../datasets/USairport2010/USairport2010.snap -o:../../datasets/USairport2010/community/USairport2010 -c:F
```

### Usage ###

```
Usage: ./community -i:<input network> -o:<output prefix> [Options]
Options:
    -i          input network (tab separated list of edges with edge weights)
    -o          output prefix (filename extensions added)
    --eps       minimum quality improvement threshold (default: 1.0e-5)
    --moves     minimum number of moves (relative) (default: 1.0e-2)
    --iters     maximum number of iterations (default: 1.0e+4)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=../../datasets/amazon0302
rm -rf $DATASET/community
mkdir $DATASET/community
./community -i:$DATASET/amazon0302.snap.FltW \
          -o:$DATASET/community/amazon0302 \
          --eps 1.0e-5 --moves:1.0e-2 --iters:1.0e+4
```