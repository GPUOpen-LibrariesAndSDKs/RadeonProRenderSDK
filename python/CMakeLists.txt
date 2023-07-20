
project(PyRPR) 
cmake_minimum_required(VERSION 3.18...3.22)
find_package(Python 3.9 COMPONENTS Interpreter Development.Module REQUIRED)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)


include_directories( 
  ../RadeonProRender/inc
)

link_directories(
  ../RadeonProRender/libWin64
)


nanobind_add_module(rpr bind_rpr.cpp bind_common.cpp bind_common.h)
nanobind_add_module(rprs bind_rprs.cpp bind_common.cpp bind_common.h)
nanobind_add_module(rprgltf bind_rprgltf.cpp bind_common.cpp bind_common.h)

target_link_libraries(rpr PUBLIC RadeonProRender64.lib)
target_link_libraries(rprs PUBLIC RprLoadStore64.lib)
target_link_libraries(rprgltf PUBLIC ProRenderGLTF.lib)

