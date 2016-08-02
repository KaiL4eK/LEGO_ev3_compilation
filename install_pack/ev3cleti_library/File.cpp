
#include <string>
#include <fstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

bool SaveFile(char* fileName, char* buffer, short append)
{

	std::ofstream OFILE;

	if(append)
		OFILE.open(fileName, std::ofstream::app);
	else 
		OFILE.open(fileName);

	if(OFILE.fail())
		return false;
	OFILE << buffer <<std::endl;
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

}