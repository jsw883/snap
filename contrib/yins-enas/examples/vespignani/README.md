vespignani (multiscale backbone extraction)
-------------------------------------------

Applies the pruning algorithm documented by Serrano, Boguna, and Vespignani
in "Extracting the multiscale backbone of complex weighted networks" (2009).
A hypothesis is performed on each edge examine whether its normalized weight
suggests the edge is statisically significant and conveys meaningful information.

Program appends -(alpha).snap to the specified output filename.  The (alpha)
in the filename is a float with 5 digits of precision.

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Parameters ###

  - -i:input graph (tab separated list of edges)
  - -o:output graph name
  - -a: threshold level of significance (default: 0.01)

### Usage ###

```bash
rm -r ../../datasets/USairport2010/vespignani
mkdir ../../datasets/USairport2010/vespignani
./vespignani -i:../../datasets/USairport2010/USairport2010.snap -o:../../datasets/USairport2010/vespignani/USairport2010-backbone
```