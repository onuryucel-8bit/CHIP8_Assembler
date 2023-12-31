#pragma once

#include<iostream>
#include<fstream>

#include<string>
#include<sstream>

#include <map>
#include <vector>

class Assembler_ch8 {

public:
	Assembler_ch8(std::string sourcePath, std::string outputPath);
	int run();

	bool isStrEmpty(std::string str);

private:
	std::string sourcePath;
	std::string outputPath;

	size_t ramUsed = 0;
	size_t lineNumber = 0;

	std::ofstream outputFile;
	std::ifstream sourceFile;

	char lineSlider = 0;

	int writeCharset_toROM();
	int writeVariable_toROM();
	int writeCode_toROM();
	
	bool isInRange(std::string line);
	void printError(std::string message);
		
	std::pair<std::string, std::string> getParts(std::string line);
	std::string combine(std::string hexOpcode, std::string hexPart);

	void writeMachineCode(std::ofstream* outputFile, std::pair<std::string, std::string> parts);

	std::map<std::string, std::string> opCodes = {

		{"ROMB", "0x01"},
		{"ROME", "0x02"},
		{"VAR_SECTION_BEGIN"  , "0x03"},
		{"VAR_SECTION_END"    , "0X04"},

		{"CODE_SECTION_BEGIN" , "0x05"},
		{"CODE_SECTION_END"   , "0X06"},

		{"LET","0X07"},
		{"LABEL","0X08"},

		{"CLS", "0x00E0"},{"RET", "0x00EE"},{"JMP", "0x1NNN"},{"CALL","0x2NNN"},
		{"CE",  "0x3XNN"},{"CNE", "0x4XNN"},{"RE",  "0x5XY0"},{"LDC", "0x6XNN"},

		{"ADDC","0x7XNN"},{"MOV", "0x8XY0"},{"OR",  "0x8XY1"},{"AND", "0x8XY2"},

		{"XOR", "0x8XY3"},{"ADDR","0x8XY4"},{"SUBX","0x8XY5"},{"SHR", "0x8XY6"},
		{"SUBY","0x8XY7"},{"SHL", "0x8XYE"},{"RNE", "0x9XY0"},{"LDI", "0xANNN"},

		{"JMPZ","0xBNNN"},{"RND", "0xCXNN"},{"DRW", "0xDXYN"},{"SKP", "0x9X9E"},
		{"SKNP","0x9X9E"},{"LDD", "0xFx07"},{"WFK", "0xFx0A"},{"LDRD","0xFx15"},

		{"LDRS","0xFx18"},{"ADDI","0xFx1E"},{"IIR", "0xFx29"},{"DTB", "0xFx33"},
		{"STR", "0xFx55"},{"CFR", "0xFx65"},
	};
	
};