#include "../inc/create.hpp"
namespace fs = std::filesystem;

int main(int argc, char** argv) {
    int HelpFlag = 0;
    int VersionFlag = 0;
    int CreateFlag = 0;
    int AddFlag = 0;
    int ForceFlag = 0;
    int TestFlag = 0;
    bool Force = 0;
    int AdditionalFlag = 0;
    std::string Additional = "";
    std::string Type = "";
    std::string Name = "";
    int opt;

    // Get opt option defenitions
    struct option Opts[] = {
        { "help", no_argument, &HelpFlag, 1 },
        { "version", no_argument, &VersionFlag, 1 },
        { "create", no_argument, &CreateFlag, 1 },
        { "add", no_argument, &AddFlag, 1 },
        { "force", no_argument, &ForceFlag, 1 },
        { "main", no_argument, &AdditionalFlag, 1 },
        { "class", no_argument, &AdditionalFlag, 1 },
        { "struct", no_argument, &AdditionalFlag, 1 },
        { "type", required_argument, NULL, 't' },
        { "name", required_argument, NULL, 'n' },
        { "test", no_argument, &TestFlag, 1 },
        { 0 }
    };

    // Infinite loop, to be broken when we are done parsing options
    while (1) {
        opt = getopt_long(argc, argv, "hvcafmCSt:n:", Opts, 0);

        // A return value of -1 indicates that there are no more options
        if (opt == -1) {
            if(HelpFlag && VersionFlag) {
                Usage();;
                return EXIT_FAILURE;
            } else if(CreateFlag) {
                if(Additional == "main") {
                    Usage("Main will be created with filesystem. Ommitting -m");
                } else if(Additional != "") {
                    Usage("Additonal files can be added post-creation of fs. Omitting");
                }
            } else if(AdditionalFlag > 1) {
                Usage("Only one additonal (eg. -C, -S...) can be used at a time");
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
        case 'f':
            ForceFlag = 1;
            Force = 1;
            break;
        case 'M':
            AdditionalFlag++;
            Additional = "main";
            break;
        case 'C':
            AdditionalFlag++;
            Additional = "class";
            break;
        case 'S':
            AdditionalFlag++;
            Additional = "struct";
            break;
        case '?':
            Usage();
            return EXIT_FAILURE;
        default:
            Usage();
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
    if(ForceFlag) Force = 1;
    if(Type == "list") {
        /* std::cout << "Current suported types:" << std::endl; */
        for(std::string s: TypeList)
            std::cout << s << std::endl;
    }

    if(CreateFlag) {
        if(Type == "cpp") {
            try { BuildFS(Name, Type, Force); }
            catch(std::string Message) {
                Usage(Message);
                return EXIT_FAILURE;
            }
            catch(const char *Message) {
                Usage(Message);
                return EXIT_FAILURE;
            }
        }
    }
    if(AddFlag) {
        try { Add(Name, Type, Additional, Force); }
        catch(std::string Message) {
            Usage(Message);
            return EXIT_FAILURE;
        }
        catch(const char *Message) {
            Usage(Message);
            return EXIT_FAILURE;
        }
    }

    if(TestFlag) {
        GetContent("CMakeLists.txt", "ajj");
        GetContent("LICENSE");
        GetContent("install.sh");
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
    const std::string Path = BasePath + Name;
    k::MkDir(Path);
    chdir(Path.c_str());
}

void SetupDirinDir(std::string Name) {
    const std::string Path = Name;
    k::MkDir(Path);
    chdir(Path.c_str());
}

void BuildFS(std::string Name, std::string Type, bool Force) {
    SetupDir(Name);
    Add(Name, "cpp", "main", Force);
    Add("config", "cpp", "", Force);
    Add(Name, "cmake", "", Force);
    Add(Name, "readme", "", Force);
    Add("", "gdb", "", Force);
    Add("", "gitignore", "", Force);
    Add("", "license", "", Force);
    CreateRepo(Name);
}

void Add(std::string Name, std::string Type, std::string Additional, bool Force) {
    if(Type == "cpp") {
        if(Name == "config") { // config file
            CreateFile(GetContent("config.hpp", Name), "inc/" , Name + ".hpp", Force);
            Git("add inc/config.hpp");
        } else {
            if(Name == "") throw "Passing a name is required for this operation";
            if(Additional == "main") { // TODO add classes/stucts
                CreateFile(GetContent("main.cpp", Name), "src/", Name + ".cpp", Force);
                CreateFile(GetContent("main.hpp", Name), "inc/", Name + ".hpp", Force);
            } else if(Additional == "class") {
                CreateFile(GetContent("class.cpp", Name), "src/", Name + ".cpp", Force);
                CreateFile(GetContent("class.hpp", Name), "inc/", Name + ".hpp", Force);
            } else if(Additional == "struct") {
                std::string name = Name;
                name[0] = toupper(name[0]);
                CreateFile("#ifndef K" + Name + Name + "\n#define K" + Name + Name \
                                    + "\n\n#include \"config.hpp\"" \
                                    + "\n\ntypedef struct {\n} " + name + ";" \
                                    + "\n\n#endif" + LicenseFooterSlash, "inc/", \
                                    Name + ".hpp", Force);
            } else {
                CreateFile(GetContent("source.cpp", Name), "src/", Name + ".cpp", Force);
                CreateFile(GetContent("header.hpp", Name), "inc/", Name + ".hpp", Force);
            }
            Git("add src/" + Name + ".cpp");
            Git("add inc/" + Name + ".hpp");
        }
    } else if(Type == "cmake") {
        if(Name == "") throw "Passing a name is required for this operation";
        CreateFile(GetContent("CMakeLists.txt", Name), "CMakeLists.txt", Force);
        CreateFile(GetContent("install.sh"), "install.sh", Force);
        int result = k::ExecCmd("chmod +x install.sh");
        CreateFile(GetContent("cmake.yml"), ".github/workflows/", "cmake.yml", Force);
        Git("add CMakeLists.txt");
        Git("add .github");
        Git("add install.sh");
    } else if(Type == "license") {
        CreateFile(GetContent("LICENSE"), "LICENSE", Force);
        Git("add LICENSE");
    } else if(Type == "gdb") {
        CreateFile(GDB, ".gdbinit", Force);
        Git("add .gdbinit");
    } else if(Type == "gitignore") {
        CreateFile(GitIgnore, ".gitignore", Force);
        Git("add .gitignore");
    } else if(Type == "readme") {
        CreateFile(GetContent("README.md", Name), "README.md", Force);
        Git("add README.md");
    } else if(Type == "bash" || Type == "sh") {
        if(Name == "") throw "Passing a name is required for this operation";
        CreateFile("#!/bin/bash\n", Name + ".sh", Force);
        int result = k::ExecCmd("chmod +x " + Name + ".sh");
        Git("add" + Name + ".sh");
    } else if(Type == "python" || Type == "py") {
        if(Name == "") throw "Passing a name is required for this operation";
        CreateFile(Python, Name + ".py", Force);
        Git("add" + Name + ".py");
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

bool Exists(const char *Name) {
    /* std::ifstream f(Name); */
    /* return f.good(); */
    struct stat buffer;
    if(stat(Name, &buffer) == 0) return 1;
    return 0;
}

bool Exists(std::string Name) {
    struct stat buffer;
    if(stat(Name.c_str(), &buffer) == 0) return 1;
    return 0;
}

void CreateFile(std::string Content, std::string Name, bool Force) {
    if(!Force)
        if(Exists(Name)) throw "A '" + Name + "' file exists! If you are sure you want to do this use -f / --force.";
    k::WriteFileLines(Content, Name);
}

void CreateFile(std::string Content, std::string Path, std::string Name, bool Force) {
    if(!Force)
        if(Exists(Path + Name)) throw "A '" + Path + Name + "' file exists! If you are sure you want to do this use -f / --force.";
    std::string Seg;
    std::stringstream P(Path);
    std::vector<std::string> IncrementingPath;
    while(std::getline(P, Seg, '/')) {
        if(Seg == Name) break;
        IncrementingPath.push_back(Seg);
    }
    std::string path = "";
    for(std::string p: IncrementingPath) {
        if(path != "") path = path + "/" + p;
        else path = p;
        k::MkDir(path);
    }
    k::WriteFileLines(Content, Path + "/" + Name);
}

std::string GetContent(std::string FileStandard, std::string Name) {
    std::string Content;
    std::string ProjectName = fs::current_path().filename();

    std::string Line;
    std::ifstream FileToRead(FileStandardsPath + "/" + FileStandard);
    while(getline(FileToRead, Line)) {
        std::string L = Line;
        std::vector<std::string> Words;
        k::SplitString(L, '$', Words, 0);
        std::string LineOut;
        for(std::string &Word: Words) {
            if(Word == "Name") {
                Word = Name;
                Word[0] = toupper(Name[0]);
            } else if(Word == "NAME") {
                Word = Name;
                for(int i(0); i<Word.size(); i++)
                    Word[i] = toupper(Word[i]);
            } else if(Word == "name")
                Word = Name;
            else if(Word == "programname")
                Word = ProjectName;
            else if(Word == "PROGRAMNAME") {
                Word = ProjectName;
                for(int i(0); i<Word.size(); i++)
                    Word[i] = toupper(Word[i]);
            } else if(Word == "year")
                Word = Year;
            LineOut = LineOut + Word;
        }
        Content = Content + LineOut + "\n";
    }

    FileToRead.close();
    return Content;
}

std::string GetContent(std::string FileStandard) {
    std::string Content;

    std::string Line;
    std::ifstream FileToRead(FileStandardsPath + "/" + FileStandard);
    while(getline(FileToRead, Line)) {
        std::string L = Line;
        std::vector<std::string> Words;
        k::SplitString(L, '$', Words, 0);
        std::string LineOut;
        for(std::string &Word: Words) {
            if(Word == "year")
                Word = Year;
            LineOut = LineOut + Word;
        }
        Content = Content + LineOut + "\n";
    }

    FileToRead.close();
    return Content;
}

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. )";
