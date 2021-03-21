cmake_minimum_required(VERSION 3.15)
project(mesh-util)

# option(MESH_UTIL_BUILD_TOOLS "Build Tools" ON)
# if(MESH_UTIL_BUILD_TOOLS)

# endif()

add_library(mesh-util INTERFACE)
target_include_directories(mesh-util INTERFACE include/)
