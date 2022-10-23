#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "music.h"


template <class Type>
std::istream& operator>>(std::istream& i, const Type& arg) { return i; }

template <class Type>
void argpars(Type& arg, std::string& str)
{
	std::stringstream buff;
	buff << str;
	buff >> arg;
	str.clear();
	std::getline(buff, str);
}

