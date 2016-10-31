# cmake file for ripple-libpp

# Include this library in your own CMake project by including the following somewhere
# below your project definition (`project(your_application)`)
#
# add_subdirectory(extras/ripple-libpp/src/unity)
#
# Customize the path as needed

cmake_minimum_required(VERSION 3.1.0)

list(APPEND CMAKE_MODULE_PATH "../../extras/rippled/Builds/CMake")
include(CMakeFuncs)

include_directories(
  ../../extras/rippled/src
  ../../extras/rippled/src/beast/include
  ../../extras/rippled/src/beast/extras)
#  ../../extras/ripple-libpp/extras/rippled/src/srcp256k1
#  ../../extras/ripple-libpp/extras/rippled/src/ed25519-donna

determine_build_type()

find_boost(
  regex
  system
  thread)

setup_build_boilerplate()

if (WIN32 OR is_xcode)
  # Rippled sources
  foreach(curdir
      beast/core
      beast/hash
      beast/utility
      basics
      core
      crypto
      json
      protocol)
    file(GLOB_RECURSE cursrcs ../../extras/rippled/src/ripple/${curdir}/*.h
      ../../extras/rippled/src/ripple/${curdir}/*.cpp)
    list(APPEND rippled_src "${cursrcs}")
    list(APPEND non_unity_srcs "${cursrcs}")
  endforeach()

  file(GLOB_RECURSE all_headers ../../src/*.hpp ../../src/*.h ../../extras/*.hpp)
  list(APPEND all_headers "../../README.md")
  list(APPEND rippled_src "${all_headers}")

  # Properties
  set_property(
    SOURCE ${non_unity_srcs}
    APPEND
    PROPERTY HEADER_FILE_ONLY
    true)
  set_property(
    SOURCE ${all_headers}
    APPEND
    PROPERTY HEADER_FILE_ONLY
    true)
    # Doesn't work
    # $<OR:$<CONFIG:Debug>,$<CONFIG:Release>>)
endif()

############################################################

if (WIN32 OR is_xcode)
  group_sources(src)
  group_sources(extras/rippled/src)
endif()

############################################################

add_with_props(lib_src ripple-libpp.cpp
  -I"${CMAKE_SOURCE_DIR}/"extras/rippled/src/secp256k1
  ${no_unused_w}
  )

add_with_props(lib_src ../../extras/rippled/src/ripple/unity/ed25519.c
  -I"${CMAKE_SOURCE_DIR}/"extras/rippled/src/ed25519-donna)

add_library(ripplelibpp OBJECT ${lib_src} ${rippled_src})

