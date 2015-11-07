community (efficient community detection algorithms)
----------------------------------------------------

Computes clusters of nodes using efficient community detection algorithms;
Louvain method with an arbitrary quality objective (modularity objective),
and a local clustering method with an arbitrary measure of centrality.

Community detection algorithms currently implemented:

  - *Louvain method (arbitrary quality objective)*
    - *Modularity*
  - *Local clustering method*

**This is currently under active delopment and is not yet completed.**

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Parameters ###

  - -i:input graph (tab separated list of edges)
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