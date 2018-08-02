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
  * [Rippled submodule](#rippled-submodule)
  * [Other dependencies](#other-dependencies)
* [Demo](#demo)
  * [Additional dependencies](#additional-dependencies)
  * [Build and run](#build-and-run)

## Dependencies

### Rippled submodule

ripple-libpp includes a git submodule to include the rippled
source code, which is not cloned by default. To get the
rippled source, either clone this repository using
```
$ git clone --recursive <location>
```
or after cloning, run the following commands
```
$ git submodule init
$ git submodule update
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
