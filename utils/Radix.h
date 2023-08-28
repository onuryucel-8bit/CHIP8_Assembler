#pragma once

#include<cstdint>
#include<string>
#include<sstream>

namespace rdx {

	/*
	* hex => dec
	* dec => bin
	* dec => hex
	*/

	//TODO 
	uint8_t bin_toDec(std::string bin);

	uint8_t toDec(std::string hex);

	std::string toBin(int dec);

	std::string toHex(uint16_t dec);
}