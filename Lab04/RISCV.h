/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * RISC-V Definitions Module */

// Start Header Guard
#ifndef GHT_RISCV
#define GHT_RISCV

//============================================
// Constants and Macros
//============================================
//--------------------------------------------
//ALUOP Values
//--------------------------------------------
#define ALU_ADD  0b0000
#define ALU_SUB  0b1000
#define ALU_AND  0b0001
#define ALU_OR   0b0010
#define ALU_XOR  0b0011
#define ALU_SLL  0b0100
#define ALU_SRL  0b0101
#define ALU_SRA  0b1101
#define ALU_CMP  0b0111
#define ALU_CMPU 0b1111
//--------------------------------------------
//Register File Register Names
//--------------------------------------------
// bank[0]
#define X0 0U
#define XZR 0U //Zero
// bank[1]
#define X1 1U
#define XR 1U //Return Address
// bank[2]
#define X2 2U
#define XSP 2U //Stack Pointer
// bank[3]
#define X3 3U
#define XGP 3U //Global Pointer
// bank[4]
#define X4 4U
#define XTP 4U //Thread Pointer
// bank[5]
#define X5 5U
#define XT0 5U //Temporary 0
// bank[6]
#define X6 6U
#define XT1 6U //Temporary 1
// bank[7]
#define X7 7U
#define XT2 7U //Temporary 2
// bank[8]
#define X8 8U
#define XS0 8U //Saved 0
#define XFP 8U //Frame Pointer
// bank[9]
#define X9 9U
#define XS1 9U //Saved 1
// bank[10]
#define X10 10U
#define XA0 10U //Argument 0
#define XR0 10U //Return 0
// bank[11]
#define X11 11U
#define XA1 11U //Argument 1
#define XR1 11U //Return 1
// bank[12]
#define X12 12U
#define XA2 12U //Argument 2
// bank[13]
#define X13 13U
#define XA3 13U //Argument 3
// bank[14]
#define X14 14U
#define XA4 14U //Argument 4
// bank[15]
#define X15 15U
#define XA5 15U //Argument 5
// bank[16]
#define X16 16U
#define XA6 16U //Argument 6
// bank[17]
#define X17 17U
#define XA7 17U //Argument 7
// bank[18]
#define X18 18U
#define XS2 18U //Saved 2
// bank[19]
#define X19 19U
#define XS3 19U //Saved 3
// bank[20]
#define X20 20U
#define XS4 20U //Saved 4
// bank[21]
#define X21 21U
#define XS5 21U //Saved 5
// bank[22]
#define X22 22U
#define XS6 22U //Saved 6
// bank[23]
#define X23 23U
#define XS7 23U //Saved 7
// bank[24]
#define X24 24U
#define XS8 24U //Saved 8
// bank[25]
#define X25 25U
#define XS9 25U //Saved 9
// bank[26]
#define X26 26U
#define XS10 26U //Saved 10
// bank[27]
#define X27 27U
#define XS11 27U //Saved 11
// bank[28]
#define X28 28U
#define XT3 28U //Temporary 3
// bank[29]
#define X29 29U
#define XT4 29U //Temporary 4
// bank[30]
#define X30 30U
#define XT5 30U //Temporary 5
// bank[31]
#define X31 31U
#define XT6 31U //Temporary 6
//--------------------------------------------
//Defined Instruction Argument Fields
//--------------------------------------------
#define SHLMT_rd       7U // value pos :  7 | rd
#define SHLMT_rs1     15U // value pos : 15 | rs1
#define SHLMT_rs2     20U // value pos : 20 | rs2
#define SHLMT_Iim11_0 20U // value pos : 20 | I-immediate[11:0]  {20- 0= 20}  |Note: also works for JALR
#define SHLMT_Sim4_0   7U // value pos :  7 | S-immediate[4:0]   { 7- 0=  7}
#define SHLMT_Sim11_5 20U // value pos : 25 | S-immediate[11:5]  {25- 5= 20}
#define SHLMT_Bim12   19U // value pos : 31 | B-immediate[12]    {31-12= 19}
#define SHLMT_Bim10_5 20U // value pos : 25 | B-immediate[10:5]  {25- 5= 20}
#define SHLMT_Bim4_1   7U // value pos :  8 | B-immediate[4:1]   { 8- 1=  7}
#define SHRMT_Bim11    4U // value pos :  7 | B-immediate[11]    { 7-11=- 4}
#define SHLMT_Uim31_12 0U // value pos : 12 | U-immediate[31:12] {12-12=  0}
#define SHLMT_Jim20   11U // value pos : 31 | J-immediate[20]    {31-20= 11}    
#define SHLMT_Jim10_1 20U // value pos : 21 | J-immediate[10:1]  {21- 1= 20}
#define SHLMT_Jim11    9U // value pos : 20 | J-immediate[11]    {20-11=  9}
#define SHLMT_Jim19_12 0U // value pos : 12 | J-immediate[19:12] {12-12=  0}
//--------------------------------------------
//Defined Decoding Opcodes
//--------------------------------------------
#define OPCODE_R 0x33
#define OPCODE_I 0x13
#define OPCODE_L 0x03
#define OPCODE_S 0x23
#define OPCODE_B 0x63
#define OPCODE_LUI 0x37
#define OPCODE_AUIPC 0x17
#define OPCODE_JAL 0x6F
#define OPCODE_JALR 0x67
#define OPCODE_HALT 0x00000000 //NON-RISCV CUSTOM HALT
//--------------------------------------------
//Defined Decoding functcodes
//--------------------------------------------
//code 000
#define F3_ADD  0b000
#define F3_SUB  0b000
#define F3_LB   0b000
#define F3_SB   0b000
#define F3_BEQ  0b000
//code 001
#define F3_SLL  0b001
#define F3_LH   0b001
#define F3_SH   0b001
#define F3_BNE  0b001
//code 010
#define F3_SLT  0b010
#define F3_LW   0b010
#define F3_SW   0b010
//code 011
#define F3_SLTU 0b011
//code 100
#define F3_XOR  0b100
#define F3_LBU  0b100
#define F3_BLT  0b100
//code 101
#define F3_SRL  0b101
#define F3_SRA  0b101
#define F3_LHU  0b101
#define F3_BGE  0b101
//code 110
#define F3_OR   0b110
#define F3_BLTU 0b110
//code 111
#define F3_AND  0b111
#define F3_BGEU 0b111
//--------------------------------------------
//Defined Instruction Operation Fields
//--------------------------------------------
#define OP_HALT 0x00000000 //NON-RISCV CUSTOM HALT
//R-type                           fn7     rs2    rs1  fn3     rd       op
#define OP_add   0x00000033 //0000|000 X|XXXX| XXXX|X 000| XXXX|X 011|0011
#define OP_sub   0x40000033 //0100|000 X|XXXX| XXXX|X 000| XXXX|X 011|0011
#define OP_sll   0x00001033 //0000|000 X|XXXX| XXXX|X 001| XXXX|X 011|0011
#define OP_slt   0x00002033 //0000|000 X|XXXX| XXXX|X 010| XXXX|X 011|0011
#define OP_sltu  0x00003033 //0000|000 X|XXXX| XXXX|X 011| XXXX|X 011|0011
#define OP_xor   0x00004033 //0000|000 X|XXXX| XXXX|X 100| XXXX|X 011|0011
#define OP_srl   0x00005033 //0000|000 X|XXXX| XXXX|X 101| XXXX|X 011|0011
#define OP_sra   0x40005033 //0100|000 X|XXXX| XXXX|X 101| XXXX|X 011|0011
#define OP_or    0x00006033 //0100|000 X|XXXX| XXXX|X 110| XXXX|X 011|0011
#define OP_and   0x00007033 //0100|000 X|XXXX| XXXX|X 111| XXXX|X 011|0011
#define OP_fadd  0x00000053 //0000|000 X|XXXX| XXXX|X 000| XXXX|X 101|0011
#define OP_fsub  0x04000053 //0000|100 X|XXXX| XXXX|X 000| XXXX|X 101|0011
//I-type : Arithmatic Variant           [11:0]    rs1  fn3     rd       op
#define OP_addi  0x00000013 //XXXX|XXX X|XXXX| XXXX|X 000| XXXX|X 001|0011
#define OP_slti  0x00002013 //XXXX|XXX X|XXXX| XXXX|X 010| XXXX|X 001|0011
#define OP_sltiu 0x00003013 //XXXX|XXX X|XXXX| XXXX|X 010| XXXX|X 001|0011
#define OP_xori  0x00004013 //XXXX|XXX X|XXXX| XXXX|X 100| XXXX|X 001|0011
#define OP_ori   0x00006013 //XXXX|XXX X|XXXX| XXXX|X 110| XXXX|X 001|0011
#define OP_andi  0x00007013 //XXXX|XXX X|XXXX| XXXX|X 111| XXXX|X 001|0011
//I-type : Shift Variant           fn7   [4:0]    rs1  fn3     rd       op
#define OP_slli  0x00001013 //0000|000 X|XXXX| XXXX|X 001| XXXX|X 001|0011
#define OP_srli  0x00005013 //0000|000 X|XXXX| XXXX|X 101| XXXX|X 001|0011
#define OP_srai  0x40005013 //0100|000 X|XXXX| XXXX|X 101| XXXX|X 001|0011
//I-type : Load Variant                 [11:0]    rs1  fn3     rd       op
#define OP_lb    0x00000003 //XXXX|XXX X|XXXX| XXXX|X 000| XXXX|X 000|0011
#define OP_lh    0x00001003 //XXXX|XXX X|XXXX| XXXX|X 001| XXXX|X 000|0011
#define OP_lw    0x00002003 //XXXX|XXX X|XXXX| XXXX|X 010| XXXX|X 000|0011
#define OP_lbu   0x00004003 //XXXX|XXX X|XXXX| XXXX|X 100| XXXX|X 000|0011
#define OP_lhu   0x00005003 //XXXX|XXX X|XXXX| XXXX|X 101| XXXX|X 000|0011
#define OP_flw   0x00002007 //XXXX|XXX X|XXXX| XXXX|X 010| XXXX|X 000|0111
//S-type                         [11:5]    rs2    rs1  fn3   [4:0]      op
#define OP_sb    0x00000023 //XXXX|XXX X|XXXX| XXXX|X 000| XXXX|X 010|0011
#define OP_sh    0x00001023 //XXXX|XXX X|XXXX| XXXX|X 001| XXXX|X 010|0011
#define OP_sw    0x00002023 //XXXX|XXX X|XXXX| XXXX|X 010| XXXX|X 010|0011
#define OP_fsw   0x00002027 //XXXX|XXX X|XXXX| XXXX|X 010| XXXX|X 010|0111
//B-type                       12[10:5]    rs2    rs1 fn3 [4:1]11       op
#define OP_beq   0x00000063 //XXXX|XXX X|XXXX| XXXX|X 000| XXXX|X 110|0011
#define OP_bne   0x00001063 //XXXX|XXX X|XXXX| XXXX|X 001| XXXX|X 110|0011
#define OP_blt   0x00004063 //XXXX|XXX X|XXXX| XXXX|X 100| XXXX|X 110|0011
#define OP_bge   0x00005063 //XXXX|XXX X|XXXX| XXXX|X 101| XXXX|X 110|0011
#define OP_bltu  0x00006063 //XXXX|XXX X|XXXX| XXXX|X 110| XXXX|X 110|0011
#define OP_bgeu  0x00007063 //XXXX|XXX X|XXXX| XXXX|X 111| XXXX|X 110|0011
//U-type                                           [31:12]     rd       op
#define OP_lui   0x00000037 //XXXX|XXX X|XXXX| XXXX|X XXX| XXXX|X 011|0111
#define OP_auipc 0x00000017 //XXXX|XXX X|XXXX| XXXX|X XXX| XXXX|X 001|0111
//J-type : JAL                           20[10:1]11[19:12]     rd       op
#define OP_jal   0x0000006F //XXXX|XXX X|XXXX| XXXX|X XXX| XXXX|X 110|1111
//J-type : JALR                         [11:0]    rs1  fn3     rd       op
#define OP_jalr  0x00000067 //XXXX|XXX X|XXXX| XXXX|X 000| XXXX|X 110|0111

//End Header Guard
#endif