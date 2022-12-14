/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * Decode Stage Module */

// Start Header Guard
#ifndef HARMON_ALU
#define HARMON_ALU

//============================================
// Libraries
//============================================
#include <cstdint>      //Standard Int Types
#include "RISCV.h"      //RISCV Definitions Header

//============================================
// ALU Object Class
//============================================
class ArithmaticLogicUnit{
 public:
    int32_t A, B, X; //ALU dataports
    union{
        uint8_t all;
        struct{
            uint8_t aluop : 4; //ALUOP Control Lines
            uint8_t  comp : 2; //ALU Compare Select  00:LT 01:EQ 10:NE 11:GE
            uint8_t:0;//union alignment
        };
    } ctrl;

    //constructor
    ArithmaticLogicUnit(void);

    //run ALU operation
    void process(void);
};

// End Header Guard
#endif