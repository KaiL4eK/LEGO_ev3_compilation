
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

	if (!OFILE.is_open() || strcmp(FName, fileName))
	{
		if (append)
			OFILE.open(fileName, std::ofstream::app);
		else
			OFILE.open(fileName);
	}
	if (OFILE.fail())
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
	if (IFILE.fail())
		return NULL;

	while (!IFILE.eof())
	{
		std::getline(IFILE, TBuffer);
		OBuffer = OBuffer + TBuffer;
	}

	IFILE.close();

	if (OBuffer.size())
		return &OBuffer.at(0);

	return NULL;

}

char* LoadStirngFromFile(char* fileName, int indexOfString)
{

	static std::ifstream FILE;
	static std::string OBuffer;
	static char* FName = NULL;
	//strcasecmp
	if (!FILE.is_open() || strcmp(FName, fileName))
	{
		FILE.open(fileName);
		FName = fileName;
	}
	for (int i = 0; !FILE.eof(); i++)
	{
		std::getline(FILE, OBuffer);
		if (i == indexOfString)
			break;
	}
	
	FILE.close();
	if (OBuffer.size())
		return &OBuffer.at(0);

	return 0;
}

void ClearFile(char* fileName)
{

	std::ofstream OFILE;
	OFILE.open(fileName);
	OFILE.close();
}

short CompareFiles(char* fileName0, char* fileName1)
{
	std::ifstream IFILE0(fileName0);
	std::ifstream IFILE1(fileName1);

	std::string Buffer0, Buffer1;

	if (!IFILE0.is_open() || !IFILE1.is_open())
		return 0;

	if (!strcmp(fileName0, fileName1))
		return 1;

	while (1)
	{

		std::getline(IFILE0, Buffer0); 
		std::getline(IFILE1, Buffer1);
		if (strcmp(Buffer0.c_str(), Buffer1.c_str()))
			return 0;

		if ((IFILE0.eof() && !IFILE1.eof()) || (!IFILE0.eof() && IFILE1.eof()))
			return 0;
		else if (IFILE0.eof() || IFILE1.eof())
			return 1;

	}

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

	char* File_load_string(char* fileName, int indexOfString)
	{

		return LoadStirngFromFile(fileName, indexOfString);

	}

	void File_Clear(char* fileName)
	{
		ClearFile(fileName);
	}

	short File_S_Compare(char* fileName0, char* fileName1)
	{
		return CompareFiles(fileName0, fileName1)
	}

}