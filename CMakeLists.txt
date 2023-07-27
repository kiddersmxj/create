cmake_minimum_required (VERSION 3.11)

project(create
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable(create src/create.cpp)

find_package(std-k REQUIRED)

target_include_directories(create
    PRIVATE inc
)

install(
    TARGETS create
    DESTINATION /usr/local/bin/
)

target_link_libraries(create
    PRIVATE std-k
)
target_compile_features(create PUBLIC cxx_std_20)

# Copyright (c) 2023, Maxamilian Kidd-May
# All rights reserved.

# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree. )";
