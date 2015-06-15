wstatistics (wegihted small world properties)
---------------------------------------------

Computes weighted network statistics / small world properties: nodes, edges,
sum of weights, average weighted degree, and maximum weighted degree.

Statistics:

  - nodes |V|, edges |E|, sum of weights, average weighted degree
  - maximum weighted degree (in / out / undirected)

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Parameters ###

  - -i:input graph (tab separated list of edges)
  - -o:output prefix (filename extensions added)

### Usage ###

```bash
rm -r ../../datasets/USairport2010/USairport2010/wstatistics
mkdir ../../datasets/USairport2010/USairport2010/wstatistics
wstatistics -i:../../datasets/USairport2010/USairport2010.snap -o:../../datasets/USairport2010/wstatistics/USairport2010
```