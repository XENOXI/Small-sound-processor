#include <iostream>
#include "Converter.h"
#include "Shell.h"
#include <sstream>

std::vector<std::string> Shell::files;

int main(int argc,char* argv[])
{
    Shell shell;
    shell.work(argc, argv);
}

