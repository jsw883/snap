connectivity (bowtie structure)
-------------------------------

Computes connectivity structures, namely weakly connected components (WCCS),
strongly connected cores (SCCS), components connecting in to the SCCS (INS),
components connecting out from the SCCS (OUTS), and tendrils from the INS
and to the OUTS (TES).

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

### Usage ###

```
Usage: ./egonets -i:<input network> -o:<output prefix> [Options]
Options:
    -i              input network (tab separated list of edges)
    -o              output prefix (filename extensions added)
    --giant-only    compute giant weakly connected component only (default: F) 
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository. 

```bash
DATASET=../../datasets/USairport2010
rm -rf $DATASET/connectivity
mkdir $DATASET/connectivity
./connectivity -i:$DATASET/USairport2010.snap \
               -o:$DATASET/connectivity/USairport2010 \
               --giant-only:T
```