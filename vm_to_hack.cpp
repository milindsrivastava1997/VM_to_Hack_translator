#include <iostream>
#include <string.h>
#include "definitions.h"
using namespace std;

int main(int argc, char** argv)
{
	if(argc == 1)
	{
		cout << "No .vm files specified in arguments." << endl;
		return 0;
	}

	int numberOfInputFiles = argc - 2;

	ifstream vmFile;
	ofstream hackFile;

	hackFile.open(argv[argc-1], ios::trunc | ios::out);

	hackFile << "@256\nD=A\n@SP\nM=D\n"; // BOOTSTRAP: set stack pointer to 256

	for(int i = 1; i <= numberOfInputFiles; i++)
	{
		vmFile.open(argv[i], ios::in);

		string temp(argv[i]);
		temp.erase(temp.end() - 3, temp.end()); // remove .vm extension to get file name

		if(i == 1)
		{
			writeCall(hackFile, string("Sys.init"), string("0"), strdup(temp.c_str())); // BOOTSRAP: "call Sys.init"
		}

		processFile(vmFile, hackFile, strdup(temp.c_str()));
		vmFile.close();
	}

	hackFile << "(END)\n@END\n0;JMP\n";

	hackFile.close();

	return 0;
}

void processFile(ifstream& vmFile, ofstream& hackFile, char* vmFilename)
{
	string instruction, foo;

	int count = 1;

	while(true)
	{
		getline(vmFile, instruction, '\r');
		getline(vmFile, foo, '\n');

		if(vmFile.eof())
		{
			break;
		}

		int commentPosition = instruction.find("//"); // removing comments
		if(commentPosition != -1)
		{
			instruction.erase(instruction.begin() + commentPosition, instruction.end());
		}

		if(instruction.size() == 0)
		{
			count++;
			continue;
		}

		instruction.erase(instruction.begin(), instruction.begin() + instruction.find_first_not_of(" \t")); // remove leading whitespaces

		if(instruction.size() == 0)
		{
			count++;
			continue;
		};

		parseAndWrite(instruction, hackFile, vmFilename, count);

		count++;
	}
}