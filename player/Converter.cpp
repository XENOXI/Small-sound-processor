#include "Converter.h"
#include <regex>



void Mute::work(std::string argv)
{
	int st, en;
	argpars<int>(st, argv);
	argpars<int>(en, argv);

	en = std::min((int)buff->signals.size(), (int)(en * buff->header.sampleRate));
	st *= buff->header.sampleRate;
	for (int i = st; i < en; i++)
		buff->signals[i] = 0;
}

void Mute::help()
{
	printConfigFile("./config/mute.txt");
}

void Reverb::work(std::string argv)
{
	int st, en;
	argpars<int>(st, argv);
	argpars<int>(en, argv);

	int delayMilliseconds; 
	argpars<int>(delayMilliseconds, argv);

	int delaySamples = (int)((float)delayMilliseconds * buff->header.sampleRate/1000);
	float decay = 0.1f;
	en = std::min((int)buff->signals.size(), (int)(en * buff->header.sampleRate));
	int lst = en * buff->header.sampleRate;
	int str = st * buff->header.sampleRate;
	for (int i = str; i < lst - delaySamples; i++)
		buff->signals[i + delaySamples] += (short)std::max(-32768.f,std::min(((float)buff->signals[i] * decay),32767.f));
}

void Reverb::help()
{
	printConfigFile("./config/reverb.txt");
}


Converter::Converter(sstream* in)
{
	if (in == NULL)
		throw std::exception("No stream");
	buff = in;
}

Converter::Converter()
{
	buff = NULL;
}

void Converter::set_sstream(sstream* in)
{
	if (in == NULL)
		throw std::exception("No stream");
	buff = in;
}

void Converter::All_help()
{
	std::string path = "./config";
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.path().extension() == ".txt")
		{
			std::cout << "----------------------------------------\n";
			std::ifstream in(entry.path());
			std::string str;
			try {
				std::getline(in, str);
				std::cout << str << '\n';
				std::getline(in, str);
				std::cout << str << '\n';
				std::getline(in, str);
				std::cout << str << '\n';
			}
			catch (std::exception e)
			{
				throw("Wrong config files");
			}
			std::cout << "----------------------------------------\n";
		}

		//if (std::regex_match())

	}
}

void Converter::printConfigFile(std::filesystem::path filename)
{
	std::ifstream in(filename);
	std::string str;
	try {
		std::getline(in, str);
		std::cout << str << '\n';
		std::getline(in, str);
		std::cout << str << '\n';
		std::getline(in, str);
		std::cout << str << '\n';
	}
	catch (std::exception e)
	{
		throw("Wrong config files");
	}
}

void Mix::work(std::string argv)
{
	sstream second;
	int sec;
	argpars<sstream>(second, argv);
	argpars<int>(sec, argv);


	sec *= buff->header.sampleRate;
	int end = std::min(sec, (int)buff->signals.size());
	for (int i = 0; i < end; i++)
		if (i >= second.signals.size())
			break;
		else
			buff->signals[i] = (buff->signals[i] + second.signals[i]) / 2;
}

void Mix::help()
{
	printConfigFile("./config/mix.txt");
}

Converter* Converter::createConv(std::string str)
{
	if (str == "mix")
		return new Mix();
	if (str == "mute")
		return new Mute();
	if (str == "reverb")
		return new Reverb();
	throw std::exception("Converter not found");
}