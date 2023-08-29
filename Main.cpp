#include "Assembler_ch8.h"

/*
		ROMB(in hex)
			0x0f.....
			......
			...
		ROME

		VARB(in dec)
			20,15,67

		VARE

		CODEB

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

		CODEF
	*/

int main(int argc, char* argv[]) {

	Assembler_ch8 assembler("Source.ch8", "Source.och8");

	if (assembler.run() == -1) {
		std::cout << "Assembler have error ! \n";
	}

}