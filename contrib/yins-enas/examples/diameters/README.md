diameters (subset connectivity)
-------------------------------

Computes neighborhood diameters and node counts for a subset of nodes in the
graph, and the average diameter of the neighborhoods for the subset overall.

Effectively, this computes the extreme of the neighborhood function isolated
to each node in the subset (although this could possibly be changed to take
advantage of the edge iteration / bit string approximation method).

As this computation is exhaustive, the cost is O(|U|(|V| + |E|)) for U in V.

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Parameters ###

  - -i:input graph (tab separated list of edges)
  - -j:subset of nodes (column list of nodes)
  - -o:output prefix (filename extensions added)
  - -c:collate centralities into matrix (T / F)

### Usage ###

```bash
rm -r ../../datasets/USairport2010/diameters
mkdir ../../datasets/USairport2010/diameters
./diameters -i:../../datasets/USairport2010/USairport2010.snap -j:../../datasets/USairport2010/nodes.TIntV -o:../../datasets/USairport2010/diameters/USairport2010 -c:F
```