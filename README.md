# ripple-libpp

[![Build Status](https://travis-ci.org/ripple/ripple-libpp.svg?branch=master)](https://travis-ci.org/ripple/ripple-libpp)
[![Build status](https://ci.appveyor.com/api/projects/status/idswqburp8xeqk7y?svg=true)](https://ci.appveyor.com/project/ripple/ripple-libpp)
[![codecov](https://codecov.io/gh/ripple/ripple-libpp/branch/master/graph/badge.svg)](https://codecov.io/gh/ripple/ripple-libpp)

Rippled-compatible serialization and transaction signing sample/demo

## Introduction

Demo application that uses the 
([rippled](https://github.com/ripple/rippled)).
C++ library to create, sign, and serialize
[Ripple](https://ripple.com) transactions before
submission to the Ripple Consensus Ledger
([rippled](https://github.com/ripple/rippled)).
This demonstrates much of the functionality of the
[`sign`](https://ripple.com/build/rippled-apis/#sign)
RPC function without the overhead of a JSON library,
network delays, needing to trust a 3rd party's rippled,
nor needing to run your own rippled.

## Table of contents

* [Dependencies](#dependencies)
  * [Rippled](#rippled)
  * [Other dependencies](#other-dependencies)
* [Demo](#demo)
  * [Additional dependencies](#additional-dependencies)
  * [Build and run](#build-and-run)

## Dependencies

### Rippled

ripple-libpp can download the rippled source code (using cmake
FetchContent) and build it as part of the normal cmake workflow.
To enable automatic download add the following flag to your
cmake invocation
```
    cmake -DLIBPPDEMO_DOWNLOAD_RIPPLED=ON
```
If rippled has been built locally and you do not wish to
download and build it again, then use
```
    cmake -DRipple_ROOT=/path/to/rippled/install/ \n"
    # or
    cmake -DRipple_DIR=/path/to/RippleConfig.cmake
```
CMake FetchContent does support building from an existing source tree,
so another option is to use
```
    cmake -DFETCHCONTENT_SOURCE_DIR_rippled=/path/to/local/rippled/source/tree
```

Note: even though the entire rippled source tree is included
in the submodule, only a subset of it is used by the library.

### Other dependencies

* C++14 or greater
* [Boost](http://www.boost.org/)
* [OpenSSL](https://www.openssl.org/)

## Demo

Code examples are provided in `src/test/ripple-libpp_demo.cpp`
to demonstrate how to create, sign, and verify the signature of a
transaction. Building and running this demo is an optional step to
verify that dependencies are installed and available as expected.

Note that the demo is not a comprehensive suite of tests of the
relevant rippled functionality; that is covered by rippled's unit
tests.

### Additional dependencies

In addition to the Usage [dependencies](#dependencies), building
the demo requires

* [cmake](https://cmake.org)

### Build and run

For linux and other unix-like OSes, run the following commands:

```
$ cd ${YOUR_RIPPLE_LIBPP_DIRECTORY}
$ mkdir -p build/debug
$ cd build/debug
$ cmake ../.. -DCMAKE_BUILD_TYPE=Debug
$ cmake --build .
$ ./ripplelibppdemo
```

For 64-bit Windows, open a MSBuild Command Prompt for Visual Studio
and run the following commands:

```
> cd %YOUR_RIPPLE_LIBPP_DIRECTORY%
> mkdir build
> cd build
> cmake -G"Visual Studio 15 2017 Win64" ..
> cmake --build .
> .\Debug\ripplelibppdemo.exe
```

32-bit Windows builds are not supported.
