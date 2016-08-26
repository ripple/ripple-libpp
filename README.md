# rclpplib

[![Build Status](https://travis-ci.org/ximinez/ripple-libpp.svg?branch=develop)](https://travis-ci.org/ximinez/ripple-libpp)
[![Build status](https://ci.appveyor.com/api/projects/status/tjm76fspwaq3te8n?svg=true)](https://ci.appveyor.com/project/ximinez/ripple-libpp)
[![codecov](https://codecov.io/gh/ximinez/ripple-libpp/branch/develop/graph/badge.svg)](https://codecov.io/gh/ximinez/ripple-libpp)

Rippled-compatible signing and serialization library

## Introduction

C++ library to create, sign, and serialize
[Ripple](https://ripple.com) transactions
before submission to the Ripple Consensus Ledger
([rippled](https://github.com/ripple/rippled)) network.
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

rclpplib includes a git submodule to include the rippled
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

Note: only a subset of the rippled source code is included
by the library.

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
    		/ rclpplib
    			**library tree**
		/ src
			**program tree**
		CMakeLists.txt
	```
2. Edit your `CMakeLists.txt` file to include the necessary unity
	source files, external libraries, and include directories:

	```
	...
	# Include paths
	include_directories(
		${YOUR_OTHER_INCLUDES}
		extras/rclpplib/extras/rippled/src
		extras/rclpplib/extras/rippled/src/srcp256k1
		extras/rclpplib/extras/rippled/src/ed25519-donna
		extras/rclpplib/extras/rippled/src/beast/include
		extras/rclpplib/extras/rippled/src/beast/extras
	)

	# Source files
	add_executable(${YOUR_APP_NAME}
		${YOUR_SOURCE_FILES}
		extras/rclpplib/src/unity/ripple-libpp.cpp
		extras/rclpplib/extras/rippled/src/ripple/unity/ed25519.c
	)

	# Boost library (if not already included)
	if (NOT WIN32)
	  set(Boost_USE_STATIC_LIBS on)
	  set(Boost_USE_MULTITHREADED on)
	  set(Boost_USE_STATIC_RUNTIME off)
	  find_package(Boost COMPONENTS
	    regex
	    system
	    thread)

	  if (Boost_FOUND)
	    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
	  else()
	    message(FATAL_ERROR "Boost not found")
	  endif()
	else(DEFINED ENV{BOOST_ROOT})
	  include_directories(SYSTEM $ENV{BOOST_ROOT})
	  link_directories($ENV{BOOST_ROOT}/stage/lib)
	endif()

	target_link_libraries(${YOUR_APP_NAME} ${Boost_LIBRARIES})

	# OpenSSL library (if not already included)
	set(openssl_min 1.0.2)
	if (WIN32)
	  if (DEFINED ENV{OPENSSL_ROOT})
	    include_directories($ENV{OPENSSL_ROOT}/include)
	    link_directories($ENV{OPENSSL_ROOT}/lib)
	  endif()
	else()
	  if (static)
	    set(tmp CMAKE_FIND_LIBRARY_SUFFIXES)
	    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
	  endif()

	  find_package(OpenSSL)

	  if (static)
	    set(CMAKE_FIND_LIBRARY_SUFFIXES tmp)
	  endif()

	  if (OPENSSL_FOUND)
	    include_directories(${OPENSSL_INCLUDE_DIR})
	  else()
	    message(FATAL_ERROR "OpenSSL not found")
	  endif()
	  if (UNIX AND NOT APPLE AND ${OPENSSL_VERSION} VERSION_LESS ${openssl_min})
	    message(FATAL_ERROR
	      "Your openssl is Version: ${OPENSSL_VERSION}, ripple-libpp requires ${openssl_min} or better.")
	  endif()
	endif()

	target_link_libraries(${YOUR_APP_NAME} ${OPENSSL_LIBRARIES})
	...
	```

## Demo

Some examples are code provided in `src/test/ripple-libpp_demo.cpp`
to demonstrate how to create, sign, and verify the signature of a
transaction. Building and running this demo is an optional step to
verify that dependencies are installed and available as expected.

Note that the demo is not a comprehensive suite of tests of the
relevent rippled functionality; that is covered by rippled's unit
tests.

### Additional dependencies

In addition to the Usage [dependencies](#dependencies), building
the demo requires

* [cmake](https://cmake.org)

### Build and run

For linux and other unix-like OSes, run the following commands:

```
$ cd ${YOUR_RCLPPLIB_DIRECTORY}
$ cmake .
$ make
$ ./ripplelibppdemo
```

For 64-bit Windows, open a MSBuild Command Prompt for Visual Studio
and run the following commands:

```
> cd %YOUR_RCLPPLIB_DIRECTORY%
> cmake -G"Visual Studio 14 2015 Win64"
> msbuild ripplelibppdemo.vcxproj
> Debug\ripplelibppdemo.exe
```
