# KMC

> Kinetic Monte Carlo simulation program.

## Build

### Install dependency
We use [pkg](https://github.com/genshen/pkg) tool to manage c++/c dependency.
Make sure you have pkg tool (version 0.2.0-beta or higher) installed, and the PATH environment variable is correctly set.
```bash
$ pkg fetch # download dependency packages
$ pkg install # build and install dependency packages
```

### build
Make sure **cmake** is installed on your system.
```bash
$ mkdir build
$ cd build
$ cmake ../
$ make
```
