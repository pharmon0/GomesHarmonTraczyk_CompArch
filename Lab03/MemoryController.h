/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * Memory Controller Object Module */

// Start Header Guard
#ifndef HARMON_MEMORY
#define HARMON_MEMORY

//============================================
// Libraries
//============================================
#include <cstdint>   //Standard Int Types
#include "RISCV.h"   //RISCV Definitions Header

//============================================
// Constants and Macros
//============================================

//============================================
// Memory Controller Class
//============================================
class MemoryController{
    int32_t memory; //placeholder for future use
                    //currently serves as a single-word memory location
 public:
    uint32_t address; //address input dataport
    int32_t  loaded;  //loaded data output dataport
    union{
        uint8_t all;
        struct{
            uint8_t  memrsz : 2; //select memory read size   | 00:disable, 01:Byte, 10:Half, 11:Word
            uint8_t  memwsz : 2; //select memory write size  | 00:disable, 01:Byte, 10:Half, 11:Word
            uint8_t:0;//union alignment
        };
    } ctrl;

    //Constructors
    MemoryController(void);

    //process functions
    void processLoad(void);
    void processStore(int32_t);
};

// End Header Guard
#endif