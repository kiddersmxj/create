#include <iostream>
#include <vector>
#include <ctime>

#include <std-k.hpp>

const std::string ProgramName = "create";
const time_t now = time(0);
inline const tm *ltm = localtime(&now);
const std::string Year = std::to_string(1900 + ltm->tm_year);
const std::string UsageNotes = R"(usage: create [ -h/-v ] [ -a/-c ] [ -t language ]
              [ -n name ] [ -C/-S/-M ] [ -f ]
options:
    -h / --help         show help and usage notes
    -v / --version      print version and exit
    -t / --type         designate langage (pass '-t list' for list)
    -n / --name         name program/file
    -c / --create       create program and fs
    -a / --add          add file with respective header if required
    -m / --main         for c/cpp add a main & respective header
    -C / --class        for c/cpp add a class & respective header
    -S / --struct       for c/cpp add a struct header
    -f / --force        force even if files exist)";
const std::string Version = "1.0.0";
const std::vector<std::string> TypeList = { "cpp", "cmake", "license", "gdb", "gitignore", "readme", "bash", "sh", "python", "py" };
const std::string BaseURL = getenv("ghk");
const std::string Sh = R"( > /dev/null 2>&1)";

const std::string BasePath = HOME + "/devel/";
const std::string FileStandardsPath = HOME + "/.cache/create/filestandards/";

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

const std::string GDB = R"(b Breakpoint)";

const std::string GitIgnore = ".gdbinit\nbuild";

const std::string Python = R"(#!/usr/bin/python

if __name__ == "__main__":
   main())";

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. )";
