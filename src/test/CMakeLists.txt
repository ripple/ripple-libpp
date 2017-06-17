# cmake file for ripplelibppdemo/ripplelibpptest

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/../../Builds/CMake")

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
  SYSTEM
  ../../src
  ../../src/include
  ../../src/beast
  ../../src/beast/include
  ../../src/beast/extras
  ../../src/secp256k1/include
  ../../src/ed25519-donna)

special_build_flags()

############################################################

use_boost(
  # resist the temptation to alphabetize these. coroutine
  # must come before context.
  chrono
  coroutine
  context
  date_time
  filesystem
  program_options
  regex
  system
  thread)

use_pthread()

use_openssl(${openssl_min})

setup_build_boilerplate()

############################################################

set(demo_src ripple-libpp_demo.cpp)
set(test_src ripple-libpp_test.cpp)

############################################################

if (WIN32 OR is_xcode)
  file(GLOB_RECURSE all_headers
    src/*.hpp
    src/*.h
    Builds/*/CMakeFuncs.cmake)
  list(APPEND demo_src "${all_headers}")
  list(APPEND test_src "${all_headers}")

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

FILE(GLOB SRC_FILES ./*.c ./*/*.c ./*/*/*.c ./*/*/*/*.c)

############################################################

# Unity sources

prepend(test_unity_srcs
    ../../src/test/unity/
    protocol_test_unity.cpp)

if (WIN32 OR is_xcode)
  group_sources(src)
endif()

############################################################

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(ripplelibppdemo
    ${demo_src}
    $<TARGET_OBJECTS:ripplelibpp>)

set_startup_project(ripplelibppdemo)

target_link_libraries(ripplelibppdemo
    ${OPENSSL_LIBRARIES}
    ${SANITIZER_LIBRARIES})

link_common_libraries(ripplelibppdemo)

############################################################

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(ripplelibpptest
    ${test_src}
    ${files}
    ${test_unity_srcs}
    $<TARGET_OBJECTS:ripplelibpp>)

set_startup_project(ripplelibpptest)

target_link_libraries(ripplelibpptest
    ${OPENSSL_LIBRARIES}
    ${PROTOBUF_LIBRARIES}
    ${SANITIZER_LIBRARIES})

link_common_libraries(ripplelibpptest)
