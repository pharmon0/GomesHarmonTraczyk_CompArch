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
#include "Datatypes.h"  //Custom Datatypes
#include <iostream>
using std::cout;
using std::endl;

//============================================
// Constants and Definitions
//============================================

//============================================
// ALU Object Class
//============================================
class ALU{
    uint32_t intCycles;
    uint32_t flopCycles;
    uint16_t counter;
 public:
    data32_t A, B, X; //ALU dataports
    union{
        uint16_t all;
        struct{
            uint16_t   aluop : 4; //ALUOP Control Lines
            uint16_t alucomp : 2; //ALU Compare Select  00:LT 01:EQ 10:NE 11:GE
            uint16_t   alufp : 1; //ALU is performing a flop
            uint16_t    busy : 1; //ALU is busy doing flops
            uint16_t:0;//union alignment
        };
    } aluctrl;

    //constructor
    ALU(void);
    ALU(uint8_t, uint8_t);

    //run ALU operation
    void process(void);
    void operate(void);
};

// End Header Guard
#endif