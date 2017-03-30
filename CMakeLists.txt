# Copyright (C) 2017 Scandy
#
# THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
# KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.

cmake_minimum_required(VERSION 3.0)
project(ScandyCoreCppExamples)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# And now add the Scandy Core directory
set(ScandyCore_DIR "${ScandyCoreCppExamples_SOURCE_DIR}/dependencies/ScandyCore/")
find_package(ScandyCore)

# Scandy Core relies on C++14
set(CMAKE_CXX_STANDARD 14)

# link the ScandyCore lib directory
link_directories(
  "${ScandyCore_LIB_DIRS}"
)

# include the ScandyCore include directory
include_directories(
  "${ScandyCore_INCLUDE_DIRS}"
)

# Create the basic_usage executable
add_executable(basic_usage "basic_usage.cpp")
# Also link the ScandyCore libraries
target_link_libraries(basic_usage
  ${ScandyCore_LIBRARIES}
)

# Create the basic_usage_file_read executable
add_executable(basic_usage_file_read "basic_usage_file_read.cpp")
# Also link the ScandyCore libraries
target_link_libraries(basic_usage_file_read
  ${ScandyCore_LIBRARIES}
)

# Create the basic_usage_mesh_read executable
add_executable(basic_usage_mesh_read "basic_usage_mesh_read.cpp")
# Also link the ScandyCore libraries
target_link_libraries(basic_usage_mesh_read
  ${ScandyCore_LIBRARIES}
)