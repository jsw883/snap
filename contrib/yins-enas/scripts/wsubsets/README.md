wsubsets
-------

Generates random subsets of weighted graphs for testing.

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./subsets -i:<input network> -o:<output prefix> [Options]
Options:
    -i          input network (tab separated list of edges)
    -o          output prefix (filename extensions added)
    -n          size of subset (default: 100, and -p is ignored)
    -k          number of disjoint subsets (default: 1)
```

### Example ###

This example generates graphs for testing.

```bash
DATASET=../../datasets/USairport2010
./wsubsets -i:$DATASET/USairport2010.snap \
           -o:$DATASET/ \
           -n:100 -k:1
```