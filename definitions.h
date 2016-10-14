#include <fstream>
#include <string>
using namespace std;

// definitions of all user-defined functions

// extract instructions from file and call parser
void processFile(ifstream&, ofstream&, char*);

// parse instruction and call appropriate function to write equivalent HACK code
void parseAndWrite(string, ofstream&, char*, int);

// write HACK code for respective VM commands

void writeAdd(ofstream& hackFile);

void writeSub(ofstream& hackFile);

void writeEq(ofstream& hackFile, char* vmFileName);

void writeLt(ofstream& hackFile, char* vmFileName);

void writeGt(ofstream& hackFile, char* vmFileName);

void writeAnd(ofstream& hackFile);

void writeOr(ofstream& hackFile);

void writeNot(ofstream& hackFile);

void writeNeg(ofstream& hackFile);

void writeLabel(ofstream& hackFile, string functionName, string labelName, char* vmFileName);

void writeGoto(ofstream& hackFile, string functionName, string labelName, char* vmFileName);

void writeIfGoto(ofstream& hackFile, string functionName, string labelName, char* vmFileName);

void writePush(ofstream& hackFile, string segment, string index,char* vmFileName, string functionName);

void writePop(ofstream& hackFile, string segment, string index, char* vmFileName, string functionName);

void writeFunction(ofstream& hackFile, string functionName, string k, char* vmFileName);

void writeCall(ofstream& hackFile, string functionName, string n, char* vmFileName);

void writeReturn(ofstream& hackFile, string functionName, char* vmFileName);