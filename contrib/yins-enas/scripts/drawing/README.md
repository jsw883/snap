drawing
-------

Graph drawing using cairo (currently being developed).

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./drawing -i:<input network> -o:<output prefix> [Options]
Options:
    -i              input network (tab separated list of edges)
    -o              output prefix (filename extensions added)
    -w              output width (default: 1000)
    -h              output height (default: 1000)
    -h              output border (default: 10)
    --png           output PNG (default: T)
    --pdf           output PDF (default: T)
    --layout        layout algorithm (random / circular / reingold)
                        (default: circular)
    --iterations    number of iterations for reingold (default: 1000)
    --cooling       cooling coefficient for reingold (default: 3)
    --vr            vertex radius relative to minimum axis
                        (default: 0.1*sqrt(nodes))
    --vw            vertex border width (default: 1)
    --ew            edge width (default: 1)
```

### Example ###

This example generates graphs for testing.

```bash
DATASET=../../datasets/USairport2010
./drawing -i:$DATASET/USairport2010.snap \
          -o:$DATASET/USairport2010.drawing
          -w:2500 -h:2500 --layout:reingold
          --cooling:1.5
```