// remove these when working with the Mullifier
// also remove all cout and assert statements
#include <iostream>
#include <cassert>

// CPU STATE
int PC; // program counter
int REGISTERS[32];
// int iMEM[16384]; // 2^(16), read-only (ROM)
int dMEM[16384]; // 2^(16), read/write (RAM)
bool skip_pc_increment = false;
// bool quit = false;

// GLOBAL INSTRUCTION DECODE DATA
int opcode;
int imm;
int rs1;
int rs2;
int funct3;
int funct7;
int rd;
int instruction;

/////////////// CONVENIENCE FUNCTIONS ///////////////
// used for bitshifting
int bitarrayBuffer[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int powersOfTwo[32] = {
    1, 2, 4, 8, 16, 32, 64, 128, 256,
    512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072,
    262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216,
    33554432, 67108864, 134217728, 268435456, 536870912, 1073741824,
    -2147483648
};

int resetBitarrayBuffer() {
    for (int i = 0; i < 32; i++) {
        bitarrayBuffer[i] = 0;
    }
    return 0;
}

int numToBits(int val) {
    // populates result in bitarrayBuffer
    // NOTE: MSB is at index 31 (it's reversed)
    resetBitarrayBuffer();
    
    if (val < 0) {
        bitarrayBuffer[31] = 1;
        val += powersOfTwo[31];
    }

    for (int i = 30; i >= 0; i--) {
        if (val >= powersOfTwo[i]) {
            bitarrayBuffer[i] = 1;
            val -= powersOfTwo[i];
        }
    }

    return 0;
}

int bitsToNum() {
    int reconstruct = 0;
    if (bitarrayBuffer[31] == 1) {
        reconstruct = 0-powersOfTwo[31];
    }
    
    for (int i=0; i<31; i++) {
        if (bitarrayBuffer[i] == 1) {
            reconstruct += powersOfTwo[i];
        }
    }

    return reconstruct;
}

int leftShift(int x, int n) {
    // in bitarrayBuffer, MSB is at index 31 (it's reversed)
    // so this implementation looks like a right shift
    numToBits(x);

    // first shift the top 31-n bits off, starting from the top
    for (int i = 31; i >= n; i--) {
        bitarrayBuffer[i] = bitarrayBuffer[i-n];
    }

    // then shift in n zeroes at the bottom
    for (int i = 0; i < n; i++) {
        bitarrayBuffer[i] = 0;
    }

    return bitsToNum();
}

int rightShift(int x, int n) {  // this is a LOGICAL right shift
    // calculates floor(x / 2^n)
    numToBits(x); // put the bits of x in an array

    for (int i = 0; i<32; i++) {
        if (i+n < 32) {
            bitarrayBuffer[i] = bitarrayBuffer[i+n];
        } else {  // shift in zeroes at the top
            bitarrayBuffer[i] = 0;
        }
            
    }

    return bitsToNum();
}

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
    bits = leftShift(bits, leftamt);
	bits = rightShift(bits, rightamt);

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

/////////////// CPU EMULATOR FUNCTIONS ///////////////
/////////////// R Type ///////////////
int add_op() {
    REGISTERS[rd] = REGISTERS[rs1] + REGISTERS[rs2];
}

int sub_op() {
    REGISTERS[rd] = REGISTERS[rs1] - REGISTERS[rs2];
}

int and_op() { return 0; }

int or_op() { return 0; }

int sll_op() {
   REGISTERS[rd] = leftShift(REGISTERS[rs1], REGISTERS[rs2]);
}

int srl_op() {
    REGISTERS[rd] = rightShift(REGISTERS[rs1], REGISTERS[rs2]);
}

int sra_op() {
    REGISTERS[rd] = rightShift(REGISTERS[rs1], REGISTERS[rs2]);
}

int slt_op()  { return 0; }
int sltu_op() { return 0; }

/////////////// I Type ///////////////

int addi_op() {
    REGISTERS[rd] = REGISTERS[rs1] + imm;
}

int xor_op() { return 0; }
int ori()    { return 0; }
int andi()   { return 0; }
int slli()   { return 0; }
int srli()   { return 0; }
int srai()   { return 0; }
int slti()   { return 0; }

int lb_op(){
    // rd = M[rs1+imm][0:7]
    REGISTERS[rd] = getnbits(7, 0, dMEM[rs1+imm]);
    return 0;
}

int lh_op(){
    // rd = M[rs1+imm][0:15]
    REGISTERS[rd] = getnbits(15, 0, dMEM[rs1+imm]);
    return 0;
}

int lw_op(){
    // rd = M[rs1+imm][0:31]
    REGISTERS[rd] = dMEM[REGISTERS[rs1]+imm];
    return 0;
}

int ori_op()   { return 0; }
int andi_op()  { return 0; }
int s_type()   { return 0; }
int xori_op()  { return 0; }
int srli_op()  { return 0; }
int srai_op()  { return 0; }
int slti_op()  { return 0; }
int sltiu_op() { return 0; }
int slli_op()  { return 0; }

int jalr_op() {
    REGISTERS[rd] = PC + 4;
    PC = REGISTERS[rs1] + imm;
    return 0;
}

/////////////// S Type ///////////////

int sb_op(){ return 0; }
int sh_op(){ return 0; }

int sw_op() {
    // M[rs1+imm][0:31] = rs2[0:31]
    dMEM[REGISTERS[rs1]+imm] = REGISTERS[rs2];
    return 0;
}

/////////////// B Type ///////////////

int beq_op() {
    if (REGISTERS[rs1] == REGISTERS[rs2]) {
        PC += imm;
        skip_pc_increment = true;
    } 
    return 0;
}

int bne_op() {
    if (rs1 != rs2) {
        PC += imm;
    } 
    return 0;
}

int blt_op() {
    if (rs1 < rs2) {
        PC += imm;
    } 
    return 0;
}

int bge_op() {
    if (rs1 >= rs2) {
        PC += imm;
    } 
    return 0;
}

int bltu_op() { return 0; }

int bgeu_op() { return 0; }

/////////////// J Type ///////////////

/////////////// U Type ///////////////

/////////////// DECODE LOGIC ///////////////

int decode() {
    /*
    input: instruction
    */

    // Types: R, I, S, B, U, J
    // take opcode (last 6 bits) and determine type
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

        if (opcode == 19) {
            if (funct3 == 0) {
                addi_op();
            }
            else if (funct3 == 4) {
                xori_op();
            } else if (funct3 == 6) {
                ori_op();
            } else if (funct3 == 7) {
                andi_op();
            } else if (funct3 == 1) {
                int _funct7 = getnbits(31, 25, instruction);
                if (_funct7 == 0) slli_op();
            } else if (funct3 == 5) {
                int _funct7 = getnbits(31, 25, instruction);
                if (_funct7 == 0) {
                    srli_op();
                } else if (_funct7 == 32) {
                    srai_op();
                }
            } else if (funct3 == 2) {
                slti_op();
            } else if (funct3 == 3) {
                sltiu_op();
            }

        } else if (opcode == 19) {
            
        } else if (opcode == 3) {   // these are all loads
            if (funct3 == 0) {  // load byte
                lb_op();
            } else if (funct3 == 1) {   // load half
                lh_op();
            } else if (funct3 == 2) {  // load word
                lw_op();
            } else if (funct3 == 4) {  // load byte (unsigned)

            } else if (funct3 == 5) {  // load half (unsigned)

            }
        } else if (opcode == 103) {
            if (funct3 == 0) {
                jalr_op();
            }
        } else if (opcode == 115) {
            
        }
        
    } else if (opcode == 35) { // S type
        // imm(31 to 25 | 11 to 7) rs2 rs1(19 to 15) funct3(14 to 12) imm (11 to 7) opcode (6 to 0)
        imm = leftShift(getnbits(31, 25, instruction), 5) + getnbits(11, 7, instruction);
        rs2 = getnbits(24, 20, instruction);
        rs1 = getnbits(19, 15, instruction);
        funct3 = getnbits(14, 12, instruction);
        if (funct3 == 0) {  // store byte
            sb_op();
        } else if (funct3 == 1) {   // store half
            sh_op();
        } else if (funct3 == 2) {  // store word
            sw_op();
        }

    } else if (opcode == 99) { // B type
        imm = leftShift(getnbits(31, 31, instruction), 12)
          + leftShift(getnbits(7, 7, instruction), 11)
          + leftShift(getnbits(30, 25, instruction), 5)
          + leftShift(getnbits(11, 8, instruction), 1);

        rs2 = getnbits(24, 20, instruction);
        rs1 = getnbits(19, 15, instruction);
        funct3 = getnbits(14, 12, instruction);
        if (funct3 == 0) {  // branch eq
            beq_op();
        } else if (funct3 == 1) {   // branch not eq
            bne_op();
        } else if (funct3 == 4) {  // branch less than
            blt_op();
        } else if (funct3 == 5) {  // branch greater than
            bge_op();
        } else if (funct3 == 6) {  // branch less than (unsigned)
            bltu_op();
        } else if (funct3 == 7) {  // branch greater than (unsigned)
            bgeu_op();
        }

    } else if (opcode == 55 | opcode == 23) { // U type
        // imm(31 to 12) rd (11 to 7) 
        imm = getnbits(31, 12, instruction);
        rd = getnbits(11, 7, instruction);

    } else if (opcode == 111) { // J type

    }
}


int main() {
    int instructions[9] = {
      5275795,
      5308691,
      1147283,
      68263971,
      67215875,
      2228579,
      115,
      104989203,
      115
    };

    PC = 0;
    while(PC < 9 && instructions[PC] != 115) {
        skip_pc_increment = false;
        instruction = instructions[PC];

        opcode = 0;
        imm = 0;
        rs1 = 0;
        rs2 = 0;
        funct3 = 0;
        funct7 = 0;
        rd = 0;

        decode();

        std::cout << "instruction " << instruction << std::endl;
        std::cout << "rd " << rd << std::endl;
        std::cout << "rs1: " << rs1 << " = " << REGISTERS[rs1] << std::endl;
        std::cout << "rs2: " << rs2 << " = " << REGISTERS[rs2] << std::endl;
        std::cout << "funct3: " << funct3 << " = " << funct3 << std::endl;
        std::cout << "opcode: " << opcode << " = " << opcode << std::endl;
        std::cout << "imm " << imm << std::endl;
        std::cout << "RESULT " << REGISTERS[rd] << std::endl;
        std::cout << "PC AFTER " << PC << std::endl;
        std::cout << std::endl;

        if (!skip_pc_increment) {
          PC++;
        }
    }

    std::cout << "DONE" << std::endl;
    std::cout << "x1: " << REGISTERS[1] << std::endl;
    std::cout << "x2: " << REGISTERS[2] << std::endl;
    std::cout << "x3: " << REGISTERS[3] << std::endl;
    std::cout << "x4: " << REGISTERS[4] << std::endl;
}
