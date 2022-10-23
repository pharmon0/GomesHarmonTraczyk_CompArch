/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * ALU Object Module */

// Start Header Guard
#ifndef GHT_ALU
#define GHT_ALU

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
    data32_t A, B, X; //ALU dataports
    union{
        uint16_t all;
        struct{
            uint16_t aluop : 4; //ALUOP Control Lines
            uint16_t  comp : 2; //ALU Compare Select  00:LT 01:EQ 10:NE 11:GE
            uint16_t  flop : 1; //ALU is performing a flop
            uint16_t  busy : 1; //ALU is busy doing
            uint16_t count : 3; //ALU timer for flops
            uint16_t:0;//union alignment
        };
    } ctrl;

    //constructor
    ArithmaticLogicUnit(void);

    //run ALU operation
    void process(void);
};

// End Header Guard
#endif