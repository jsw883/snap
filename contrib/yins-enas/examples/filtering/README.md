filtering (multiscale backbone extraction)
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
rm -r ../../datasets/USairport2010/filtering
mkdir ../../datasets/USairport2010/filtering
./filtering -i:../../datasets/USairport2010/USairport2010.snap -o:../../datasets/USairport2010/filtering/USairport2010
```


### Usage ###

```
Usage: ./filtering -i:<input network> -o:<output prefix> [Options]
Options:
    -i      input network (tab separated list of edges with edge weights)
    -o      output prefix (filename extensions added)
    -a      alpha significance level threshold (default: 0.01)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=../../datasets/USairport2010
rm -rf $DATASET/filtering
mkdir $DATASET/filtering
./filtering -i:$DATASET/USairport2010.snap \
             -o:$DATASET/filtering/USairport2010 \
             -a:0.05
```