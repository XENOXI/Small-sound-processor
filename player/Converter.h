#pragma once
#include <iostream>
#include "music.h"
#include <filesystem>
#include "Argparser.h"


class Converter
{
protected:
	sstream* buff;
public:	
	Converter(sstream*);
	Converter();
	void set_sstream(sstream* in);
	static void All_help();
	virtual void help()=0;
	virtual void work(std::string)=0;
	void printConfigFile(std::filesystem::path filename);
	static Converter* createConv(std::string);
	//virtual void parser(std::string){};
};



class Mix : public Converter
{
public:
	void work(std::string);
	void help();
	
};


class Mute : public Converter
{
public:
	void work(std::string argv);
	void help();
};

class Reverb :public Converter
{
public:
	void work(std::string argv);
	void help();
};