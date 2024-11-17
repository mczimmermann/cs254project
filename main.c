#include <iostream>

// TODOS:
// get add, addi working
// get multiple instructions working (in a loop over instructions)
// get lw, sw working

// CPU STATE
int PC; // program counter
int REGISTERS[32];
// int iMEM[16384]; // 2^(16), read-only (ROM)
// int dMEM[16384]; // 2^(16), read/write (RAM)
// bool quit = false;

// GLOBAL INSTRUCTION DECODE DATA
int opcode;
int imm;
int rs1;
int rs2;
int funct3;
int funct7;
int rd;
int opcode;


// CONVENIENCE FUNCTIONS

int getnbits(int msb, int lsb, int bits){
  /*
  input: end, start, bits

  end: ending bit (MSB)
  start: starting bit (LSB)
  bits: bitstring input

  example: getnbits(3, 1, 0b1010) -> 101
  */

	// get (32 - end) number of bits
	int leftamt = 31 - msb;
	// get (start + left amt) number of bits
	int rightamt = lsb + leftamt;
	// shift left by the leftamt, shift right by rightamt
	bits = bits << leftamt;
	bits = bits >> rightamt;
	// only return bits between start and end of the 32-bit string
	return bits;
}


// CPU EMULATOR FUNCTIONS

int decode(int instruction) {
    /*
    input: instruction
    */

    // Types: R, I, S, B, U, J
    // 1. take last opcode (last 6 bits) and determine type

    // R = 0110011
    // I = 0000011 or 0010011 or 1110011 or 1100111
    // S = 0100011
    // B = 1100011
    // J = 1101111
    // U = 0110111 or 0010111


    opcode = getnbits(6, 0, instruction);

    if (opcode == 51) { // R type
        // func7 (31 to 25) rs2(24 to 20) rs1(19 to 15) funct3(14 to 12) rd (11 to 7) opcode (6 to 0)

        funct7 = getnbits(31, 25, instruction);
        rs2 = getnbits(24, 20, instruction);
        rs1 = getnbits(19, 15, instruction);
        funct3 = getnbits(14, 12, instruction);
        rd = getnbits(11, 7, instruction);


        //call function for execution
        if (funct3 == 0 && funct7 == 0) {
            add();
        } else if (funct3 == 0 && funct7 == 32) {
            sub();
        } else if (funct3 == 4 && funct7 == 0) {
            xor();
        } else if (funct3 == 6 && funct7 == 0) {
            or();
        } else if (funct3 == 7 && funct7 == 0) {
            and();
        } else if (funct3 == 1 && funct7 == 0) {
            sll();
        } else if (funct3 == 5 && funct7 == 0) {
            srl();
        } else if (funct3 == 5 && funct7 == 32) {
            sra();
        } else if (funct3 == 2 && funct7 == 0) {
            slt();
        } else if (funct3 == 3 && funct7 == 0) {
            sltu();
        }
    } else if (opcode == 3 || opcode == 19 || opcode == 115 || opcode == 103) { // I type
        // imm(31 to 20) rs1(19 to 15) funct3(14 to 12) rd (11 to 7) opcode (6 to 0)
        imm = getnbits(31, 20, instruction);
        rs1 = getnbits(19, 15, instruction);
        funct3 = getnbits(14, 12, instruction);
        rd = getnbits(11, 7, instruction);
        
    } else if (opcode == 35) { // S type
        // imm(31 to 25 | 11 to 7) rs2 rs1(19 to 15) funct3(14 to 12) imm (11 to 7) opcode (6 to 0)
        imm = getnbits(31, 20, instruction) << 6 + getnbits(11, 7, instruction);
        rs2 = getnbits(24, 20, instruction);
        rs1 = getnbits(19, 15, instruction);
        funct3 = getnbits(14, 12, instruction);

    } else if (opcode == 99) { // B type

    } else if (opcode == 55 | opcode == 23) { // U type
        // imm(31 to 12) rd (11 to 7) 
        imm = getnbits(31, 12, instruction);
        rd = getnbits(11, 7, instruction);

    } else if (opcode == 111) { // J type

    }
}

int add() {
    return 0;
}

int addi() {
    return 0;
}

int sub() {
    return 0;
}

int xor() {
    return 0;
}

int main() {
    // add x5, x6, x7
    int instr_to_decode = 7537331;
    decode(instr_to_decode);
}