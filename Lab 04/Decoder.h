/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Decode Stage Module */

// Start Header Guard
#ifndef GHT_DECODER
#define GHT_DECODER

//============================================
// Libraries
//============================================
#include <cstdint>      //Standard Int Types
#include <iostream>     //basic UI control
#include <string>       //string datatype
#include <bitset>       //print binary
#include "RISCV.h"      //RISCV Definitions Header
#include "Instruction.h"//Instruction Datatype
#include "BinOps.h"     //Binary Helper Functions
#include "SignExtend.h" //Sign Extension
using std::cout;
using std::string;
using std::endl;
using std::bitset;

//============================================
// Decode Stage Class
//============================================
class Decoder{
 public:
    union{    //Immediate Control Signals
        uint32_t all;
        struct{
            uint32_t   value :20; //Immediate Value   |
            uint32_t immsize : 1; //Size of Immediate | 0:12-bit , 1:20-bit  
            uint32_t immshft : 1; //Shift Amount concat with immsize ::[size,shft]:: 00:0 , 01||10:1 , 11:12
            uint32_t:0;//union alignment
        };
    } immctrl;//Immediate Control Signals

    union{    //Register File Control Signals
        uint16_t all;
        struct{
            uint16_t  selrs1 : 5; //select RS1        |
            uint16_t  selrs2 : 5; //select RS2        |
            uint16_t   selrd : 5; //select RD         |  
            uint16_t   rfwen : 1; //enable write to RD| 0:disable, 1:enable
            uint16_t:0;//union alignment
        };
    } rfctrl; //Register File Control Signals

    union{    //ALU Control Signals
        uint8_t all;
        struct{
            uint8_t   aluop : 4; //select ALU Operation      |
            uint8_t alucomp : 2; //select ALU Comparison Type|
            uint8_t aluasel : 1; //select ALU A Source       | 0:RS1, 1:PC
            uint8_t alubsel : 1; //select ALU B Source       | 0:RS2, 1:Immediate
            uint8_t:0;//union alignment
        };
    } aluctrl;//ALU Control Signals

    union{    //Memory Control Signals
        uint8_t all;
        struct{
            uint8_t  memrsz : 2; //select memory read size   | 00:disable, 01:Byte, 10:Half, 11:Word
            uint8_t  memwsz : 2; //select memory write size  | 00:disable, 01:Byte, 10:Half, 11:Word
            uint8_t memrsgn : 1; //enable sign extend of load| 0:disable, 1:enable based on memrsz
            uint8_t:0;//union alignment
        };
    } memctrl;//Memory Control Signals

    union{    //Miscellaneous Control Signals
        uint8_t all;
        struct{
            uint8_t  halt : 1; //halt CPU clock loop    | 0:continue , 1:HALT
            uint8_t btype : 1; //Branch on ALU Condition| 0:no branch, 1:Check ALU
            uint8_t jtype : 1; //Send ALU result to PC and store PC+4 in RD
            uint8_t:0;//union alignment
        };
    } ctrl;   //Miscellaneous Control Signals

    //constructor
    Decoder(void);

    //Process Decoder DataPorts
    void process(uint32_t);

    //debug printer
    void printCtrl(void);
};

// End Header Guard
#endif