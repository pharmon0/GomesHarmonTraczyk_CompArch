/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * CPU Core Object Module */

// Start Header Guard
#ifndef GHT_CPUCORE
#define GHT_CPUCORE

//============================================
// Libraries
//============================================
#include <cstdint>      //Standard Int Types
#include "RISCV.h"      //RISCV Definitions Header
#include <iostream>
#include <iomanip>
#include <bitset>
#include <string>
#include "Datatypes.h"  //Custom Datatypes Header
#include "Functions.h"
#include "Decoder.h"
#include "ALU.h"
#include "PipelineRegisters.h"
#include "RegisterFile.h"

using std::string;
using std::cout;
using std::bitset;
using std::endl;
using std::setw;
using std::setfill;

//============================================
// Constants and Definitions
//============================================
#ifndef TICKS_PER_CLOCK
#define TICKS_PER_CLOCK 10
#endif

//============================================
// CPU Core Object Class
//============================================
class Core{
    //CPU Components
    Decoder dec;
    RegisterFile rf;
    FDRegister fdr;
    DERegister der;
    EWRegister ewr;
    ALU alu;
    uint32_t pc;    //CPU program counter
    //CPU Control
    union{
        uint8_t all;
        struct{
            uint8_t   stallF : 1;
            uint8_t   stallD : 1;
            uint8_t   stallE : 1;
            uint8_t   stallW : 1;
            uint8_t branched : 1;
            uint8_t:0; //union alignment
        };
    } ctrl;
    //Core ID for differentiating CPU Cores
    string coreID;

 public:
    //I-port
    memport_t portI;

    //D-port
    memport_t portD;
    
    //instruction counter for CPI calculation
    uint32_t instructionCount;

    //constructor
    Core(string, uint8_t, uint8_t);

    //Clock Cycle Process
    bool process(uint64_t);
};

// End Header Guard
#endif