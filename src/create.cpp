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
    std::string Type = "";
    std::string Name = "";
    int opt;

    // Get opt option defenitions
    struct option Opts[] = {
        { "help", no_argument, &HelpFlag, 1 },
        { "version", no_argument, &VersionFlag, 1 },
        { "create", no_argument, &CreateFlag, 1 },
        { "type", required_argument, NULL, 't' },
        { "name", required_argument, NULL, 'n' },
        { 0 }
    };

    // Infinite loop, to be broken when we are done parsing options
    while (1) {
        opt = getopt_long(argc, argv, "hvct:n:", Opts, 0);

        // A return value of -1 indicates that there are no more options
        if (opt == -1) {
            if(HelpFlag && VersionFlag) {
                Usage();;
                return EXIT_FAILURE;
            }
            if(CreateFlag) {
                if(Type == "" || Name == "") {
                    Usage("Creating requires type and name to be passed");
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
    k::MkDir("inc");
    k::MkDir("src");
    k::MkDir(".github");
    k::MkDir(".github/workflows");
    std::string CMakeLists = CMake1 + Name + CMake2 + Name + " src/" + \
            Name + "." + Type  + CMake3 + Name + CMake4 + Name + CMake5 \
            + Name + CMake6 + LicenseFooterHash;
    std::string Config = "#ifndef Kconfig" + Name + "\n#define Kconfig" + Name \
                          + "\n\n#include <iostream>\n\n#endif" + LicenseFooterSlash;
    std::string Header = "#ifndef K" + Name + Name + "\n#define K" + Name + Name \
                          + "\n\n#include \"config.hpp\"\n\n#endif" + LicenseFooterSlash;
    std::string Main = "#include \"../inc/" + Name + ".hpp\"\n\nint main(int argc, char** argv) { \
                        \n\treturn 0;\n}" + LicenseFooterSlash;
    std::string ReadMe = "#" + Name + "\n\n" + ReadMe1 + Name + ReadMe2 + Name + ReadMe3;
    k::WriteFileLines(CMakeLists, "CMakeLists.txt");
    k::WriteFileLines(Config, "inc/config.hpp");
    k::WriteFileLines(Header, "inc/" + Name + ".hpp");
    k::WriteFileLines(Main, "src/" + Name + ".cpp");
    k::WriteFileLines(License, "LICENSE");
    k::WriteFileLines(Install + LicenseFooterHash, "install.sh");
    k::ExecCmd("chmod +x install.sh", 0, 0);
    k::WriteFileLines(Action, ".github/workflows/cmake.yml");
    k::WriteFileLines(GDB, ".gdbinit");
    k::WriteFileLines(GitIgnore, ".gitignore");
    k::WriteFileLines(ReadMe, "README.md");
    k::ExecCmd("git init" + Sh + "; git branch -m main" + Sh \
            + Sh + "; shopt -s dotglob" + Sh + "; git add *" + Sh + \
            "; git remote add master " + BaseURL + "/" + Name + \
            Sh + "; git commit -am \"initial commit\"" + Sh + "; git pull"\
            + Sh + "; git branch  --set-upstream-to=master/main main" + Sh + \
            "git push --set-upstream master main" + Sh, 0, 0);
}
