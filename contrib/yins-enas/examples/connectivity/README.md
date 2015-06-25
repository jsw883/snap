connectivity (bowtie structure)
-------------------------------

Computes connectivity structures, namely weakly connected components (WCCS),
strongly connected cores (SCCS), components connecting in to the SCCS
(INS), components connecting out from the SCCS (OUTS), and tendrils from the
INS and to the OUTS (TES).

Connectivity structures (giant only):

  - giant weakly connected component (GWCC)
  - giant strongly connected component (GSCC)
  - components connecting in to the GSCC (IN)
  - components connected out from the GSCC (OUT)
  - tendrils from the IN and to the OUT (TE)

Connectivity structures (each weak):

  - weakly connected components (WCCS)
  - strongly connected cores (SCCS)
  - components connecting in to the SCCS (INS)
  - components connected out from the SCCS (OUTS)
  - tendrils from the INS and to the OUTS (TES)

Note that for each weak the SCCS, INS, OUTS, and TES are for each weakly
connected component and as such the output files should have the same
number of connected components. Hence instead of obtaining all strongly
connected components, merely the strongly connected core of each weakly
connected component is obtained.

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Parameters ###

  - -i:input graph (tab separated list of edges)
  - -o:output prefix (filename extensions added)
  - -s:giant only / each weak structure (T / F)

### Usage ###

```bash
rm -r ../../datasets/USairport2010/connectivity
mkdir ../../datasets/USairport2010/connectivity
connectivity -i:../../datasets/USairport2010/USairport2010.snap -o:../../datasets/USairport2010/connectivity/USairport2010 -c:F
```