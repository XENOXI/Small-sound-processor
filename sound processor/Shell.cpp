#include "Shell.h"


void Shell::use_cnf(std::ifstream& in,sstream* stream)
{
    Converter* cnv;
    while (!in.eof())
    {
        std::string buff,buff2;
        std::getline(in, buff);
        if (buff[0] == '#')
            continue;
        std::stringstream str;
        str << buff;
        str >> buff;
        std::getline(str, buff2);

        cnv = Converter::createConv(buff);
        cnv->set_sstream(stream);
        cnv->work(buff2);
    }

}

std::string Shell::getFilesNames(int i)
{
    if (i >= Shell::files.size())
        throw std::exception("No such file");
    return Shell::files[i];
}

void Shell::work(int argc, char* argv[])
{
    sstream res;
    if (argc==2 && (std::string)argv[1]=="-h")
    {
        Converter::All_help();
        exit(0);
    }
    if (argc > 4 && (std::string)argv[1] == "-c")
    {
        std::ifstream conf(argv[2]);
        if (!conf.is_open())
            throw std::exception("No config file");

        res.read(argv[4]);

        for (int i = 5; i < argc; i++)
            files.push_back(argv[i]);

        use_cnf(conf, &res);
        res.save(argv[3]);
        exit(0);
    }
    throw std::exception("Wrong command");
}
