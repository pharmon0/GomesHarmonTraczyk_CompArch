/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * Program Instruction Memory Header */

// Start Header Guard
#ifndef HARMON_PROGRAM
#define HARMON_PROGRAM

//============================================
// Libraries
//============================================
#include "RISCV.h"   //RISCV Definitions Header
#include <cstdint>   //Standard Int Types

//============================================
// Constants and Macros
//============================================

//============================================
// Instruction Memory Global Array Declaration
// - Write Instructions in Program.cpp
// ! ! ! DO NOT WRITE INSTRUCTIONS HERE ! ! ! 
//============================================
extern uint32_t InstructionMemory[];

//============================================
// Helper Functions
//============================================
uint32_t  vRs1(uint32_t);
uint32_t  vRs2(uint32_t);
uint32_t   vRd(uint32_t);
uint32_t vImmI(uint32_t);
uint32_t vImmS(uint32_t);
uint32_t vImmB(uint32_t);
uint32_t vImmU(uint32_t);
uint32_t vImmJ(uint32_t);

// End Header Guard
#endif