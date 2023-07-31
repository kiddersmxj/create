cmake_minimum_required(VERSION 3.11)
 
# --- Fetch libs --------------------------------------------------------------
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.11)

project($name$
  LANGUAGES CXX
  VERSION 1.0.0
)

add_executable($name$ src/$name$.cpp)

find_package(std-k REQUIRED)

target_include_directories($name$
    PRIVATE inc
)

install(
    TARGETS $name$
    DESTINATION /usr/local/bin/
)

target_link_libraries($name$
    PRIVATE std-k
)
target_compile_features($name$ PUBLIC cxx_std_20)

# Copyright (c) 2023, Maxamilian Kidd-May
# All rights reserved.

# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.
