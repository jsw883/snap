wstatistics (weighted small world properties)
---------------------------------------------

Computes weighted network statistics / small world properties: nodes, edges,
sum of weights, average weighted degree, and maximum weighted degree.

Statistics:

  - nodes |V|, edges |E|, sum of weights, average weighted degree
  - maximum weighted degree (in / out / undirected)
  - clustering coefficient (average weighted / directed)

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./wstatistics -i:<input network> -o:<output prefix> [Options]
Options:
    -i      input network (tab separated list of edges with edge weights)
    -o      output prefix (filename extensions added)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=USairport2010
EXT=snap
EXAMPLE=wstatistics
ROOT=../../datasets/$DATASET
rm -rf $ROOT/$EXAMPLE
mkdir $ROOT/$EXAMPLE
./$EXAMPLE -i:$ROOT/$DATASET.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
```