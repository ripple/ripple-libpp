if (WIN32 OR is_xcode)
  message("Rippled sources")
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
  message("group_sources ${PROJECT_SOURCE_DIR}")
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

