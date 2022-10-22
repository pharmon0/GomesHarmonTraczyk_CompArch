/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * CPU Core Object Module */

// Start Header Guard
#ifndef GHT_ALU
#define GHT_ALU

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
    ArithmaticLogicUnit alu;
    uint32_t pc;
    //CPU Control
    union{
        uint8_t all;
        struct{
            uint8_t stallF : 1;
            uint8_t stallD : 1;
            uint8_t stallE : 1;
            uint8_t stallW : 1;
            uint8_t:0; //union alignment
        }
    } ctrl;

 public:
    //I-port
    struct{
        uint32_t address;
        uint32_t data;
        union{
            uint8_t all;
            struct{
                uint8_t membusy : 1; //set to request memory access
                                     //wait for value to fall to 0
                uint8_t:0; //union alignment
            }
        } ctrl;
    } portI;

    //D-port
    struct{
        uint32_t address;
        uint32_t data;
        uint32_t address;
        uint32_t data;
        union{
            uint8_t all;
            struct{
                uint8_t membusy : 1; //set to request memory access
                                     //wait for value to fall to 0
                uint8_t:0; //union alignment
            }
        } ctrl;
    } portD;
    //constructor
    Core(void);

    //Clock Cycle Process
    bool process(void);
};

// End Header Guard
#endif