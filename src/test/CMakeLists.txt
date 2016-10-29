# cmake file for ripplelibppdemo

list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/../../extras/rippled/Builds/CMake")
include(CMakeFuncs)

set(openssl_min 1.0.2)

parse_target()

if (nonunity)
  set(target "" CACHE STRING "Build target. Nounity not supported" FORCE)
  message(FATAL_ERROR "Nonunity builds not supported")
endif()

setup_build_cache()

############################################################

determine_build_type()

check_gcc4_abi()

############################################################

include_directories(
  ../../extras/rippled/src
  ../../extras/rippled/src/beast/include
  ../../extras/rippled/src/beast/extras)
#  extras/ripple-libpp/extras/rippled/src/srcp256k1
#  extras/ripple-libpp/extras/rippled/src/ed25519-donna

special_build_flags()

############################################################

use_boost(
  regex
  system
  thread)

use_pthread()

use_openssl(${openssl_min})

setup_build_boilerplate()

############################################################

set(demo_src ripple-libpp_demo.cpp)

############################################################

if (WIN32 OR is_xcode)
  file(GLOB_RECURSE all_headers src/*.hpp src/*.h
    ../../extras/rippled/src/*.hpp ../../extras/rippled/src/*.h
    ../../extras/*/CMakeFuncs.cmake)
  list(APPEND demo_src "${all_headers}")

  # Properties
  set_property(
    SOURCE ${all_headers}
    APPEND
    PROPERTY HEADER_FILE_ONLY
    true)
endif()

if (NOT is_msvc)
  set(no_unused_w -Wno-unused-function)
else()
  unset(no_unused_w)
endif()

############################################################

if (WIN32 OR is_xcode)
  group_sources(src)
  group_sources(extras/rippled/src)
  group_sources(extras/rippled/Builds)
endif()

############################################################

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(ripplelibppdemo ${demo_src} $<TARGET_OBJECTS:ripplelibpp>)

set_startup_project(ripplelibppdemo)

target_link_libraries(ripplelibppdemo
  ${OPENSSL_LIBRARIES} ${SANITIZER_LIBRARIES})

link_common_libraries(ripplelibppdemo)
