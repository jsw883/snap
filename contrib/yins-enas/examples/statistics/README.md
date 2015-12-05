statistics (small world properties)
------------------------------------

Computes network statistics / small world properties: nodes, edges, average
degree, density, maximum degree, diameter, average path length, average and
global clustering coefficients / transitivity.

Statistics:

  - nodes |V|, edges |E|, average degree 2*|E|/|V|, density |E|/|V|^2
  - maximum degree (in / out / undirected)
  - diameter (effective / approximate)
  - average path length
  - clustering coefficient (average / global)

Curves:

  - approximate neighbourhood function / shortest path cumulative density

Note that diameter and average path length are found using the approximate
neighbourhood function / shortest path cumulative density as described in
the paper [ANF: A Fast and Scalable Tool for Data Mining in Massive Graphs](http://www.cs.cmu.edu/~christos/PUBLICATIONS/kdd02-anf.pdf)
by C. R. Palmer, P. B. Gibbons, and C. Faloutsos and implemented in SNAP.

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./statistics -i:<input network> -o:<output prefix> [Options]
Options:
    -i          input network (tab separated list of edges with edge weights)
    -o          output prefix (filename extensions added)
    --exact     compute exact neighborhood function: T / F (default: T)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=../../datasets/USairport2010
rm -rf $DATASET/statistics
mkdir $DATASET/statistics
./statistics -i:$DATASET/USairport2010.snap \
             -o:$DATASET/statistics/USairport2010 \
             --exact:T
```