#include <iostream>

// TODOS:
// get multiple instructions working (in a loop over instructions)
// get lw, sw working
// speed up bitshift by precomputing powers of 2

// CPU STATE
int PC; // program counter
int REGISTERS[32];
// int iMEM[16384]; // 2^(16), read-only (ROM)
int dMEM[16384]; // 2^(16), read/write (RAM)
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

// CONVENIENCE FUNCTIONS
//1001 (9), leftshift(9, 3) -> 1001000 (72)
int leftShift(int x, int n) {
    // calculates x * 2^n

    // Calculate 2^n using repeated addition
    int power_of_two = 1;
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

int rightShift(int x, int n) {
    // calculates floor(x / 2^n)

    // Calculate 2^n using repeated addition
    int power_of_two = 1; // 2^0
    for (int i = 0; i < n; ++i) {
        power_of_two += power_of_two; // Double each time
    }
    
    // Divide x by 2^n using repeated subtraction
    int quotient = 0;
    while (x >= power_of_two) {
        x -= power_of_two;
        quotient++;
    }

    return quotient;
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

    // TODO: use leftShift and rightShift, don't use uint32_t
    // bits = leftShift(bits, leftamt);
	// bits = rightShift(bits, rightamt);
    uint32_t bitsu = bits;
    bitsu = bitsu << leftamt;
	bitsu = bitsu >> rightamt;
    bits = bitsu;

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
/////////////// R Type ///////////////
int add_op() {
    // std::cout  << "Rd: " << REGISTERS[rd];
    REGISTERS[rd] = REGISTERS[rs1] + REGISTERS[rs2];
}

int sub_op() {
    REGISTERS[rd] = REGISTERS[rs1] - REGISTERS[rs2];
}

int and_op() { return 0; }

int or_op() { // TODO: fix
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
    std::cout << "LOADING WORD" << " LOC " << REGISTERS[rs1]+imm << " VALUE " << dMEM[REGISTERS[rs1]+imm] << std::endl;
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
    std::cout << "RD =  " << REGISTERS[rd] << std::endl;
    std::cout << "RS1 = " << REGISTERS[rs1] << std::endl;
    std::cout << "imm =  " << imm << std::endl;
    REGISTERS[rd] = PC + 4;
    PC = REGISTERS[rs1] + imm;
    std::cout << "PC AFTER " << PC << std::endl;
    return 0;
}

/////////////// S Type ///////////////

int sb_op(){ return 0; }
int sh_op(){ return 0; }

int sw_op() {
    std::cout << "STORING WORD " << "VALUE " << REGISTERS[rs2] << " LOC " << REGISTERS[rs1]+imm << " " << std::endl;
    // M[rs1+imm][0:31] = rs2[0:31]
    dMEM[REGISTERS[rs1]+imm] = REGISTERS[rs2];
    return 0;
}

/////////////// B Type ///////////////

/*************  ✨ Codeium Command ⭐  *************/
/**
 * beq_op
 *
 * Branches to PC + imm if rs1 == rs2.
 *
 * @return 0
 */
/******  1e5c73e0-3f35-427b-abf8-5f268c287494  *******/
int beq_op() {
    if (rs1 == rs2) {
        PC += imm;
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

int bltu_op() {
    return 0;
}

int bgeu_op() {
    return 0;
}

/////////////// J Type ///////////////

/////////////// U Type ///////////////


int decode() {
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
        imm = leftShift(getnbits(31, 30, instruction), 12) + leftShift(getnbits(7, 6, instruction), 11);
        imm += leftShift(getnbits(30, 25, instruction), 5) + leftShift(getnbits(11, 8, instruction), 1);
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

    //RISC-V Generator: https://luplab.gitlab.io/rvcodecjs/#q=lw+x3,+0(x2)&abi=false&isa=AUTO

    // EXPECTED BEHAVIOR (x6=5; x5=x6+3; exit)
    // instruction, decimal
    // addi x6, x6, 5 = 5440275
    // addi x5, x6, 3 = 3342995
    // ecall = 115
    // int instructions[3] = {5440275, 3342995, 115};

    // EXPECTED BEHAVIOR (x1=10; x2=420; store x1 at addr in x2; load from addr in x2 to x3; exit)
    //     x3 should be the same as x1
    // instruction, decimal
    // addi x1, x1, 5 = 0x00508093 = 5275795
    // addi x4, x4, 6 = 0x00620213  =  6423059 (loc of exit instruction) TODO: change to work with 4-byte PC
    // jalr x0, 0(x4) = 0x00020067 = 131175 (SHOULD BE SOMETHING ELSE, dont write to 0)
    // add x1, x1, x1 = 0x001080b3 = 1081523
    // addi x2, x2, 420 = 0x1a410113 = 440467731
    // sw x1, 0(x2) = 0x00112023 = 1122339
    // lw x3, 0(x2) = 0x00012183 = 74115
    // ecall = 115

    int instructions[8] = {
        5275795,
        6423059,
        131175,
        1081523,
        440467731,
        1122339,
        74115,
        115
    };

    PC = 0;
    while(instructions[PC] != 115) {
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
        std::cout << "RESULT " << REGISTERS[rd] << std::endl; // should give 9
        std::cout << "PC AFTER " << PC << std::endl;
        std::cout << std::endl;

        PC++;
    }

    std::cout << "DONE" << std::endl;
    std::cout << "x1: " << REGISTERS[1] << std::endl;
    std::cout << "x2: " << REGISTERS[2] << std::endl;
    std::cout << "x3: " << REGISTERS[3] << std::endl;

    // test: add x5, x6, x7
    // int instr_to_decode = 7537331;
    // decode(instr_to_decode);

    // test: addi x5, x6, 3
}