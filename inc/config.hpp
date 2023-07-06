#include <cstdlib>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

const std::string ProgramName = "create";
const time_t now = time(0);
const tm *ltm = localtime(&now);
const std::string Year = std::to_string(1900 + ltm->tm_year);
const std::string UsageNotes = R"(usage: create [ -t/--type="language" ]
options:
    -h / --help         show help and usage notes
    -v / --version      print version and exit
    -t / --type         designate langage (cpp)
    -n / --name         name program
    -c / --create       create program and fs
    -a / --add          add file (respective header)
    -m / --main         for c/cpp add a main (respective header))";
const std::string Version = "0.0.0";
const std::string BaseURL = getenv("ghk");
const std::string Sh = R"( > /dev/null 2>&1)";

const std::string CMake1 = { R"(cmake_minimum_required (VERSION 3.11)
 
# --- Fetch libs --------------------------------------------------------------

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(std-k
  GIT_REPOSITORY https://github.com/kiddersmxj/std-k
  GIT_TAG main
)
 
FetchContent_GetProperties(std-k)
if(NOT std-k_POPULATED)
  FetchContent_Populate(std-k)
  add_subdirectory(${std-k_SOURCE_DIR} ${std-k_BINARY_DIR})
endif()
 
# ------------------------------------------------------------------------------

# Set cpp standard (with flags for gh actions)
set (CMAKE_CXX_STANDARD 20)
set(CXXFLAGS "-std=c++20")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")

project()" }; // Name

const std::string CMake2 = R"(
    LANGUAGES CXX
    VERSION 0.0.0
)

add_executable()"; // Name Name.cpp

const std::string CMake3 = R"()

target_include_directories()"; // Name

const std::string CMake4 = R"( 
    PRIVATE src
    SHARED ${std-k_SOURCE_DIR}/inc
)

install(
    TARGETS )"; // Name

const std::string CMake5 = R"(
    DESTINATION /usr/local/bin/
)

link_directories(${std-k_BINARY_DIR})
target_link_libraries()"; // Name

const std::string CMake6 = R"(
    PUBLIC std-k
))";

const std::string License = R"(MIT License

Copyright (c) )" + Year + R"( Maxamilian Kidd-May

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.)";

const std::string LicenseFooterHash = R"(

# Copyright (c) 2023, Maxamilian Kidd-May
# All rights reserved.

# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree. )";

const std::string LicenseFooterSlash = R"(

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. )";

const std::string Install = R"(#!/bin/bash

if [[ $1 != "" ]]; then
    ARGS="-D$1"
fi

cmake -B build $ARGS && cmake --build build && sudo cmake --install build)";

const std::string Action = R"(name: CMake

on:
  push:
    branches: [ "main" ]
  pull_request:
    branches: [ "main" ]

env:
  # Customize the CMake build type here (Release, Debug, RelWithDebInfo, etc.)
  BUILD_TYPE: Release

jobs:
  build:
    # The CMake configure and build commands are platform agnostic and should work equally well on Windows or Mac.
    # You can convert this to a matrix build if you need cross-platform coverage.
    # See: https://docs.github.com/en/free-pro-team@latest/actions/learn-github-actions/managing-complex-workflows#using-a-build-matrix
    runs-on: ubuntu-latest

    steps:
    - name: Checkout repo
      uses: actions/checkout@master
    - name: Build using CMake
      # Configure CMake in a 'build' subdirectory. `CMAKE_BUILD_TYPE` is only required if you are using a single-configuration generator such as make.
      # See https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html?highlight=cmake_build_type
      run: ./install.sh)";

const std::string GDB = R"(b Breakpoint)";

const std::string GitIgnore = ".gdbinit\nbuild";

const std::string BaseUsage = R"(
options:
    -h / --help         show help and usage notes
    -v / --version      print version and exit
)";

const std::string ReadMe1 = R"([![CMake](https://img.shields.io/github/actions/workflow/status/kiddersmxj/)"; // Name
const std::string ReadMe2 = R"(/cmake.yml?style=for-the-badge)](https://github.com/kiddersmxj/)"; // Name
const std::string ReadMe3 = R"(/actions/workflows/cmake.yml))";
