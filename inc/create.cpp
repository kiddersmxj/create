#ifndef Kcreate
#define Kcreate

#include "config.hpp"

#include <iostream>
#include <getopt.h>
#include <std-k.hpp>
#include <sys/stat.h>

void Usage();
void Usage(std::string Message);
void PrintVersion();
void SetupDir(std::string Name);
void BuildFS(std::string Name, std::string Type);

#endif
