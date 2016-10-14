#include <iostream>
#include <vector>
#include <sstream>
#include "definitions.h"
using namespace std;

void parseAndWrite(string instruction, ofstream& hackFile, char* vmFileName, int lineNumber)
{
	static string functionName(""); // default functionName

	stringstream s(instruction);
	string temp;
	vector <string> tokens;
 
	while(getline(s, temp, ' ')) // tokenize instruction
	{
		tokens.push_back(temp);
	}

	if(!tokens[0].compare("add"))
	{
		writeAdd(hackFile);
	}
	else if(!tokens[0].compare("sub"))
	{
		writeSub(hackFile);
	}
	else if(!tokens[0].compare("eq"))
	{
		writeEq(hackFile, vmFileName);
	}
	else if(!tokens[0].compare("lt"))
	{
		writeLt(hackFile, vmFileName);
	}
	else if(!tokens[0].compare("gt"))
	{
		writeGt(hackFile, vmFileName);
	}
	else if(!tokens[0].compare("and"))
	{
		writeAnd(hackFile);
	}
	else if(!tokens[0].compare("or"))
	{
		writeOr(hackFile);
	}
	else if(!tokens[0].compare("not"))
	{
		writeNot(hackFile);
	}
	else if(!tokens[0].compare("neg"))
	{
		writeNeg(hackFile);
	}
	else if(!tokens[0].compare("label"))
	{
		writeLabel(hackFile, functionName, tokens[1], vmFileName);
	}
	else if(!tokens[0].compare("goto"))
	{
		writeGoto(hackFile, functionName, tokens[1], vmFileName);
	}
	else if(!tokens[0].compare("if-goto"))
	{
		writeIfGoto(hackFile, functionName, tokens[1], vmFileName);
	}
	else if(!tokens[0].compare("push"))
	{
		writePush(hackFile, tokens[1], tokens[2], vmFileName, functionName);
	}
	else if(!tokens[0].compare("pop"))
	{
		writePop(hackFile, tokens[1], tokens[2], vmFileName, functionName);
	}
	else if(!tokens[0].compare("function"))
	{
		functionName = tokens[1];
		writeFunction(hackFile, tokens[1], tokens[2], vmFileName);
	}
	else if(!tokens[0].compare("call"))
	{
		functionName = tokens[1];
		writeCall(hackFile, tokens[1], tokens[2], vmFileName);
	}
	else if(!tokens[0].compare("return"))
	{
		writeReturn(hackFile, functionName, vmFileName);
	}
	else
	{
		cerr << "Wrong VM command at " << lineNumber << endl;
		exit(0);
	}
}