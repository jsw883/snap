wcommunity (efficient weighted community detection algorithms)
--------------------------------------------------------------

Computes clusters of nodes using efficient community detection algorithms;
Louvain method with an arbitrary quality objective (modularity objective),
*and a local clustering method with an arbitrary measure of centrality*.

Community detection algorithms currently implemented:

  - Louvain method (arbitrary quality objective)
    - Weighted modularity

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

```
Usage: ./community -i:<input network> -o:<output prefix> [Options]
Options:
    -i          input network (tab separated list of edges with edge weights)
    -o          output prefix (filename extensions added)
    --eps       minimum quality improvement threshold (default: 1.0e-5)
    --moves     minimum number of moves (relative) (default: 1.0e-2)
    --iters     maximum number of iterations (default: 1.0e+4)
    --subgraphs output community subgraphs: T / F (default: F)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=USairport2010
EXT=snap
EXAMPLE=wcommunity
ROOT=../../datasets/$DATASET
rm -rf $ROOT/$EXAMPLE
mkdir $ROOT/$EXAMPLE
./$EXAMPLE -i:$ROOT/$DATASET.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           --eps 1.0e-5 --moves:1.0e-2 --iters:1.0e+4 \
           -s:1.0 -l:0.5
```