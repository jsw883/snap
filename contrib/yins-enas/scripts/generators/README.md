generators (graph generators)
------------------------------------

Generates graphs for testing.

Deterministic graph generators:

  - line
  - grid (2D)
  - circle
  - complete
  - tree

Random graph generators:

  - erdos (Erdos Renyi)
  - power (Power Law)

Makefiles are provided for compilation under Windows with Cygwin and gcc,
and under Mac OS X, Linux, and other Unix operating systems with gcc as
well. For makefiles, compile the code with `make all`.

### Usage ###

```
Usage: ./generators -o:<output prefix> [Options]
Options:
    -o          output prefix (filename extensions added)
    -n          nodes (default: 1000)
    -k          circle connection degree (default: 1)
    -p          Erdos Renyi edge probability (default: 0.5)
    --exp       power law exponent (default: 2.5)
```

### Example ###

This example generates graphs for testing.

```bash
N=1000
rm -rf ../../generated/$N
mkdir ../../generated/$N
./generators -o:../../generated/$N/graph -n:1000 -k:5
```