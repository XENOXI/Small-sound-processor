#pragma once
#include "Converter.h"
#include <regex>
class Shell
{
private:
	static std::vector<std::string> files;
	void use_cnf(std::ifstream&,sstream*);
public:
	static std::string getFilesNames(int);
	void work(int argc, char* argv[]);
};

