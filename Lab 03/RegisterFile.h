/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * Register File Object Module */

// Start Header Guard
#ifndef HARMON_REGISTER_FILE
#define HARMON_REGISTER_FILE

//============================================
// Libraries
//============================================
#include <cstdint>   //Standard Int Types
#include "RISCV.h"   //RISCV Definitions Header

//============================================
// Constants and Macros
//============================================

//============================================
// Register File Class
//============================================
class RegisterFile{
    //register bank memory allocation
    int32_t bank[32];

 public:
    //Constructors
    RegisterFile(void);

    union{
        uint16_t all;
        struct{
            uint16_t selrs1 : 5; //ctrlPort : select source 1 register
            uint16_t selrs2 : 5; //ctrlPort : select source 2 register
            uint16_t selrd  : 5; //ctrlPort : select destination register
            uint16_t rfwen  : 1; //ctrlPort : write-enable flag
            uint16_t:0;//union alignment
        };
    } ctrl;      
    int32_t rs1,    //dataPort : Value-to-read from source 1
            rs2;    //dataPort : Value-to-read from source 2

    //Process RegisterFile Data Ports
    void processRead(void);
    void processWrite(int32_t);
};

// End Header Guard
#endif