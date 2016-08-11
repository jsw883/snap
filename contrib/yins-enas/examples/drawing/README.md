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
    -b              output border (default: 10)
    --png           output PNG (default: T)
    --pdf           output PDF (default: T)
    --layout        layout algorithm (random / circular / reingold)
                        (default: circular)
    --iterations    number of iterations for reingold (default: 1500)
    --cooling       cooling coefficient for reingold (default: 1.5)
	--shuffle       shuffle vertex order for circular layout (and reingold)
                        (default: F)
    --vr            vertex radius relative to minimum axis
                        (default: 0.1*sqrt(nodes))
    --vw            vertex border width (default: 1)
    --vf            vertex fill (default: 000000)
    --vc            vertex border color (default: FFFFFF)
    --vfalpha       vertex fill alpha (default: 1)
    --vcalpha       vertex color alpha (default: --vfalpha)
    --ew            edge width (default: 1)
    --ec            edge color (default: black)
    --ecalpha       edge color alpha (default: 0.5)
```

### Example ###

This example uses [USairport2010](/contrib/yins-enas/datasets/USairport2010),
which is included in this repository.

Run filtering and wcentrality examples before drawing.

```bash
DATASET=USairport2010
EXT=snap
EXAMPLE=drawing
ROOT=../../datasets/$DATASET
rm -rf $ROOT/$EXAMPLE
mkdir $ROOT/$EXAMPLE
./$EXAMPLE -i:$ROOT/filtering/$DATASET-3.727594e-04.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -w:2500 -h:2500 -b:50 --layout:reingold --iterations:1500 \
           --cooling:1.5 --shuffle:T --vf:FF0000 --ecalpha:0.1 \
           --vrv:$ROOT/wcentrality/$DATASET.wpgr \
           --vfv:$ROOT/wcommunity/$DATASET.louvain.modularity.HEX
```