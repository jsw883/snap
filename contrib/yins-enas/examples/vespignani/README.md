vespignani (multiscale backbone extraction)
-------------------------------------------

Applies the edge filtering algorithm defined by Serrano, Boguna, and Vespignani
in the paper [Extracting the multiscale backbone of complex weighted networks](http://arxiv.org/pdf/0904.2389.pdf) (2009).

Edges that have a statistically significant normalized edge wight are retained,
with respect to a unifrom null distribution and the significance level (alpha)
specified. A smaller value of alpha will remove more edges from the graph, and
an alpha value of one will remove no edges.

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Parameters ###

  - -i:input network (tab separated list of edges)
  - -o:output prefix (alpha value and filename extensions added)
  - -a:alpha significance threshold (default: 0.01)

### Usage ###

```bash
rm -r ../../datasets/USairport2010/vespignani
mkdir ../../datasets/USairport2010/vespignani
./vespignani -i:../../datasets/USairport2010/USairport2010.snap -o:../../datasets/USairport2010/vespignani/USairport2010
```


### Usage ###

```
Usage: ./vespignani -i:<input network> -o:<output prefix> [Options]
Options:
    -i            input network (tab separated list of edges with edge weights)
    -o            output prefix (filename extensions added)
    -a            alpha significance level threshold (default: 0.01)
    --alphav      vector of alpha significance level threshold (overrides -a)
    --verbose     verbose output for each step of the Vespignani method
                      (default: true)
    --bootstrap   bootstrap to retain --ratio of total weight W
    --ratio       bootstrap target ratio of total weight W
    --lowerbound  lower bound for alpha (binary search)
    --upperbound  upper bound for alpha (binary search)
    --tolerance   tolerance for alpha (binary search)
    --spread      spread for bootstrapped alpha benchmark (binary search)

```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=amazon0302
EXT=snap.FltW
EXAMPLE=vespignani
ROOT=../../datasets/$DATASET
rm -rf $ROOT/$EXAMPLE
mkdir $ROOT/$EXAMPLE
./$EXAMPLE -i:$ROOT/$DATASET.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           --bootstrap:T --ratio:0.5 --tolerance:1e-4 \
           --verbose:T
```
