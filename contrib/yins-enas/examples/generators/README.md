nullmodels (weighted random graph generators)
---------------------------------------------

Generates random weighted graphs using generative algorithms with a predefined
null model or randomization algorithms which randomize an existing graph using
Markov Chain Monte Carlo algorithms that preserve specific distributions.

Graph generators:

  - Weighted random graph model (analogue of Erdos Renyi)
    - <http://arxiv.org/abs/0902.0897>
  - Weighted preferential attachment model (analogue of Barabsi Albert)
    - <http://journals.aps.org/pre/pdf/10.1103/PhysRevE.86.026117>

Algorithms:

  - Global weight reshuffling
    - *Degree, weight, and connectivity preserving*
    - *Weighted degree breaking*
  - ~Local weight reshuffling (reshuffling direction can be swapped)~
    - *Degree, weighted out degree, weight, and connectivity preserving*
    - *Weighted in degree breaking*

  - ~Edge rewiring~
    - *Degree, weight preserving*
    - *Weighted degree, onnectivity breaking*
  - ~Tetragonal weight redistributing~
    - *Degree, connectivity preserving (weights can be vanishing)*
    - *Weighted degree, weight breaking*
    - <http://journals.aps.org/pre/pdf/10.1103/PhysRevE.84.026103>

Note that if an input graph is provided, generator parameters are automatically
populated based on graph statistics: nodes, edges, and the sum of weights. Any
other parameter required by the model must be specified manually, and carefully
decided.

WARNING: the threshold for the exponentially weighted random graph model should
be set VERY carefully, as if it is too small the graph will be fully connected,
which would be prohibitively expensive (CPU and RAM).

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./generators -o:<output prefix> [Options]
Options:
    -i          input network (tab separated list of edges with edge weights)
    -o          output prefix (filename extensions added)
    -g          generator or algorithm to apply (default: 4)
                    WPrefAttach: Weighted preferential attachment model
                           (parameters: k)
                    GWShuffling: Global weight reshuffling
                           (parameters: none)
    -n          nodes (default: Graph->GetNodes())
    -m          edges (default: Graph->GetNodes())
    -w          sum of weights (default: Graph->GetTotalW())
    -k          out degree for preferential attachment model
    --scale     Pareto scale for preferential attachment model
                    (default: FitParetoWeights(Graph, Scale, Shape))
    --shape     Pareto shape for preferential attachment model
                    (default: FitParetoWeights(Graph, Scale, Shape))
    --threshold threshold for exponentially weighted random graph model
                    (WARNING: DO NOT SET TO 0, GRAPH WILL BE FULLY CONNECTED)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=USairport2010
EXT=snap
EXAMPLE=generators
ROOT=../../datasets/$DATASET
rm -rf $ROOT/$EXAMPLE
mkdir $ROOT/$EXAMPLE
./$EXAMPLE -i:$ROOT/$DATASET.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -g:GWShuffling
```