#include "Radix.h"

namespace rdx {

	//TODO !code here
	uint8_t bin_toDec(std::string bin) {

		return 0;
	}

	uint8_t toDec(std::string hex) {
		
		//0xFE => FE

		unsigned int dec = std::stoul(hex, nullptr, 16);

		return (uint8_t)(dec);
	}

	std::string toBin(int dec) {
		std::string bin = "";

		while (dec > 1) {

			bin += std::to_string(dec % 2);
			dec /= 2;
		}

		bin += std::to_string(dec);

		std::reverse(bin.begin(), bin.end());

		return bin;
	}

	std::string toHex(uint16_t dec) {

		std::stringstream ss;
		ss << std::hex << std::uppercase << dec;
		return ss.str();
	}
}