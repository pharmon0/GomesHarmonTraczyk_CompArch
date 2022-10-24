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

 public:
    //I-port
    struct{
        uint32_t address;
        uint32_t data;
        union{
            uint8_t all;
            struct{
                uint8_t  memrsz : 2; //select memory read size   | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t  memwsz : 2; //select memory write size  | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t request : 1; //has a memory request been made?
                uint8_t  memack : 1; //is memory operation complete?
                uint8_t:0; //union alignment
            };
        } memctrl;
    } portI;

    //D-port
    struct{
        uint32_t address;
        uint32_t data;
        union{
            uint8_t all;
            struct{
                uint8_t  memrsz : 2; //select memory read size   | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t  memwsz : 2; //select memory write size  | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t request : 1; //has a memory request been made?
                uint8_t  memack : 1; //is memory operation complete?
                uint8_t:0; //union alignment
            };
        } memctrl;
    } portD;

    //constructor
    Core(void);

    //Clock Cycle Process
    bool process(void);
};

// End Header Guard
#endif