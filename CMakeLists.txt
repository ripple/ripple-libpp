#-------------------------------------------------------------
# mandatory project setup
#-------------------------------------------------------------
cmake_minimum_required(VERSION 3.12)
project(ripple-libppdemo)

#-------------------------------------------------------------
# find_package() uses <PackageName>_ROOT variables.
#-------------------------------------------------------------
if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

#-------------------------------------------------------------
# find Rippled if possible, but don't quit on error
# (NB. turn off rippled static build flag that causes problems when finding boost etc)
#-------------------------------------------------------------
set(static OFF)
find_package(Ripple QUIET)

#if(Ripple_DIR)
#    set(date_DIR ${Ripple_DIR}/../date)
#    message("FOUND Date ${date_FOUND}")
#endif()

#-------------------------------------------------------------
# if Rippled was not found, allow a download using cmake FetchContent
#-------------------------------------------------------------
if (NOT Ripple_FOUND)
  # if neither of these two vars are set, show a helpful message
  if (NOT LIBPPDEMO_DOWNLOAD_RIPPLED AND NOT FETCHCONTENT_SOURCE_DIR_rippled)
    message(FATAL_ERROR
        "\nRippled was not found: please use -DRipple_ROOT=/path/to/rippled/install/ \n"
        "or"
        "You can download and build Rippled automatically by using -DLIBPPDEMO_DOWNLOAD_RIPPLED=ON "
        "or you can set -DFETCHCONTENT_SOURCE_DIR_rippled=/path/to/local/rippled/source/tree")
  endif()

  # if the user asked for a download, or set a local source tree
  if ("${LIBPPDEMO_DOWNLOAD_RIPPLED}" OR NOT ("${FETCHCONTENT_SOURCE_DIR_rippled}" STREQUAL ""))
      #-------------------------------------------------------------
      # Include module for downloading or finding other projects
      include(FetchContent)

      #-------------------------------------------------------------
      if (FETCHCONTENT_SOURCE_DIR_rippled)
          message("Rippled will be taken locally from ${FETCHCONTENT_SOURCE_DIR_rippled}")
      endif()

      #-------------------------------------------------------------
      # Rippled, fetch master branch by default
      #-------------------------------------------------------------
      set(FETCHCONTENT_QUIET OFF)
      FetchContent_Declare(rippled
          GIT_REPOSITORY https://github.com/ripple/rippled.git
          GIT_TAG master
          GIT_SHALLOW TRUE
      )

      FetchContent_GetProperties(rippled)

      if(NOT rippled_POPULATED)
           FetchContent_Populate(rippled)
           set(static OFF)
           add_subdirectory(${rippled_SOURCE_DIR} ${rippled_BINARY_DIR})
      endif()
  endif()
endif()

add_executable(ripplelibppdemo src/test/ripple-libpp_demo.cpp)
target_link_libraries(ripplelibppdemo
  PUBLIC
    Ripple::xrpl_core)
