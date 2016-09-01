# This is an demonstration cmake build file for rclpplib
#
# cmake support in rclpplib. Currently supports:
#
#  * debug/release
#  * sanitizer builds
#  * optional release build with assert turned on
#  * `target` variable to easily set compiler/debug
#     (i.e. -Dtarget=gcc.debug)
#  * gcc/clang/visual studio/xcode
#  * linux/mac/win
#  * gcc 4 ABI, when needed
#  * ninja builds
#  * check openssl version on linux
#  * static builds (swd TBD: needs to be tested by building & deploying on different systems)
#
# TBD:
#  * jemalloc support
#  * count
#
# Notes:
#  * Use the -G"Visual Studio 14 2015 Win64" generator on Windows. Without this
#    a 32-bit project will be created. There is no way to set the generator or
#    force a 64-bit build in CMakeLists.txt (setting CMAKE_GENERATOR_PLATFORM won't work).
#    The best solution may be to wrap cmake with a script.
#
#  * It is not possible to generate a visual studio project on linux or
#    mac. The visual studio generator is only available on windows.
#
#  * Language is _much_ worse than python, poor documentation and "quirky"
#    language support (for example, generator expressions can only be used
#    in limited contexts and seem to work differently based on
#    context (set_property can set multiple values, add_compile_options
#    can not/or is buggy)
#
#  * Could not call out to `sed` because cmake messed with the regular
#    expression before calling the external command. I did not see a way
#    around this.
#
#  * Makefile generators want to be single target. It wants a separate
#    directory for each target type. I saw some mentions on the web for
#    ways around this bug haven't look into it. The visual studio project
#    does support debug/release configurations in the same project (but
#    not unity/non-unity).

############################################################

cmake_minimum_required(VERSION 3.1.0)

include(extras/rippled/CMakeFuncs.cmake)

set(openssl_min 1.0.2)

parse_target()

set(CMAKE_CURRENT_BINARY_DIR
  "${CMAKE_CURRENT_BINARY_DIR}/build/${target}.cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

if (nonunity)
  set(target "" CACHE STRING "Build target. Nounity not supported" FORCE)
  message(FATAL_ERROR "Nonunity builds not supported")
endif()

setup_build_cache()

project(ripple-libpp)

############################################################

determine_build_type()

check_gcc4_abi()

############################################################

include_directories(
  extras/rippled/src
  extras/rippled/src/beast/include
  extras/rippled/src/beast/extras)

special_build_flags()

############################################################

find_boost(
  regex
  system
  thread)

find_pthread()

find_openssl(${openssl_min})

setup_build_boilerplate()

############################################################

prepend(lib_unity_srcs
  src/test/
  ripple-libpp_demo.cpp)

list(APPEND lib_src ${lib_unity_srcs})

set_property(
  SOURCE ${lib_unity_srcs}
  APPEND
  PROPERTY HEADER_FILE_ONLY
  false)
# Doesn't work
# $<OR:$<CONFIG:DebugClassic>,$<CONFIG:ReleaseClassic>>)

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
    file(GLOB_RECURSE cursrcs extras/rippled/src/ripple/${curdir}/*.h
      extras/rippled/src/ripple/${curdir}/*.cpp)
    list(APPEND lib_src "${cursrcs}")
    list(APPEND non_unity_srcs "${cursrcs}")
  endforeach()

  # Properties
  set_property(
    SOURCE ${non_unity_srcs}
    APPEND
    PROPERTY HEADER_FILE_ONLY
    true)
    # Doesn't work
    # $<OR:$<CONFIG:Debug>,$<CONFIG:Release>>)
endif()

############################################################

if (NOT is_msvc)
  set(no_unused_w -Wno-unused-function)
else()
  unset(no_unused_w)
endif()

add_with_props("lib_src" src/unity/ripple-libpp.cpp
  -I"${CMAKE_SOURCE_DIR}/"extras/rippled/src/secp256k1
  ${no_unused_w}
  )

add_with_props("lib_src" extras/rippled/src/ripple/unity/ed25519.c
  -I"${CMAKE_SOURCE_DIR}/"extras/rippled/src/ed25519-donna)

############################################################

if (WIN32 OR is_xcode)
  group_sources(src)
  group_sources(extras/rippled/src)
endif()

add_executable(ripplelibppdemo ${lib_src})

create_build_folder(ripplelibppdemo)

set_startup_project(ripplelibppdemo)

target_link_libraries(ripplelibppdemo
  ${OPENSSL_LIBRARIES} ${SANITIZER_LIBRARIES})


foreach(cur_project ripplelibppdemo)
  if (NOT WIN32)
    target_link_libraries(${cur_project} ${Boost_LIBRARIES})
    target_link_libraries(${cur_project} dl)
    target_link_libraries(${cur_project} Threads::Threads)
    if (APPLE)
      find_library(app_kit AppKit)
      find_library(foundation Foundation)
      target_link_libraries(${cur_project}
        crypto ssl ${app_kit} ${foundation})
    else()
      target_link_libraries(${cur_project} rt)
    endif()
  else(NOT WIN32)
    target_link_libraries(${cur_project}
      $<$<OR:$<CONFIG:Debug>,$<CONFIG:DebugClassic>>:VC/static/ssleay32MTd>
      $<$<OR:$<CONFIG:Debug>,$<CONFIG:DebugClassic>>:VC/static/libeay32MTd>)
    target_link_libraries(${cur_project}
      $<$<OR:$<CONFIG:Release>,$<CONFIG:ReleaseClassic>>:VC/static/ssleay32MT>
      $<$<OR:$<CONFIG:Release>,$<CONFIG:ReleaseClassic>>:VC/static/libeay32MT>)
    target_link_libraries(${cur_project}
      legacy_stdio_definitions.lib Shlwapi kernel32 user32 gdi32 winspool comdlg32
      advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32)
  endif (NOT WIN32)
endforeach()
