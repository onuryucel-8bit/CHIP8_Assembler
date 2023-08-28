#pragma once

#include <map>

#include "utils/Radix.h"

class Assembler_ch8 {

public:
	Assembler_ch8(std::string sourcePath, std::string outputPath);

private:
	std::string sourcePath;
	std::string outputPath;

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

Assembler_ch8::Assembler_ch8(std::string sourcePath, std::string outputPath) {

	this->sourcePath = sourcePath;
	this->outputPath = outputPath;

	//TODO check file paths are valid
	
}