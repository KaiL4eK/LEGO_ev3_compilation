
#include <string>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool SaveFile(char* fileName, char* buffer, short append)
{

    static std::ofstream OFILE;
    static char* FName = NULL;

    if(!OFILE.is_open() || strcasecmp(FName, fileName))
    {
        if(append)
            OFILE.open(fileName, std::ofstream::app);
        else
            OFILE.open(fileName);
    }
    if(OFILE.fail())
        return false;
    OFILE << buffer << std::endl;
    OFILE.close();
    return true;

}

char* LoadFile(char* fileName)
{

	std::string TBuffer;
	static std::string OBuffer;

	OBuffer.clear();

	std::ifstream IFILE(fileName);
	if(IFILE.fail())
		return NULL;

	while(!IFILE.eof())
	{
		std::getline(IFILE, TBuffer);
		OBuffer = OBuffer + TBuffer;
	}

	IFILE.close();

	if(OBuffer.size())
		return &OBuffer.at(0);

	return NULL;

}

char* LoadStirngFromFile(char* fileName)
{

    static std::ifstream FILE;
    static std::string OBuffer;
    static char* FName = NULL;

    if(!FILE.is_open() || strcasecmp(FName, fileName))
    {
        FILE.open(fileName);
        FName = fileName;
    }

    std::getline(FILE, OBuffer);

    if(OBuffer.size())
        return &OBuffer.at(0);

    return 0;
}

extern "C"
{

	short File_save(char* fileName, char* buffer, short append)
	{

		return SaveFile(fileName, buffer, append);

	}

	char* File_load(char* fileName)
	{

		return LoadFile(fileName);

	}

	char* File_load_string(char* fileName)
	{

		return LoadStirngFromFile(fileName);

	}

}