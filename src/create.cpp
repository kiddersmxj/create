#include "../inc/create.cpp"
#include <cstdlib>
#include <ostream>
#include <std-k.hpp>
#include<stdio.h>
#include<unistd.h>

int main(int argc, char** argv) {
    int HelpFlag = 0;
    int VersionFlag = 0;
    int CreateFlag = 0;
    int AddFlag = 0;
    std::string Type = "";
    std::string Name = "";
    int opt;

    // Get opt option defenitions
    struct option Opts[] = {
        { "help", no_argument, &HelpFlag, 1 },
        { "version", no_argument, &VersionFlag, 1 },
        { "create", no_argument, &CreateFlag, 1 },
        { "add", no_argument, &AddFlag, 1 },
        { "type", required_argument, NULL, 't' },
        { "name", required_argument, NULL, 'n' },
        { 0 }
    };

    // Infinite loop, to be broken when we are done parsing options
    while (1) {
        opt = getopt_long(argc, argv, "hvcat:n:", Opts, 0);

        // A return value of -1 indicates that there are no more options
        if (opt == -1) {
            if(HelpFlag && VersionFlag) {
                Usage();;
                return EXIT_FAILURE;
            }
            if(CreateFlag || AddFlag) {
                if(Type == "" || Name == "") {
                    Usage("Requires type and name to be passed");
                    return EXIT_FAILURE;
                }
            }
            break;
        } 

        switch (opt) {
        case 'h':
            HelpFlag = 1;
            break;
        case 'v':
            VersionFlag = 1;
            break;
        case 't':
            Type = optarg;
            break;
        case 'n':
            Name = optarg;
            break;
        case 'c':
            CreateFlag = 1;
            break;
        case '?':
            /* A return value of '?' indicates that an option was malformed.
             * this could mean that an unrecognized option was given, or that an
             * option which requires an argument did not include an argument.
             */
            Usage();
            return EXIT_FAILURE;
        default:
            break;
        }
    }

    // Act on flags
    if(HelpFlag) {
        Usage();
        return EXIT_SUCCESS;
    }
    if(VersionFlag) {
        PrintVersion();
        return EXIT_SUCCESS;
    }

    if(CreateFlag) {
        if(Type == "cpp") {
            SetupDir(Name);
            BuildFS(Name, Type);
        }
    }
    if(AddFlag) {
        if(Type == "cpp") {
            Add(Name, 0, 0);
        }
    }

    return 0;
}

void Usage() {
    std::cout << UsageNotes << std::endl;
}

void Usage(std::string Message) {
    std::cout << Message << std::endl;
    std::cout << UsageNotes << std::endl;
}

void PrintVersion() {
    std::cout << ProgramName << ": version " << Version << std::endl;
}

void SetupDir(std::string Name) {
    k::MkDir(Name);
    const char *Path = Name.c_str();
    chdir(Path);
}

void BuildFS(std::string Name, std::string Type) {
    Add(Name, "cpp", 1);
    Add("config", "cpp", 0);
    Add(Name, "cmake", 0);
    Add(Name, "readme", 0);
    Add("", "gdb", 0);
    Add("", "gitignore", 0);
    Add("", "license", 0);
    k::ExecCmd("git init" + Sh + "; git branch -m main" + Sh \
            + Sh + "; shopt -s dotglob" + Sh + "; git add *" + Sh + \
            "; git remote add master " + BaseURL + "/" + Name + \
            Sh + "; git commit -am \"initial commit\"" + Sh + "; git pull"\
            + Sh + "; git branch  --set-upstream-to=master/main main" + Sh + \
            "git push --set-upstream master main" + Sh, 0, 0);
}

void Add(std::string Name, std::string Type, bool Main) {
    if(Type == "cpp") {
        k::MkDir("inc");
        k::MkDir("src");
        if(Name == "config") { // config file
        k::WriteFileLines("#ifndef Kconfig\n#define Kconfig\
                            \n\n#include <iostream>\n\n#endif" \
                            + LicenseFooterSlash, "inc/config.hpp");
        }
        k::WriteFileLines("#include \"../inc/" + Name + ".hpp\"\n\n \
                            int main(int argc, char** argv) { \n\treturn 0;\n}" \
                            + LicenseFooterSlash, "src/" + Name + ".cpp");
        k::WriteFileLines("#ifndef K" + Name + Name + "\n#define K" + Name + Name \
                            + "\n\n#include \"config.hpp\"\n\n#endif" \
                            + LicenseFooterSlash, "inc/" + Name + ".hpp");
    } else if(Type == "cmake") {
        k::WriteFileLines(Install + LicenseFooterHash, "install.sh");
        k::ExecCmd("chmod +x install.sh", 0, 0);
        k::WriteFileLines(CMake1 + Name + CMake2 + Name + " src/" \
                            + Name + ".cpp" + CMake3 + Name + CMake4 + Name + CMake5 \
                            + Name + CMake6 + LicenseFooterHash, "CMakeLists.txt");
        k::MkDir(".github");
        k::MkDir(".github/workflows");
        k::WriteFileLines(Action, ".github/workflows/cmake.yml");
    } else if(Type == "license") {
        k::WriteFileLines(License, "LICENSE");
    } else if(Type == "gdb") {
        k::WriteFileLines(GDB, ".gdbinit");
    } else if(Type == "gitignore") {
        k::WriteFileLines(GitIgnore, ".gitignore");
    } else if(Type == "readme") {
        std::string ReadMe = "#" + Name + "\n\n" + ReadMe1 + Name + ReadMe2 + Name + ReadMe3;
        k::WriteFileLines(ReadMe, "README.md");
    }
}
