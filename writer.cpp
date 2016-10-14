#include <fstream>
#include <string>
#include <cstdlib>
#include <string.h>
using namespace std;

// common macros

#define commonUnaryOp "@SP\nA=M-1\nD=M\n"
#define commonBinaryOp "@SP\nAM=M-1\nD=M\nA=A-1\n"
#define commonPush "A=D+M\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n"
#define commonPop "@SP\nAM=M-1\nD=M\n"

void writeAdd(ofstream& hackFile)
{
	hackFile.seekp(0, ios::end);
	hackFile << commonBinaryOp;
	hackFile << "M=M+D\n";
}

void writeSub(ofstream& hackFile)
{
	hackFile.seekp(0, ios::end);
	hackFile << commonBinaryOp;
	hackFile << "M=M-D\n";
}

void writeEq(ofstream& hackFile, char* vmFileName)
{
	static int count = 1;

	hackFile.seekp(0, ios::end);
	hackFile << commonBinaryOp;
	hackFile << "D=M-D\n";
	hackFile << "@" << vmFileName << ".eq_label_" << count << "\n";
	hackFile << "D;JNE\n";
	hackFile << "@SP\n";
	hackFile << "A=M-1\n";
	hackFile << "M=-1\n";
	hackFile << "@" << vmFileName << ".eq_label_continue_" << count << "\n";
	hackFile << "0;JMP\n";
	hackFile << "(" << vmFileName << ".eq_label_" << count << ")\n";
	hackFile << "@SP\n";
	hackFile << "A=M-1\n";
	hackFile << "M=0\n";
	hackFile << "(" << vmFileName << ".eq_label_continue_" << count << ")\n";

	count++;
}

void writeLt(ofstream& hackFile, char* vmFileName)
{
	static int count = 1;

	hackFile.seekp(0, ios::end);
	hackFile << commonBinaryOp;
	hackFile << "D=M-D\n";
	hackFile << "@" << vmFileName << ".lt_label_" << count << "\n";
	hackFile << "D;JGE\n";
	hackFile << "@SP\n";
	hackFile << "A=M-1\n";
	hackFile << "M=-1\n";
	hackFile << "@" << vmFileName << ".lt_label_continue_" << count << "\n";
	hackFile << "0;JMP\n";
	hackFile << "(" << vmFileName << ".lt_label_" << count << ")\n";
	hackFile << "@SP\n";
	hackFile << "A=M-1\n";
	hackFile << "M=0\n";
	hackFile << "(" << vmFileName << ".lt_label_continue_" << count << ")\n";

	count++;
}

void writeGt(ofstream& hackFile, char* vmFileName)
{
	static int count = 1;

	hackFile.seekp(0, ios::end);
	hackFile << commonBinaryOp;
	hackFile << "D=M-D\n";
	hackFile << "@" << vmFileName << ".gt_label_" << count << "\n";
	hackFile << "D;JLE\n";
	hackFile << "@SP\n";
	hackFile << "A=M-1\n";
	hackFile << "M=-1\n";
	hackFile << "@" << vmFileName << ".gt_label_continue_" << count << "\n";
	hackFile << "0;JMP\n";
	hackFile << "(" << vmFileName << ".gt_label_" << count << ")\n";
	hackFile << "@SP\n";
	hackFile << "A=M-1\n";
	hackFile << "M=0\n";
	hackFile << "(" << vmFileName << ".gt_label_continue_" << count << ")\n";

	count++;
}

void writeAnd(ofstream& hackFile)
{
	hackFile.seekp(0, ios::end);
	hackFile << commonBinaryOp;
	hackFile << "M=M&D\n";
}

void writeOr(ofstream& hackFile)
{
	hackFile.seekp(0, ios::end);
	hackFile << commonBinaryOp;
	hackFile << "M=M|D\n";	
}

void writeNot(ofstream& hackFile)
{
	hackFile.seekp(0, ios::end);
	hackFile << commonUnaryOp;
	hackFile << "M=!D\n";
}

void writeNeg(ofstream& hackFile)
{
	hackFile.seekp(0, ios::end);
	hackFile << commonUnaryOp;
	hackFile << "M=-D\n";
}

void writeLabel(ofstream& hackFile, string functionName, string labelName, char* vmFileName)
{
	hackFile.seekp(0, ios::end);
	hackFile << "(";
	if(functionName.size() != 0)
	{
		hackFile << functionName;
	}
	if(labelName.size() != 0)
	{
		hackFile << "." << labelName;
	}
	hackFile << ")\n";
}

void writeGoto(ofstream& hackFile, string functionName, string labelName, char* vmFileName)
{
	hackFile.seekp(0, ios::end);
	hackFile << "@";
	if(functionName.size() != 0)
	{
		hackFile << functionName;
	}
	if(labelName.size() != 0)
	{
		hackFile << "." << labelName;
	}
	hackFile << "\n";
	hackFile << "0;JMP\n";
}

void writeIfGoto(ofstream& hackFile, string functionName, string labelName, char* vmFileName)
{
	hackFile.seekp(0, ios::end);
	hackFile << "@SP\nAM=M-1\nD=M\n";
	hackFile << "@";
	if(functionName.size() != 0)
	{
		hackFile << functionName;
	}
	if(labelName.size() != 0)
	{
		hackFile << "." << labelName;
	}
	hackFile << "\n";
	hackFile << "D;JNE\n";
}

void writePush(ofstream& hackFile, string segment, string index, char* vmFileName, string functionName)
{
	hackFile.seekp(0, ios::end);

	static int staticCount = 1;

	if(segment.compare("static"))
	{
		hackFile << "@" << index << "\n";
		hackFile << "D=A\n";
	}

	if(!segment.compare("local"))
	{
		hackFile << "@LCL\n";
		hackFile << commonPush;
	}
	else if(!segment.compare("argument"))
	{
		hackFile << "@ARG\n";
		hackFile << commonPush;
	}
	else if(!segment.compare("static"))
	{
		hackFile << "@" << vmFileName << ".staticStart." << index << "\n";
		hackFile << "D=M\n";
		hackFile << "@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	}
	else if(!segment.compare("constant"))
	{
		hackFile << "@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	}
	else if(!segment.compare("temp"))
	{
		hackFile << "@5\n";
		hackFile << "A=A+D\n";
		hackFile << "D=M\n";
		hackFile << "@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	}
	else if(!segment.compare("pointer"))
	{
		hackFile << "@3\n";
		hackFile << "A=A+D\n";
		hackFile << "D=M\n";
		hackFile << "@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	}
	else if(!segment.compare("this"))
	{
		hackFile << "@3\n";
		hackFile << commonPush;
	}
	else if(!segment.compare("that"))
	{
		hackFile << "@4\n";
		hackFile << commonPush;	
	}

	staticCount++;
}

void writePop(ofstream& hackFile, string segment, string index, char* vmFileName, string functionName)
{
	static int count = 1;

	static int staticCount = 1;

	hackFile.seekp(0, ios::end);

	if(!segment.compare("local"))
	{
		hackFile << "@LCL\n";
		hackFile << "D=M\n";
		hackFile << "@" << index << "\n";
		hackFile << "D=D+A\n";
		hackFile << "@13\n";
		hackFile << "M=D\n";
		hackFile << commonPop;
		hackFile << "@13\n";
		hackFile << "A=M\n";
		hackFile << "M=D\n";
	}
	else if(!segment.compare("argument"))
	{
		hackFile << "@ARG\n";
		hackFile << "D=M\n";
		hackFile << "@" << index << "\n";
		hackFile << "D=D+A\n";
		hackFile << "@13\n";
		hackFile << "M=D\n";
		hackFile << commonPop;
		hackFile << "@13\n";
		hackFile << "A=M\n";
		hackFile << "M=D\n";
	}
	else if(!segment.compare("static"))
	{
		hackFile << "@" << vmFileName << ".staticStart." << index << "\n";
		hackFile << "D=A\n";
		hackFile << "@13\n";
		hackFile << "M=D\n";
		hackFile << commonPop;
		hackFile << "@13\n";
		hackFile << "A=M\n";
		hackFile << "M=D\n";
	}
	else if(!segment.compare("temp"))
	{
		hackFile << "@5\n";
		hackFile << "D=A\n";
		hackFile << "@" << index << "\n";
		hackFile << "D=D+A\n";
		hackFile << "@13\n";
		hackFile << "M=D\n";
		hackFile << commonPop;
		hackFile << "@13\n";
		hackFile << "A=M\n";
		hackFile << "M=D\n";
	}
	else if(!segment.compare("pointer"))
	{
		hackFile << "@3\n";
		hackFile << "D=A\n";
		hackFile << "@" << index << "\n";
		hackFile << "D=D+A\n";
		hackFile << "@13\n";
		hackFile << "M=D\n";
		hackFile << commonPop;
		hackFile << "@13\n";
		hackFile << "A=M\n";
		hackFile << "M=D\n";
	}
	else if(!segment.compare("this"))
	{
		hackFile << "@3\n";
		hackFile << "D=M\n";
		hackFile << "@" << index << "\n";
		hackFile << "D=D+A\n";
		hackFile << "@13\n";
		hackFile << "M=D\n";
		hackFile << commonPop;
		hackFile << "@13\n";
		hackFile << "A=M\n";
		hackFile << "M=D\n";
	}
	else if(!segment.compare("that"))
	{
		hackFile << "@4\n";
		hackFile << "D=M\n";
		hackFile << "@" << index << "\n";
		hackFile << "D=D+A\n";
		hackFile << "@13\n";
		hackFile << "M=D\n";
		hackFile << commonPop;
		hackFile << "@13\n";
		hackFile << "A=M\n";
		hackFile << "M=D\n";
	}

	count++;
	staticCount++;
}

void writeFunction(ofstream& hackFile, string functionName, string k, char* vmFileName)
{
	hackFile.seekp(0, ios::end);

	int localVars = atoi(k.c_str());

	static int count = 1;

	writeLabel(hackFile, functionName, "", strdup(string("").c_str()));

	while(localVars > 0)
	{
		hackFile << "@SP\nM=M+1\nA=M-1\nM=0\n";

		localVars--;
	}

	count++;
}

void writeCall(ofstream& hackFile, string functionName, string n, char* vmFileName)
{
	hackFile.seekp(0, ios::end);

	int args = atoi(n.c_str());

	static int count = 1;

	writePush(hackFile, string("constant"), functionName + string(".") + string("return-address") + string(".") + to_string(count), vmFileName, functionName);
	hackFile << "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	hackFile << "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	hackFile << "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	hackFile << "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";

	hackFile << "@SP\nA=M\nM=A\n@SP\nM=M+1\n";
	writePush(hackFile, string("constant"), to_string(args+5), vmFileName, functionName);
	writeSub(hackFile);
	hackFile << "@SP\nAM=M-1\nD=M\n@ARG\nM=D\n";

	hackFile << "@SP\nD=M\n@LCL\nM=D\n";

	writeGoto(hackFile, functionName, "", vmFileName);
	writeLabel(hackFile, functionName, string("return-address.") + to_string(count), strdup(string("").c_str()));

	count++;
}

void writeReturn(ofstream& hackFile, string functionName, char* vmFileName)
{
	hackFile.seekp(0, ios::end);

	static int count = 1;

	hackFile << "@LCL\nA=M-1\nA=A-1\nA=A-1\nA=A-1\nA=A-1\nD=M\n";
	hackFile << "@" << vmFileName << "." << functionName << ".RET" << "\nM=D\n";

	hackFile << "@SP\nA=M-1\nD=M\n@ARG\nA=M\nM=D\n";
	hackFile << "@ARG\nD=M+1\n@SP\nM=D\n";
	
	hackFile << "@LCL\nA=M-1\nD=M\n@THAT\nM=D\n";
	hackFile << "@LCL\nA=M-1\nA=A-1\nD=M\n@THIS\nM=D\n";
	hackFile << "@LCL\nA=M-1\nA=A-1\nA=A-1\nD=M\n@ARG\nM=D\n";
	hackFile << "@LCL\nA=M-1\nA=A-1\nA=A-1\nA=A-1\nD=M\n@LCL\nM=D\n";
	hackFile << "@" << vmFileName << "." << functionName << ".RET" << "\n";
	hackFile << "A=M\n0;JMP\n"; 

	count++;
}