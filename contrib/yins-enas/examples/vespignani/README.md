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

  - -i:input graph (tab separated list of edges)
  - -o:output prefix (alpha value and filename extensions added)
  - -a:alpha significance threshold (default: 0.01)

### Usage ###

```bash
rm -r ../../datasets/USairport2010/vespignani
mkdir ../../datasets/USairport2010/vespignani
./vespignani -i:../../datasets/USairport2010/USairport2010.snap -o:../../datasets/USairport2010/vespignani/USairport2010
```