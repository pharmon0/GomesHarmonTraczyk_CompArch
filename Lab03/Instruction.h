/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * Instruction Datatype Module */

// Start Header Guard
#ifndef HARMON_INSTRUCTION
#define HARMON_INSTRUCTION

//============================================
// Libraries
//============================================
#include <cstdint>   //Standard Int Types

//============================================
// Instruction Datatype
// - Unifies a 32-bit number with RISC-V instruction fields
//============================================
typedef union{
    uint32_t value;
    struct{
        uint32_t opcode : 7;
        uint32_t data : 25;
    }base;
    struct{
        uint32_t opcode : 7;
        uint32_t     rd : 5;
        uint32_t funct3 : 3;
        uint32_t    rs1 : 5;
        uint32_t    rs2 : 5;
        uint32_t funct7 : 7;
        uint32_t:0;//alignment
    }R;
    struct{
        uint32_t opcode : 7;
        uint32_t     rd : 5;
        uint32_t funct3 : 3;
        uint32_t    rs1 : 5;
        uint32_t  im4_0 : 5;
        uint32_t im11_5 : 7;
        uint32_t:0;//alignment
    }I;
    struct{
        uint32_t opcode : 7;
        uint32_t  im4_0 : 5;
        uint32_t funct3 : 3;
        uint32_t    rs1 : 5;
        uint32_t    rs2 : 5;
        uint32_t im11_5 : 7;
        uint32_t:0;//alignment
    }S;
    struct{
        uint32_t opcode : 7;
        uint32_t   im11 : 1;
        uint32_t  im4_1 : 4;
        uint32_t funct3 : 3;
        uint32_t    rs1 : 5;
        uint32_t    rs2 : 5;
        uint32_t im10_5 : 6;
        uint32_t   im12 : 1;
        uint32_t:0;//alignment
    }B;
    struct{
        uint32_t  opcode : 7;
        uint32_t      rd : 5;
        uint32_t im31_12 : 20;
        uint32_t:0;//alignment
    }U;
    struct{
        uint32_t  opcode : 7;
        uint32_t      rd : 5;
        uint32_t im19_12 : 8;
        uint32_t    im11 : 1;
        uint32_t  im10_1 : 10;
        uint32_t    im20 : 1;
        uint32_t:0;//alignment
    }J;
} instruction_t;

//End Header Guard
#endif