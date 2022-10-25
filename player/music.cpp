#include "music.h"
#include "Shell.h"

bool sstream::str_4_eq(const char* f, const char* s)
{
    for (int i = 0; i < 4; i++)
        if (f[i] != s[i])
            return false;
    return true;
}

void sstream::read(std::string filename)
{
    std::ifstream in(filename, std::ios::binary | std::ios::in);
    if (in.fail())
        throw std::exception("File not found");


    in.read((char*)&header, sizeof(WAVHEADER));

    if (!str_4_eq(header.chunkId, "RIFF"))
        throw std::exception("File is not supported");

    if (!str_4_eq(header.format, "WAVE"))
        throw std::exception("File is not supported");

    if (!str_4_eq(header.subchunk1Id, "fmt "))
        throw std::exception("File is not supported");

    if (header.subchunk1Size != 16)
        throw std::exception("File is not supported");

    if (header.audioFormat != 1)
        throw std::exception("File is not supported");

    if (header.numChannels != 1)
        throw std::exception("File is not supported");

    if (header.sampleRate != 44100)
        throw std::exception("File is not supported");

    if (header.byteRate != 88200)
        throw std::exception("File is not supported");

    if (header.blockAlign != 2)
        throw std::exception("File is not supported");

    if (header.bitsPerSample != 16)
        throw std::exception("File is not supported");

    if (str_4_eq(header.subchunk2Id, "LIST"))
    {
        int skip;
        in.read((char*)&skip, 4);
        for (int i = 0; i < skip; i++)
            in.read((char*)&skip, 1);
        in.read(header.subchunk2Id, 4);
    }
    if (!str_4_eq(header.subchunk2Id, "data"))
        throw std::exception("File is not supported");

    length = 1.f * header.subchunk2Size / (header.bitsPerSample / 8) / header.numChannels / header.sampleRate;

    signals.clear();
    int data_cnt = header.subchunk2Size/ (header.bitsPerSample / 8) / header.numChannels;
    for (int i = 0; i <data_cnt; i++)
    {
        signals.push_back(0);
        in.read((char*)&(signals.back()), 2);
    }
    in.close();
}

void sstream::save(std::string filename)
{
    std::ofstream out(filename, std::ios::binary | std::ios::out);
    out.write((char*)&header, sizeof(WAVHEADER));
    for (short i : signals)
        out.write((char*)&i, 2);
    out.close();
}

std::istream& operator>>(std::istream& i, sstream& arg)
{
    std::string buff;
    i >> buff;
    if (buff.size() < 2 || buff[0] != '$')
        throw std::exception("Not pointer");

    int num = 0;
    for (int i = 1; i < buff.size(); i++)
    {
        if (buff[i] < '0' || buff[i]>'9')
            throw std::exception("Wrong pointer");
        num += buff[i] - '0';
    }
    arg.read(Shell::getFilesNames(num));
    return i;
}