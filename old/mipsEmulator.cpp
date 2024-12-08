// MIPS emulator - written by Kate for CS64

#include <cstdint>
#include <iostream>
#include <bitset>

using std::uint32_t;
using std::int32_t;


// create emulator cpu (global vars)
uint32_t PC; // program counter
uint32_t REGISTERS[32];
uint32_t iMEM[16384]; // 2^(16), read-only (ROM)
uint32_t dMEM[16384]; // 2^(16), read/write (RAM)
bool quit = false;

struct Operation {
	uint32_t rs = 0;
	uint32_t rt = 0;
	uint32_t rd = 0;
	int32_t shamt = 0;
	uint32_t funct = 0;
	uint32_t opcode = 0;
	int16_t immediate = 0;
	uint32_t address = 0;
};

void setRegZeroes() {
	for(int i = 0; i < 32; i++) {
		REGISTERS[i] = 0;
	}
}

int getnbits(int end, int start, uint32_t bits){
	// get (32 - end) number of bits
	int leftamt = 31 - end;
	// get (start + left amt) number of bits
	int rightamt = start + leftamt;
	// shift left by the leftamt, shift right by rightamt
	bits = bits << leftamt;
	bits = bits >> rightamt;
	// only return bits between start and end of the 32-bit string
	return bits;
}

// return operation, given instruction in bits
Operation decode(uint32_t instruction){
	Operation operation;
	operation.opcode = getnbits(31, 26, instruction);

	// if R-Type
	if (operation.opcode == 0){
		std::cout << "    Is R-Type " << std::endl;
		operation.rs = getnbits(25, 21, instruction);
		operation.rt = getnbits(20, 16, instruction);
		operation.rd = getnbits(15, 11, instruction);
		operation.shamt = getnbits(10, 6, instruction);
		operation.funct = getnbits(5, 0, instruction);
	}

	// if jump (J-Type)
	else if (operation.opcode == 2){
		std::cout << "    Is J-Type " << std::endl;
		operation.address = getnbits(25, 0, instruction);
	}

	// else: I-Type
	else {
		std::cout << "    Is I-Type " << std::endl;
		operation.rs = getnbits(25, 21, instruction);
		operation.rt = getnbits(20, 16, instruction);
		operation.immediate = getnbits(15, 0, instruction);
	}

	return operation;
}

// OPERATION DEFINITIONS

// opcode: 0, funct: 12
// syscall
void syscall() {
	if (REGISTERS[2] == 1) {
		//print int
		std::cout << REGISTERS[4] << std::endl;
	} else if (REGISTERS[2] == 4) {
		//print string
		std::cout << REGISTERS[4] << std::endl;
	} else if (REGISTERS[2] == 5) {
		//input string
		std::cin >> REGISTERS[4];
	} else if (REGISTERS[2] == 10) {
		//quit
		quit = true;

	}
}

//opcode: 0, funct: 32
// add rd, rs, rt
void add(const Operation operation) {
	// set rd to rs + rt
	REGISTERS[operation.rd] = REGISTERS[operation.rs] + REGISTERS[operation.rt];

	// Logging
	std::cout << "    Add $" << operation.rd << ", $" << operation.rs << ", $" << operation.rt << " -> " << REGISTERS[operation.rd] << std::endl;
}

//opcode: 35
// lw rt, 0(rs) (immediate shifts 32 bits not 4!)
void lw(const Operation operation) {
	// set rs to (index(rt) in dMEM[])
	REGISTERS[operation.rt] = dMEM[operation.rs + operation.immediate];

	// Logging
	std::cout << "    Lw $" << operation.rt << ", "<< operation.immediate << "($" << operation.rs << ") -> " << REGISTERS[operation.rt] << std::endl;
}

// 43 -> 10 1011
// sw rt, 0(rs)
void sw(const Operation operation) {
	// set (index(rt) in dMEM[]) to rs
	dMEM[operation.rs + operation.immediate] = REGISTERS[operation.rt];

	// Logging
	// std::cout << "Sw $" << operation.rt << ", "<< operation.immediate << "($" << operation.rs << ") -> " << dMEM[operation.rs + operation.immediate] << std::endl;
	std::cout << "    Sw $" << operation.rt << ", "<< operation.immediate << "($" << operation.rs << ") -> " << REGISTERS[operation.rt] << std::endl;

}

//opcode: 8 -> 00 1000
// addi rt, rs, immediate
void addi(const Operation operation) {
	// set rt to rs + imm
	REGISTERS[operation.rt] = REGISTERS[operation.rs] + operation.immediate;

	// Logging
	std::cout << "    Addi $" << operation.rt << ", $" << operation.rs << ", " << operation.immediate << " -> " << REGISTERS[operation.rt] << std::endl;
}

//opcode: 4 -> 00 0100
// addi rt, rs, immediate
void beq(const Operation operation) {

}

//opcode: 2 -> 00 0010
// addi rt, rs, immediate
void jump(const Operation operation) {

}

int main() // argv: 1 = opcode (in bits)
{
	setRegZeroes();

	PC = 0;
	while (true) {
		uint32_t instruction;

		std::cin >> std::hex >> instruction;

		// "end" in string
		if(instruction == 0 || quit) {
			break;
		}

		iMEM[PC] = instruction;
		PC++;
	}

	for (int i = 0; i < PC; i++) {
		uint32_t instruction = iMEM[i];

		// break up binary code into struct
		Operation operation = decode(instruction);

		std::cout << "    funct: " << operation.funct << std::endl;

		// execute
		if(operation.opcode == 0) {
			if (operation.funct == 12) {
				syscall();
			} else if (operation.funct == 32) {
				add(operation);
			} 
		} else if(operation.opcode == 35) {
			lw(operation);
		} else if(operation.opcode == 43) {
			sw(operation);
		} else if(operation.opcode == 8) {
			addi(operation);
		} else if(operation.opcode == 4) {
			beq(operation);
		} else if(operation.opcode == 2) {
			jump(operation);
		}
	}

	return 0;
}