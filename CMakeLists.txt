cmake_minimum_required (VERSION 3.11)
 
# --- Fetch libs --------------------------------------------------------------

# include(FetchContent)

# set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
# FetchContent_Declare(std-k
#   GIT_REPOSITORY https://github.com/kiddersmxj/std-k
#   GIT_TAG main
# )
 
# FetchContent_GetProperties(std-k)
# if(NOT std-k_POPULATED)
#   FetchContent_Populate(std-k)
#   add_subdirectory(${std-k_SOURCE_DIR} ${std-k_BINARY_DIR})
# endif()
 
# ------------------------------------------------------------------------------

project(create
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(create src/create.cpp)

# Find the shared_library package
find_package(std-k REQUIRED)

target_include_directories(create
    PRIVATE inc
    # SHARED ${std-k_SOURCE_DIR}/inc
)

install(
    TARGETS create
    DESTINATION /usr/local/bin/
)

# link_directories(${std-k_BINARY_DIR})
target_link_libraries(create
    PRIVATE std-k
)
target_compile_features(create PUBLIC cxx_std_20)

# Copyright (c) 2023, Maxamilian Kidd-May
# All rights reserved.

# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree. )";
