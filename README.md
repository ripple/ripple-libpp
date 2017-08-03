# ripple-libpp

[![Build Status](https://travis-ci.org/ripple/ripple-libpp.svg?branch=master)](https://travis-ci.org/ripple/ripple-libpp)
[![Build status](https://ci.appveyor.com/api/projects/status/idswqburp8xeqk7y?svg=true)](https://ci.appveyor.com/project/ripple/ripple-libpp)
[![codecov](https://codecov.io/gh/ripple/ripple-libpp/branch/master/graph/badge.svg)](https://codecov.io/gh/ripple/ripple-libpp)

Rippled-compatible serialization and transaction signing library

## Introduction

C++ library to create, sign, and serialize
[Ripple](https://ripple.com) transactions
before submission to the Ripple Consensus Ledger
([rippled](https://github.com/ripple/rippled)).
Duplicates much of the functionality of the
[`sign`](https://ripple.com/build/rippled-apis/#sign)
RPC function without the overhead of a JSON library,
network delays, needing to trust a 3rd party's rippled,
nor needing to run your own rippled.

## Table of contents

* [Dependencies](#dependencies)
  * [Rippled submodule](#rippled-submodule)
  * [Other dependencies](#other-dependencies)
* [Installation](#installation)
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

## Installation

CMake project installation example:

1. Copy, clone, or submodule the library into your project, eg:

    ```
    / My Project
    	/ extras
			/ ripple-libpp
    			**library tree**
		/ src
			**program tree**
		CMakeLists.txt
	```
2. Edit your `CMakeLists.txt` file to include the ripple-libpp config,
	and the include directories you'll need in your source code:

	```
	...
	add_subdirectory(extras/ripple-libpp/src/unity)

	# Include paths
	include_directories(
        SYSTEM
		extras/ripple-libpp/extras/rippled/src
		extras/ripple-libpp/extras/rippled/src/beast/include
		extras/ripple-libpp/extras/rippled/src/beast/extras
	)
	```

3. If you don't already have the boost and OpenSSL libraries included in 
	your project, you can use the utility functions provided by rippled

	```
	list(APPEND CMAKE_MODULE_PATH "extras/ripple-libpp/extras/rippled/Builds/CMake")
	include(CMakeFuncs)

	# Boost library
	use_boost(
	    regex
	    system
	    thread)

	target_link_libraries(${YOUR_APP_NAME} ${Boost_LIBRARIES})

	# OpenSSL library
	set(openssl_min 1.0.2)
	use_openssl(${openssl_min})

	target_link_libraries(${YOUR_APP_NAME} ${OPENSSL_LIBRARIES})
	...
	```

## Demo

Some code examples are provided in `src/test/ripple-libpp_demo.cpp`
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
$ mkdir -p build/gcc.debug
$ cd build/gcc.debug
$ cmake ../.. -Dtarget=gcc.debug
$ cmake --build .
$ ./ripplelibppdemo
$ ./ripplelibpptest
```

For 64-bit Windows, open a MSBuild Command Prompt for Visual Studio
and run the following commands:

```
> cd %YOUR_RIPPLE_LIBPP_DIRECTORY%
> mkdir build
> cd build
> cmake -G"Visual Studio 14 2015 Win64" ..
> cmake --build .
> .\Debug\ripplelibppdemo.exe
```

32-bit Windows builds are not officially supported.
