#include "Assembler_ch8.h"

Assembler_ch8::Assembler_ch8(std::string sourcePath, std::string outputPath) {

	this->sourcePath = sourcePath;
	this->outputPath = outputPath;


	//TODO check file paths are valid

}

//print out error message with lineNumber after print close source and output files
void Assembler_ch8::printError(std::string message) {
	std::cout << "ERROR: at " << lineNumber << " " << message << "\n";
	sourceFile.close();
	outputFile.close();
}

bool Assembler_ch8::isInRange(std::string line) {

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

bool Assembler_ch8::isStrEmpty(std::string str) {
	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] != ' ') {
			return false;
		}
	}

	return true;
}

std::pair<std::string, std::string> Assembler_ch8::getParts(std::string line) {

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
		else {
			parts.second = line.substr(j + 1, line.length() - j);//20
		}

		return parts;
	}

	return parts;

}

void Assembler_ch8::writeMachineCode(std::ofstream* outputFile, std::pair<std::string, std::string> parts) {

	if (parts.first == "CLS" || parts.first == "RET") {
		*outputFile << opCodes[parts.first].substr(0, 4) << ",0x" << opCodes[parts.first].substr(4, 2) << ",";

	}
	else {
		std::string machineCode = combine(opCodes[parts.first], parts.second.substr(1, 3));

#ifndef DEBUG_MACHINE_CODE
		std::cout << "Machine code = " << machineCode << "\n";
#endif // DEBUG_MACHINE_CODE

		* outputFile << machineCode.substr(0, 4) << ",0x" << machineCode.substr(4, 2) << ",";

	}

	lineSlider++;

	if (lineSlider == 5) {
		lineSlider = 0;
		*outputFile << "\n";
	}

	ramUsed += 2;
}

std::string Assembler_ch8::combine(std::string hexOpcode, std::string hexPart) {

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

int Assembler_ch8::writeCharset_toROM() {

	std::string line;
	size_t blankLineIndex;

	//get label of beginning char set
	std::getline(sourceFile, line);
	while (line.empty()) {
		std::getline(sourceFile, line);
	}

	if (line != "ROMB") {
		printError("getCharsetData() : Theres NOT begining of Charset keyword");
		return -1;
	}

	outputFile << "ROMB ";
	char lineSlider = 0;

	while (std::getline(sourceFile, line)) {

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
		printError("ERROR: End of char set undefined !");
		return -1;
	}

	outputFile << "ROME\n";

	return 0;
}

int Assembler_ch8::writeVariable_toROM() {

	std::string line;

	//get label of beginning char set
	std::getline(sourceFile, line);
	while (line.empty()) {
		std::getline(sourceFile, line);
	}

	//check VARB
	if (line != "VARB") {
		std::cout << "ERROR : getCharsetData() : Theres NOT VARB keyword \n";
		return -1;
	}

	outputFile << "VARB ";

	size_t blankLineIndex;
	char lineSlider = 0;

	while (std::getline(sourceFile, line)) {

		//if line at end of char set break the loop
		if (line == "VARE") {
			break;
		}
		else {
			/*EXAMPLE:		  01234  ->INDEX
			*		          --253  ->LINE
			*	  	  		    ^
			*  blankLineIndex---/ => 2
			*  line.length() => 5
			*  isRange(253);
			*
			*/
			blankLineIndex = line.find_first_not_of(' ') + 1;

			//check number boundries 0<= x <= 0xff
			if (!isInRange(line.substr(blankLineIndex, line.length() - blankLineIndex))) {
				printError("variable size is big or small LINE = " + line.substr(blankLineIndex, line.length() - blankLineIndex));
				return -1;
			}


			outputFile << line.substr(blankLineIndex, line.length() - blankLineIndex) << ",";
			ramUsed++;

			lineSlider++;

			//lineSlider Control
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

	return 0;
}

int Assembler_ch8::writeCode_toROM() {

	std::string line;

	//get label of beginning char set
	std::getline(sourceFile, line);
	while (line.empty()) {
		std::getline(sourceFile, line);
	}

	if (line != "CODEB") {
		std::cout << "ERROR : Theres NOT CODEB keyword \n";
		return -1;
	}

	outputFile << "CODEB ";
	std::pair<std::string, std::string> parts;

	bool isEmpty = false;

	while (std::getline(sourceFile, line)) {

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

	return 0;
}

int Assembler_ch8::run() {

	std::cout << "writed by Onur Yucel in 13.08.23 \n";
	std::cout << "===============================" << "\n";
	std::cout << "Welcome to Chip8 Assembler V0.5" << "\n";
	std::cout << "===============================" << "\n";

	outputFile.open(outputPath, std::ios::out);
	sourceFile.open(sourcePath, std::ios::in);

	if (!outputFile.is_open()) {
		printError("cannot open output file");
		return -1;
	}

	if (!sourceFile.is_open()) {
		printError("cannot open source file");
		return -1;
	}

	if (writeCharset_toROM() == -1) {
		return -1;
	}

	if (writeVariable_toROM() == -1) {
		return -1;
	}

	writeCode_toROM();

	//close files
	sourceFile.close();
	outputFile.close();

	std::cout << ramUsed << " RAM bytes used " << 0xfff - ramUsed << " bytes left" << "\n";

	return 0;
}
