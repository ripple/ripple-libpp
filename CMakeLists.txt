
cmake_minimum_required(VERSION 3.9)
project(ripple-libppdemo)

#[===========================================[
  this pulls-in the rippled source code via
  git submodule. This demo exe depends on the
  xrpl_core library which is defined by the
  rippled project.
#]===========================================]
if (MSVC)
  #[=======================================[
    HACK to patch a one-line windows-only
    bug in rippled CMakeLists.txt. This can
    be removed once rippled 1.2 is available
    and we update the submodule
  #]=======================================]
  execute_process( COMMAND powershell -Command
     "(Get-Content extras/rippled/CMakeLists.txt) \
     -replace '^exclude_if_included\\s+\\(rocksdb\\)', '##' \
     | Set-Content extras/rippled/CMakeLists.txt"
     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
endif ()
add_subdirectory(extras/rippled)

add_executable(ripplelibppdemo src/test/ripple-libpp_demo.cpp)
target_link_libraries (ripplelibppdemo Ripple::xrpl_core)
