/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Register File Object Module */

// Start Header Guard
#ifndef GHT_REGISTER_FILE
#define GHT_REGISTER_FILE

//============================================
// Libraries
//============================================
#include <cstdint>    //Standard Int Types
#include "Datatypes.h"//Custom Datatypes Header
#include "RISCV.h"    //RISCV Definitions Header
#include <iostream>
using std::cout;
using std::endl;

//============================================
// Constants and Macros
//============================================

//============================================
// Register File Class
//============================================
class RegisterFile{
    //register bank memory allocation
    int32_t iBank[32];
    float   fBank[32];

 public:
    //Constructors
    RegisterFile(void);

    union{
        uint32_t all;
        struct{
            uint32_t selrs1 : 5; //ctrlPort : select source 1 register
            uint32_t selrs2 : 5; //ctrlPort : select source 2 register
            uint32_t selrd  : 5; //ctrlPort : select destination register
            uint32_t rfwen  : 1; //ctrlPort : write-enable flag
            uint32_t r1flop : 1; //ctrlPort : Select floating-point register for rs1
            uint32_t r2flop : 1; //ctrlPort : Select floating-point register for rs2
            uint32_t rdflop : 1; //ctrlPort : Select floating-point register for rd
            uint32_t:0;//union alignment
        };
    } rfctrl;      
    data32_t rs1,    //dataPort : Value-to-read from source 1
             rs2,    //dataPort : Value-to-read from source 2
             rd;     //dataPort : Value-to-write

    //Process RegisterFile Data Ports
    void processRead(void);
    void processWrite(void);
};

// End Header Guard
#endif