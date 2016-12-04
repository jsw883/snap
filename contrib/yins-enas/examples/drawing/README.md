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
    --layout        layout algorithm (random / circular / reingold / atlas)
                        (default: circular)
    --iterations    number of iterations for reingold (default: 1500)
    --cooling       cooling coefficient for reingold (default: 1.5)
	--shuffle       shuffle vertex order for circular layout (and reingold)
                        (default: F)
    --scaling       repulsion scaling for force directed (atlas)
    --gravity       gravity for force directed (atlas)
    --weights       weight influence exponent for force directed (atlas)
    --nohubs        dissuade hubs for force directed (atlas)
    --linlog        switch linlog mode for force directed (atlas)
    --vr            vertex radius relative to minimum axis
                        (default: 0.1*sqrt(nodes))
    --vw            vertex border width (default: 1)
    --vf            vertex fill (default: 000000)
    --vc            vertex border color (default: FFFFFF)
    --vfalpha       vertex fill alpha (default: 1)
    --vcalpha       vertex color alpha (default: --vfalpha)
    --label         label vertices by NId (default: F)
    --vrv           vertex radius mapping relative to vertex radius
                        (overrides --vr)
    --vwv           vertex border width mapping (overrides --vw)
    --vfv           vertex fill mapping (overrides --vf)
    --vcv           vertex border color mapping (overrides -- vcstr)
    --vfcommunity   color vertices by community (overrides --vf and --vfv)
                        (default: F)
    --eps           minimum quality improvement threshold
    --moves         minimum number of moves (relative)
    --iters         maximum number of iterations
    -s              community vertex color saturation value (0.0 - 1.0)
    -l              community vertex lightness value (0.0 - 1.0)
    --ew            edge width (default: 1)
    --ec            edge color (default: black)
    --ecs           source edge color for gradient and duotone coloring
                        (default: red)
    --ecd           destination edge color for gradient and duotone coloring
                        (default: blue)
    --ecalpha       edge color alpha (default: 0.5)
    --direction     how to show directionality (arrow / gradient / duotone)
                        (default: none)
    --as            arrow size relative to minimum axis
                        (default: 0.05*sqrt(nodes))
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
./$EXAMPLE -i:$ROOT/vespignani/$DATASET-3.727594e-04.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -w:2500 -h:2500 -b:50 --layout:reingold --iterations:1500 \
           --cooling:1.5 --shuffle:T --vf:FF0000 --ecalpha:0.1 \
           --label:T \
           --vrv:$ROOT/wcentrality/$DATASET.WPgRH \
           --vfv:$ROOT/wcommunity/$DATASET.louvain.NIdHEXH
```

```bash
DATASET=USairport2010
EXT=snap
EXAMPLE=drawing
ROOT=../../datasets/$DATASET
rm -rf $ROOT/$EXAMPLE
mkdir $ROOT/$EXAMPLE
./$EXAMPLE -i:$ROOT/vespignani/$DATASET-3.727594e-04.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -w:2500 -h:2500 -b:50 --layout:atlas --iterations:1500 \
           --cooling:1.5 --shuffle:T \
           --scaling:5e-3 --gravity:5 --weights:0.5 --nohubs:F --linlog:T \
           --vf:FF0000 --ecalpha:0.1 \
           --label:T \
           --vrv:$ROOT/wcentrality/$DATASET.WPgRH \
           --vfv:$ROOT/wcommunity/$DATASET.louvain.NIdHEXH
```

DATASET=USairport2010
EXT=snap
EXAMPLE=drawing
ROOT=../../datasets/$DATASET
rm -rf $ROOT/$EXAMPLE
mkdir $ROOT/$EXAMPLE
./$EXAMPLE -i:$ROOT/vespignani/$DATASET-3.727594e-04.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -w:2500 -h:2500 -b:50 --layout:reingold --iterations:1500 \
           --cooling:1.5 --shuffle:T --vf:FF0000 --ecalpha:0.1 \
           --label:T \
           --vrv:$ROOT/wcentrality/$DATASET.WPgRH \
           --vfv:$ROOT/wcommunity/$DATASET.louvain.NIdHEXH


DATASET=USairport2010
EXT=snap
EXAMPLE=drawing
ROOT=../../datasets/$DATASET
./$EXAMPLE -i:$ROOT/vespignani/$DATASET-3.727594e-04.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -w:2500 -h:2500 -b:50 --layout:reingold --iterations:1500 \
           --cooling:1.5 --shuffle:T --vf:FF0000 --ecalpha:0.1 \
           --label:T \
           --vrv:$ROOT/wcentrality/$DATASET.WPgRH \
           --vfv:$ROOT/wcommunity/$DATASET.louvain.NIdHEXH

DATASET=USairport2010
EXT=snap
EXAMPLE=drawing
ROOT=../../datasets/$DATASET
./$EXAMPLE -i:$ROOT/vespignani/$DATASET-3.727594e-04.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -w:2500 -h:2500 -b:50 --layout:atlas --initial:$ROOT/drawing/$DATASET.C1.50e+00.reingold.2500x2500.CoordH \
           --iterations:1500 \
           --cooling:1.5 --shuffle:T \
           --scaling:5e-3 --gravity:5 --weights:0.5 --nohubs:F --linlog:T \
           --vf:FF0000 --ecalpha:0.1 \
           --label:T \
           --vrv:$ROOT/wcentrality/$DATASET.WPgRH \
           --vfv:$ROOT/wcommunity/$DATASET.louvain.NIdHEXH

DATASET=USairport2010
EXT=snap
EXAMPLE=drawing
ROOT=../../datasets/$DATASET
./$EXAMPLE -i:$ROOT/vespignani/$DATASET-3.727594e-04.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -w:2500 -h:2500 -b:50 --layout:precomputed --initial:$ROOT/drawing/$DATASET.C1.50e+00.reingold.2500x2500.CoordH \
           --vf:FF0000 --ecalpha:0.1 \
           --label:T \
           --vrv:$ROOT/wcentrality/$DATASET.WPgRH \
           --vfv:$ROOT/wcommunity/$DATASET.louvain.NIdHEXH

DATASET=USairport2010
EXT=snap
EXAMPLE=drawing
ROOT=../../datasets/$DATASET
./$EXAMPLE -i:$ROOT/vespignani/$DATASET-3.727594e-04.$EXT \
           -o:$ROOT/$EXAMPLE/$DATASET \
           -w:2500 -h:2500 -b:50 --layout:atlas --initial:$ROOT/drawing/$DATASET.C1.50e+00.reingold.2500x2500.CoordH \
           --iterations:1500 \
           --cooling:1.5 --shuffle:T \
           --scaling:5e-3 --gravity:5 --weights:0.5 --nohubs:F --linlog:T \
           --vf:FF0000 --ecalpha:0.1 \
           --label:T \
           --vrv:$ROOT/wcentrality/$DATASET.WPgRH \
           --vfv:$ROOT/wcommunity/$DATASET.louvain.NIdHEXH
