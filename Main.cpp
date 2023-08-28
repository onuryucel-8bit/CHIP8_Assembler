#include <iostream>
#include <string>

#include <cstdint>

#include <sstream>
#include <fstream>

#include <map>
#include <vector>

#include "Assembler_ch8.h"

#define DEBUG_OPCODE_PARTS 0
#define DEBUG_MACHINE_CODE 0

/*
		ROMB;
			0x0f.....
			......
			...
		ROME;

		VAR_SECTION_BEGIN
			LET 0X0F
			LET 0xf
			....
			....

		VAR_SECTION_END

		CODE_SECTION_BEGIN

			label sinFunc:
				CLS
				LDR
				SHR
			END

			CLS
			LDI
			JMP  jmp_Label
			CALL sinFunc

			label jmp_Label

		CODE_SECTION_END
	*/

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

char lineSlider = 0;

int ramUsed = 0;

int startIndex = 0;
int endIndex = 0;
int lineNumber = 0;

uint8_t toDec(std::string hex) {

	//0xFE => FE

	unsigned int dec = std::stoul(hex, nullptr, 16);

	return (uint8_t)(dec);
}

//translate decimal to hex
std::string toHex(uint16_t dec) {

	std::stringstream ss;
	ss << std::hex << std::uppercase << dec;
	return ss.str();
}

//translate decimal to hex
std::string toHex(std::string dec) {

	std::stringstream ss;
	ss << std::hex << std::uppercase << dec;
	return ss.str();
}

bool isStrEmpty(std::string str) {
	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] != ' ') {
			return false;
		}
	}

	return true;
}

void printError(std::string message) {
	std::cout << "ERROR : " << message << "\n";
}

std::string combine(std::string hexOpcode, std::string hexPart) {

	//JUMP 0x1nnn
	//hexOpcode 1 hexPart nnn 

	char firstHex = hexOpcode.substr(2, 1)[0];

	switch (firstHex)
	{

	case '1':
	case '2':
	case '3':
	case '4':
	case '6':
	case '7':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
		hexOpcode.replace(3, 3, hexPart);
		break;

	case '5':
	case '8':
		hexOpcode.replace(3, 2, hexPart);
		break;

	case '9':
	case 'F':
		hexOpcode.replace(3, 1, hexPart);
		break;
	}

	return hexOpcode;
}

void writeSourceFile(std::vector<std::string>* inputs) {

	std::fstream file;
	file.open("Source.och8", std::ios::out);

	if (file.is_open()) {

		char lineNumber = 0;

		for (size_t i = 0; i < inputs->size(); i++) {
			file << (*inputs)[i] << ", ";

			lineNumber++;
			if (lineNumber % 5 == 0) {
				file << "\n";
				lineNumber = 0;
			}
		}

		file.close();
	}
	else
	{
		std::cout << "ERROR :: couldnt opened file \n";
	}
}
/*
	och8 reader put these func to Chip8.cpp
*/

int writeCharset_toROM(std::ifstream* file) {

	//open .och8 file
	std::ofstream outputFile("Source.och8", std::ios::out);

	//if file cant open
	if (!outputFile.is_open()) {
		std::cout << "ERROR : cannot open Source.och8 \n";
		return -1;
	}
	else {

		int lineNumber = 0;
		std::string line;
		size_t blankLineIndex;

		//get label of beginning char set
		std::getline(*file, line);
		while (line.empty()) {
			std::getline(*file, line);
		}

		if (line != "ROMB") {
			std::cout << "ERROR : getCharsetData() : Theres NOT begining of Charset keyword \n";
			return -1;
		}

		outputFile << "ROMB ";
		char lineSlider = 0;

		while (std::getline(*file, line)) {

			//if line at end of char set break the loop
			if (line == "ROME") {
				break;
			}
			else {

				//get first char index of line str
				blankLineIndex = line.find_first_not_of(' ') + 1;

				while (blankLineIndex < line.length()) {

					outputFile << line.substr(blankLineIndex, 4) << ",";
					ramUsed++;

					lineNumber++;
					blankLineIndex += 5;

					lineSlider++;

					if (lineSlider == 5) {
						lineSlider = 0;
						outputFile << "\n";
					}
				}
			}
		}

		if (line != "ROME") {
			std::cout << "ERROR: End of char set undefined ! \n";
			return -1;
		}

		outputFile << "ROME\n";
		outputFile.close();

		return lineNumber;
	}
}

bool isInRange(std::string line) {

	//line = "24,67,250,255"
	
	std::vector<std::string> numbers;

	// Split the string by comma
	std::istringstream ss(line);
	std::string token;
	while (std::getline(ss, token, ',')) {
		numbers.push_back(token);
	}

	// Check if each number is in the range 0 to 255
	for (const std::string& numStr : numbers) {

		int num = std::stoi(numStr);
		if (num < 0 || num > 255) {
			return false;
		}

	}
		
	//check for range
	return true;
}

int writeVariable_toROM(std::ifstream* file) {
	//open .och8 file
	std::ofstream outputFile("Source.och8", std::ios::app);

	//if file cant open
	if (!outputFile.is_open()) {
		std::cout << "ERROR : cannot open Source.och8 \n";
		return -1;
	}
	else {

		std::string line;
		size_t blankLineIndex;

		//get label of beginning char set
		std::getline(*file, line);
		while (line.empty()) {
			std::getline(*file, line);
		}

		if (line != "VARB") {
			std::cout << "ERROR : getCharsetData() : Theres NOT VARB keyword \n";
			return -1;
		}

		outputFile << "VARB ";
		char lineSlider = 0;

		while (std::getline(*file, line)) {

			//if line at end of char set break the loop
			if (line == "VARE") {
				break;
			}
			else{
				blankLineIndex = line.find_first_not_of(' ') + 1;

				//check number boundries 0<= x <= 0xff
				if (!isInRange(line.substr(blankLineIndex, line.length() - blankLineIndex))) {
					printError("variable size is big or small LINE = " + line.substr(blankLineIndex, line.length() - blankLineIndex));
					return -1;
				}


				outputFile << line.substr(blankLineIndex,line.length() - blankLineIndex) << ",";
				ramUsed++;

				lineSlider++;

				if (lineSlider == 5) {
					lineSlider = 0;
					outputFile << "\n";
				}
				
			}
		}

		if (line != "VARE") {
			std::cout << "ERROR: End of VAR SECTION undefined ! \n";
			return -1;
		}

		outputFile << "VARE\n";
		outputFile.close();

		return 0;
	}
}

std::pair<std::string,std::string> getParts(std::string line) {
	
	std::pair<std::string, std::string> parts;
	parts.first = "0";
	parts.second = "0";

	size_t i = 0;
	while (line[i] == ' ') {
		i++;
	}

	size_t j = i;
	while (line[j] != ' ' && j < line.length()) {

		//check syntax error
		if (line[j] == '#' || line[i] == '$') {
			printError("syntax error there is no space between opcode and numbers");
			return parts;
		}
		j++;
	}

	if (opCodes.find(line.substr(i, j - i)) != opCodes.end()) {

		//jmp 20
		parts.first = line.substr(i, j - i);//jmp
		if (parts.first == "CLS" || parts.first == "RET") {
			parts.second = "empty";
		}
		else{
			parts.second = line.substr(j + 1, line.length() - j);//20
		}
		
		return parts;
	}
	
	return parts;
	
}

void writeMachineCode(std::ofstream* outputFile,std::pair<std::string, std::string> parts) {
	
	if (parts.first == "CLS" || parts.first == "RET") {
		*outputFile << opCodes[parts.first].substr(0,4) << ",0x" << opCodes[parts.first].substr(4, 2)<<",";
		
	}
	else{
		std::string machineCode = combine(opCodes[parts.first], parts.second.substr(1,3));
		
		#ifndef DEBUG_MACHINE_CODE
			std::cout << "Machine code = " << machineCode << "\n";
		#endif // DEBUG_MACHINE_CODE

		*outputFile << machineCode.substr(0, 4) << ",0x" << machineCode.substr(4, 2)<<",";
			
	}

	lineSlider++;
	
	if (lineSlider == 5) {
		lineSlider = 0;
		*outputFile << "\n";
	}

	ramUsed += 2;
}

int writeCode_toROM(std::ifstream* file) {

	//open .och8 file
	std::ofstream outputFile("Source.och8", std::ios::app);

	//if file cant open
	if (!outputFile.is_open()) {
		std::cout << "ERROR : cannot open Source.och8 \n";
		return -1;
	}
	else {

		int lineNumber = 0;
		std::string line;
		

		//get label of beginning char set
		std::getline(*file, line);
		while (line.empty()) {
			std::getline(*file, line);
		}

		if (line != "CODEB") {
			std::cout << "ERROR : Theres NOT CODEB keyword \n";
			return -1;
		}

		outputFile << "CODEB ";
		std::pair<std::string, std::string> parts;

		bool isEmpty = false;

		while (std::getline(*file, line)) {

			//check end of code section
			if (line == "CODEF") {
				break;
			}
			
			//check if its empty line
			if (!isStrEmpty(line)) {

				//get parts
				parts = getParts(line);

				#ifndef DEBUG_OPCODE_PARTS
					std::cout << "first " << parts.first << " second " << parts.second << "\n";
				#endif // !DEBUG_OPCODE_PARTS

				//write to file
				writeMachineCode(&outputFile, parts);

			}
			
			//check syntax error
			if (parts.first == "0") {
				printError("syntax error at " + std::to_string(lineNumber));
				break;
			}

			
			//for error message
			lineNumber++;
		}

		if (line != "CODEF") {
			printError("End of CODE SECTION undefined or CODE SECTION has error\n");
			return -1;
		}

		outputFile << "CODEF\n";
		outputFile.close();

		return lineNumber;
	}
}

int main(int argc, char* argv[]) {

	std::cout << "writed by Onur Yucel in 13.08.23 \n";
	std::cout << "===============================" << "\n";
	std::cout << "Welcome to Chip8 Assembler V0.5" << "\n";
	std::cout << "===============================" << "\n";
	

	std::ifstream sourceFile("Source.ch8",std::ios::in);

	if (!sourceFile.is_open()) {
		std::cout << "ERROR : File couldnt opened \n";
	}
	else {

		if (writeCharset_toROM(&sourceFile) == -1) {
			sourceFile.close();
			return -1;
		}

		if (writeVariable_toROM(&sourceFile) == -1) {
			sourceFile.close();
			return -1;
		}
		writeCode_toROM(&sourceFile);
	}


	sourceFile.close();

	std::cout << ramUsed << " RAM bytes used " << 0xfff - ramUsed << " bytes left" << "\n";
}

	/*
	else if (line == "addr") {
			std::cout << "RAM address : " << ramAddress << " : vector size :"<< inputs.size() << "\n";
		}

		//ram limit check
		if (ramAddress + 3 >= 0xfff) {
			std::cout << "ERROR:: RAM out of limit \n";
		}

		//find end of the memonic (in this example its 3)

		//index          012345
		//EXAMPLE line = JMP 2F
		//lineIndex will be 3

		for (size_t i = 0; i < line.length(); i++) {
			if (line[lineIndex] == ' ') {
				break;
			}
			lineIndex++;
		}

		//search opcode is it exist in map key
		if (opCodes.find(line.substr(0, lineIndex)) != opCodes.end()) {

			if (line.substr(0, lineIndex) == "CLS" || line.substr(0, lineIndex) == "RET") {
				//get value
				std::string mapValue = opCodes[line.substr(0, lineIndex)];

				//split in two
				//push the hex code to vector
				inputs[ramAddress] = mapValue.substr(0, 4);
				ramAddress++;
				inputs[ramAddress] = "0x" + mapValue.substr(4, 2);
				ramAddress++;
			}
			else{
				//JMP F0F

				//0x1nnn <== JMP
				std::string hexOpcode = opCodes[line.substr(0, lineIndex)];

				//F0F <== JMP F0F
				std::string hexPart = line.substr(lineIndex + 1, line.size());

				#ifndef DEBUG_CONTROL
					std::cout << "hexOpcode" << hexOpcode << "\n";
				#endif // !DEBUG_CONTROL

				//let say hexOpcode = 0X12FF
				std::string machineCode = combine(hexOpcode, hexPart);;
				//push back 0x12
				inputs[ramAddress] = machineCode.substr(0, 4);
				ramAddress++;
				//push back FF
				inputs[ramAddress] = "0x" + machineCode.substr(4, 2);
				ramAddress++;
			}

		}
		else {
			std::cout << "Syntax Error" << "\n";
		}

		lineIndex = 0;

	}

	writeSourceFile(&inputs);
	*/