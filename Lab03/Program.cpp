/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * Program Instruction Memory Code */
#include "Program.h"

//============================================
// Instruction Memory Global Array Definition
// - Write Instructions Here!
//============================================
uint32_t InstructionMemory[] =
{
    //R-type
    OP_add   |  vRd(X5) | vRs1(X0) | vRs2(X1),
    OP_sub   |  vRd(X5) | vRs1(X0) | vRs2(X1),
    OP_sll   |  vRd(X5) | vRs1(X0) | vRs2(X1),
    OP_slt   |  vRd(X5) | vRs1(X0) | vRs2(X1),
    OP_sltu  |  vRd(X5) | vRs1(X0) | vRs2(X1),
    OP_xor   |  vRd(X5) | vRs1(X0) | vRs2(X1),
    OP_srl   |  vRd(X5) | vRs1(X0) | vRs2(X1),
    OP_sra   |  vRd(X5) | vRs1(X0) | vRs2(X1),
    OP_or    |  vRd(X5) | vRs1(X0) | vRs2(X1),
    OP_and   |  vRd(X5) | vRs1(X0) | vRs2(X1),
    //I-type:A/L
    OP_addi  |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    OP_slti  |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    OP_sltiu |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    OP_xori  |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    OP_ori   |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    OP_andi  |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    //I-type:Shift
    OP_slli  |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    OP_srli  |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    OP_srai  |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    //I-type:Load
    OP_lb    |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    OP_lh    |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    OP_lw    |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    OP_lbu   |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    OP_lhu   |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    //S-type
    OP_sb    | vRs1(X0) | vRs2(X1) | vImmS(0xA),
    OP_sh    | vRs1(X0) | vRs2(X1) | vImmS(0xA),
    OP_sw    | vRs1(X0) | vRs2(X1) | vImmS(0xA),
    //B-type
    OP_beq   | vRs1(X0) | vRs2(X1) | vImmB(0xA),
    OP_bne   | vRs1(X0) | vRs2(X1) | vImmB(0xA),
    OP_blt   | vRs1(X0) | vRs2(X1) | vImmB(0xA),
    OP_bge   | vRs1(X0) | vRs2(X1) | vImmB(0xA),
    OP_bltu  | vRs1(X0) | vRs2(X1) | vImmB(0xA),
    OP_bgeu  | vRs1(X0) | vRs2(X1) | vImmB(0xA),
    //U-type
    OP_lui   |  vRd(X5) | vImmU(0xA000),
    OP_auipc |  vRd(X5) | vImmU(0xA000),
    //J-type:JAL
    OP_jal   |  vRd(X5) | vImmJ(0xA),
    //J-type:JALR
    OP_jalr  |  vRd(X5) | vRs1(X0) | vImmI(0xA),
    //Custom Halt Instruction
    OP_HALT
};
//============================================
// End of Instruction Memory
//============================================

//============================================
// Helper Functions
//============================================
uint32_t vImmI(uint32_t imm){
    //imm is [11:0]
    return (imm & 0x00000FFF) << SHLMT_Iim11_0;
}
uint32_t vImmS(uint32_t imm){
    //imm is [11:0]
    uint32_t res = 0x00000000;
    res |= (imm & 0x00000FE0) << SHLMT_Sim11_5;
    res |= (imm & 0x0000001F) << SHLMT_Sim4_0;
    return res;
}
uint32_t vImmB(uint32_t imm){
    //imm is [12:1]
    uint32_t res = 0x00000000;
    res |= (imm & 0x00001000) << SHLMT_Bim12;
    res |= (imm & 0x00000800) >> SHRMT_Bim11;
    res |= (imm & 0x000007E0) << SHLMT_Bim10_5;
    res |= (imm & 0x0000001E) << SHLMT_Bim4_1;
    return res;
}
uint32_t vImmU(uint32_t imm){
    //imm is [31:12]
    return imm & 0xFFFFF000;
    //No Shift Required
}
uint32_t vImmJ(uint32_t imm){
    //imm is [20:1]
    uint32_t res = 0x00000000;
    res |= (imm & 0x00100000) << SHLMT_Jim20;
    res |= (imm & 0x000FF000) << SHLMT_Jim19_12;
    res |= (imm & 0x00000800) << SHLMT_Jim11;
    res |= (imm & 0x000007FE) << SHLMT_Jim10_1;
    return res;
}
uint32_t vRs1(uint32_t val){return ((val&0x1F) << SHLMT_rs1);}
uint32_t vRs2(uint32_t val){return ((val&0x1F) << SHLMT_rs2);}
uint32_t  vRd(uint32_t val){return ((val&0x1F) << SHLMT_rd) ;}