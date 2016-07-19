colors
------

Colors for using with cairo (currently being developed).

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./colors [Options]
Options:
    -o          output prefix (filename extensions added)
    -n          size of color scale (default: 30)
    --type      type of color scale (default: quantitative)
```

### Example ###

```bash
SCRIPT=colors
TYPE=quantitative
N=30
mkdir -p ../../colors
./$SCRIPT -o:../../colors/$TYPE$N \
          -n:$N --type:$TYPE
```