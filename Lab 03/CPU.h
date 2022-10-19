/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * CPU Object Module */

// Start Header Guard
#ifndef HARMON_CPU
#define HARMON_CPU

//============================================
// Libraries
//============================================
#include <cstdint>           //Standard Int Types
#include <iostream>          //Basic UI Control
#include <iomanip>           //Better UI Control
#include <bitset>            //Print Binary
#include "RISCV.h"           //RISCV Definitions Header
#include "Program.h"         //Instruction Memory Header
#include "Instruction.h"     //Instruction Datatype
#include "RegisterFile.h"    //Register File Object Class
#include "Decoder.h"         //Decoder Object Class
#include "ALU.h"             //ALU Object Class
#include "MemoryController.h"//Memory Controller Object Class
#include "SignExtend.h"      //Sign Extension Class
#include "BinOps.h"          //Binary Helper Functions
using std::cout;
using std::setw;
using std::setfill;
using std::endl;
using std::bitset;

//============================================
// Constants and Macros
//============================================
//define this macro to only allow pc=pc+4. no branching
#define DECODER_DEBUG //comment this line to allow CPU to behave as an actual CPU

//============================================
// CPU Object Class
//============================================
class CPU{
    //CPU Components
    RegisterFile regfile;   //CPU Register File Object
    Decoder decoder;        //CPU Decoder Object
    ArithmaticLogicUnit alu;//CPU ALU Object
    MemoryController mem;   //CPU Memory Object
    uint32_t pc;            //CPU Program Counter
    uint32_t clock;         //CPU Clock Cycle Counter

 public:
    //Constructors
    CPU(void);

    //Function runs main program loop
    void run(void);
};

// End Header Guard
#endif