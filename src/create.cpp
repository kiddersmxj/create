#include "../inc/create.hpp"
#include <cstdlib>
#include <ostream>
#include <std-k.hpp>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int HelpFlag = 0;
    int VersionFlag = 0;
    int CreateFlag = 0;
    int AddFlag = 0;
    int MainFlag = 0;
    std::string Type = "";
    std::string Name = "";
    int opt;

    // Get opt option defenitions
    struct option Opts[] = {
        { "help", no_argument, &HelpFlag, 1 },
        { "version", no_argument, &VersionFlag, 1 },
        { "create", no_argument, &CreateFlag, 1 },
        { "add", no_argument, &AddFlag, 1 },
        { "add", no_argument, &MainFlag, 1 },
        { "type", required_argument, NULL, 't' },
        { "name", required_argument, NULL, 'n' },
        { 0 }
    };

    // Infinite loop, to be broken when we are done parsing options
    while (1) {
        opt = getopt_long(argc, argv, "hvcamt:n:", Opts, 0);

        // A return value of -1 indicates that there are no more options
        if (opt == -1) {
            if(HelpFlag && VersionFlag) {
                Usage();;
                return EXIT_FAILURE;
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
        case 'a':
            AddFlag = 1;
            break;
        case 'm':
            MainFlag = 1;
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
            try { BuildFS(Name, Type); }
            catch(const char *Message) {
                Usage(Message);
                return EXIT_FAILURE;
            }
        }
    }
    if(AddFlag) {
        try { Add(Name, Type, MainFlag); }
        catch(const char *Message) {
            Usage(Message);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
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
    SetupDir(Name);
    Add(Name, "cpp", 1);
    Add("config", "cpp", 0);
    Add(Name, "cmake", 0);
    Add(Name, "readme", 0);
    Add("", "gdb", 0);
    Add("", "gitignore", 0);
    Add("", "license", 0);
    CreateRepo(Name);
}

void Add(std::string Name, std::string Type, bool Main) {
    if(Type == "cpp") {
        k::MkDir("inc");
        k::MkDir("src");
        if(Name == "config") { // config file
            k::WriteFileLines("#ifndef Kconfig\n#define Kconfig\
                                \n\n#include <iostream>\n\n#endif" \
                                + LicenseFooterSlash, "inc/config.hpp");
            Git("add inc/config.hpp");
        } else {
            if(Name == "") throw "Passing a name is required for this operation";
            if(Main) { // TODO add classes/stucts
                k::WriteFileLines("#include \"../inc/" + Name + ".hpp\"\n\n" \
                                    + "int main(int argc, char** argv) { \n\treturn 0;\n}" \
                                    + LicenseFooterSlash, "src/" + Name + ".cpp");
                k::WriteFileLines("#ifndef K" + Name + Name + "\n#define K" + Name + Name \
                                    + "\n\n#include \"config.hpp\"\n\n#endif" \
                                    + LicenseFooterSlash, "inc/" + Name + ".hpp");
            } else {
                k::WriteFileLines("#include \"../inc/" + Name + ".hpp\"" \
                                    + LicenseFooterSlash, "src/" + Name + ".cpp");
                k::WriteFileLines("#ifndef K" + Name + Name + "\n#define K" + Name + Name \
                                    + "\n\n#endif" + LicenseFooterSlash, "inc/" + Name + ".hpp");
            }
            Git("add src/" + Name + ".cpp");
            Git("add inc/" + Name + ".hpp");
        }
    } else if(Type == "cmake") {
        if(Name == "") throw "Passing a name is required for this operation";
        k::WriteFileLines(Install + LicenseFooterHash, "install.sh");
        int result = k::ExecCmd("chmod +x install.sh");
        k::WriteFileLines(CMake1 + Name + CMake2 + Name + " src/" \
                            + Name + ".cpp" + CMake3 + Name + CMake4 + Name + CMake5 \
                            + Name + CMake6 + LicenseFooterHash, "CMakeLists.txt");
        k::MkDir(".github");
        k::MkDir(".github/workflows");
        k::WriteFileLines(Action, ".github/workflows/cmake.yml");
        Git("add CMakeLists.txt");
        Git("add .github");
    } else if(Type == "license") {
        k::WriteFileLines(License, "LICENSE");
        Git("add LICENSE");
    } else if(Type == "gdb") {
        k::WriteFileLines(GDB, ".gdbinit");
        Git("add .gdbinit");
    } else if(Type == "gitignore") {
        k::WriteFileLines(GitIgnore, ".gitignore");
        Git("add .gitignore");
    } else if(Type == "readme") {
        if(Name == "") throw "Passing a name is required for this operation";
        std::string ReadMe = "# " + Name + " " + ReadMe1 + Name + ReadMe2 + Name + ReadMe3;
        k::WriteFileLines(ReadMe, "README.md");
        Git("add README.md");
    } else if(Type == "bash" || Type == "sh") {
        if(Name == "") throw "Passing a name is required for this operation";
        std::string Bash = "#!/bin/bash\n";
        k::WriteFileLines(Bash, Name + ".sh");
        int result = k::ExecCmd("chmod +x " + Name + ".sh");
        Git("add" + Name + ".sh");
    } else
        throw "Passing a type is required for this operation";
    return;
}

bool Git(std::string Cmd) {
    Cmd =  "git " + Cmd + Sh;
    std::string Output;
    int result = k::ExecCmd(Cmd, Output);
    /* std::cout << "0: " << Output << std::endl; */
    return result;
}

bool CreateRepo(std::string Name) {
    Git("init");
    Git("branch -m main");
    Git("remote add master " + BaseURL + "/" + Name);
    Git("add *; shopt -s dotglob; git add *");
    Git("commit -am \"intial commit\"");
    Git("pull");
    Git("branch  --set-upstream-to=master/main main");
    Git("push --set-upstream master main");
    return 0;
}
