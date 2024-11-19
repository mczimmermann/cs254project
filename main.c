#include <iostream>

// TODOS:
// get add, addi working
// get multiple instructions working (in a loop over instructions)
// get lw, sw working
// speed up bitshift by precomputing powers of 2

// CPU STATE
uint32_t PC; // program counter
uint32_t REGISTERS[32];
// int iMEM[16384]; // 2^(16), read-only (ROM)
// int dMEM[16384]; // 2^(16), read/write (RAM)
// bool quit = false;

// GLOBAL INSTRUCTION DECODE DATA
uint32_t opcode;
uint32_t imm;
uint32_t rs1;
uint32_t rs2;
uint32_t funct3;
uint32_t funct7;
uint32_t rd;


// CONVENIENCE FUNCTIONS
//1001 (9), leftshift(9, 3) -> 1001000 (72)
uint32_t leftShift(uint32_t x, int n) {
    // calculates x * 2^n

    // Calculate 2^n using repeated addition
    uint32_t power_of_two = 1;
    for (int i = 0; i < n; ++i) {
        power_of_two += power_of_two; // Double each time
    }
    
    // Multiply x by 2^n using repeated addition
    int result = 0;
    for (int i = 0; i < power_of_two; ++i) {
        result += x;
    }
    
    return result;
}

uint32_t rightShift(uint32_t x, int n) {
    // calculates floor(x / 2^n)

    // Calculate 2^n using repeated addition
    uint32_t power_of_two = 1; // 2^0
    for (int i = 0; i < n; ++i) {
        power_of_two += power_of_two; // Double each time
        // std::cout << "FOR n= " << n << " EXPECT " << 1<<n << "POWER OF TWO " << power_of_two << std::endl;
    }
    // std::cout << "POWER OF TWO " << power_of_two << std::endl;
    
    // Divide x by 2^n using repeated subtraction
    int quotient = 0;
    while (x >= power_of_two) {
        x -= power_of_two;
        quotient++;
    }
    
    return quotient;
}


uint32_t getnbits(int msb, int lsb, uint32_t bits){
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
    // bits = leftShift(bits, leftamt);
	// bits = rightShift(bits, rightamt);
    bits = bits << leftamt;
	bits = bits >> rightamt;

	// only return bits between start and end of the 32-bit string
	return bits;
}

int bitwiseAnd(int x1, int x2) {
    // computes bitwise AND between x1 and x2
    // (both 32 bit integers)
    
    int result = 0;
    for (int bitIdx = 0; bitIdx < 32; bitIdx++) {
        // int x1_bit = getnbits(bitIdx, bitIdx);
        int x = 0;
    }

    return result;
}


// CPU EMULATOR FUNCTIONS
int add_op() {
    REGISTERS[rd] = REGISTERS[rs1] + REGISTERS[rs2];
}

int addi_op() {
    REGISTERS[rd] = REGISTERS[rs1] + imm;
}

int sub_op() {
    REGISTERS[rd] = REGISTERS[rs1] - REGISTERS[rs2];
}

int and_op() {
    return 0;
}

int or_op() {
    REGISTERS[rd] = REGISTERS[rs1] || REGISTERS[rs2];

}

int sll_op() {
   REGISTERS[rd] = leftShift(REGISTERS[rs1], REGISTERS[rs2]);
}

int srl_op() {
    REGISTERS[rd] = rightShift(REGISTERS[rs1], REGISTERS[rs2]);
}
// this is unclear, but I think for signed integers x / 2n is equivalent to shift left arithmetic

int sra_op() {
    REGISTERS[rd] = rightShift(REGISTERS[rs1], REGISTERS[rs2]);
}

int slt_op() {
    return 0;
}

int sltu_op() {
    return 0;
}

int xor_op() {
    return 0;
}

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
            add_op();
        } else if (funct3 == 0 && funct7 == 32) {
            sub_op();
        } else if (funct3 == 4 && funct7 == 0) {
            xor_op();
        } else if (funct3 == 6 && funct7 == 0) {
            or_op();
        } else if (funct3 == 7 && funct7 == 0) {
            and_op();
        } else if (funct3 == 1 && funct7 == 0) {
            sll_op();
        } else if (funct3 == 5 && funct7 == 0) {
            srl_op();
        } else if (funct3 == 5 && funct7 == 32) {
            sra_op();
        } else if (funct3 == 2 && funct7 == 0) {
            slt_op();
        } else if (funct3 == 3 && funct7 == 0) {
            sltu_op();
        }
    } else if (opcode == 3 || opcode == 19 || opcode == 115 || opcode == 103) { // I type
        // imm(31 to 20) rs1(19 to 15) funct3(14 to 12) rd (11 to 7) opcode (6 to 0)
        imm = getnbits(31, 20, instruction);
        rs1 = getnbits(19, 15, instruction);
        funct3 = getnbits(14, 12, instruction);
        rd = getnbits(11, 7, instruction);
        
    } else if (opcode == 35) { // S type
        // imm(31 to 25 | 11 to 7) rs2 rs1(19 to 15) funct3(14 to 12) imm (11 to 7) opcode (6 to 0)
        imm = leftShift(getnbits(31, 20, instruction), 6) + getnbits(11, 7, instruction);
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

int main() {
    // add x5, x6, x7
    // int instr_to_decode = 7537331;
    // decode(instr_to_decode);

    // testing bit shifter operation
    // 1010 (10)
    std::cout << "GETNBITS " << getnbits(3, 1, 10) << std::endl; // should give 5

    uint32_t leftShifted = leftShift(1, 31);
    std::cout << "RESULT " << leftShifted << std::endl; // should give 2147483648

    int rightShifted = rightShift(leftShifted, 31);
    std::cout << "RESULT " << rightShifted << std::endl; // should give 1

    std::cout << "GETNBITS " << getnbits(1, 1, 10) << std::endl; // should give 1
    std::cout << "GETNBITS " << getnbits(2, 2, 10) << std::endl; // should give 0
}