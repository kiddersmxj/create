#ifndef Kcreate
#define Kcreate

#include "config.hpp"

#include <bits/getopt_core.h>
#include <unistd.h>
#include <iostream>
#include <getopt.h>
#include <std-k.hpp>
#include <sys/stat.h>
#include <fstream>
#include <sstream>

void Usage();
void Usage(std::string Message);
void PrintVersion();
void SetupDir(std::string Name);
void BuildFS(std::string Name, std::string Type, bool Force);
void Add(std::string Name, std::string Type, std::string Additional, bool Force);
bool Git(std::string Cmd);
bool CreateRepo(std::string Name);
bool Exists(const char *Name);
bool Exists(std::string Name);
void CreateFile(std::string Content, std::string Name, bool Force);
void CreateFile(std::string Content, std::string Path, std::string Name, bool Force);

#endif
