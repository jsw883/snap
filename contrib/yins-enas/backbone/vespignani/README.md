vespignani (multiscale backbone extraction)
-------------------------------------------

Summagy goes here

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
./vespignani -i:../../datasets/USairport2010/USairport2010.snap -o:../../datasets/USairport2010/vespignani/USairport2010-backbone.snap
```