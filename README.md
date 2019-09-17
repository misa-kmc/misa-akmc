# KMC

> Parallel on lattice kinetic Monte Carlo simulation program.

## Build

### Install dependency
We use [pkg](https://github.com/genshen/pkg) tool to manage c++/c dependency.
Make sure you have pkg tool (version 0.3.0 or higher) installed, and the PATH environment variable is correctly set.
```bash
$ PKG_AUTH=username?token@git.hpcer.dev pkg fetch # download dependency packages
$ pkg install # build and install dependency packages
```

### build
Make sure **cmake** is installed on your system.
```bash
$ cmake -B./build -H./ -DCMAKE_BUILD_TYPE=Release
$ cmake --build ./build
```
