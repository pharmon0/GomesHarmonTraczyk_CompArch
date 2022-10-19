/* ECGR 5181 - Computer Architecture
 * Phillip Harmon
 * Memory Object Module */

// Start Header Guard
#ifndef HARMON_MEMORY
#define HARMON_MEMORY

//============================================
// Libraries
//============================================
#include <cstdint>   //Standard Int Types
#include <map>       //Standard Ordered Hash Map
#include "RISCV.h"   //RISCV Definitions Header
using std::map;

//============================================
// Constants and Macros
//============================================

//============================================
// Memory Controller Class
//============================================
class Memory{
    map<uint32_t, uint8_t> bank; //Memory Bank Hashtable

 public:
    uint32_t address; //address input dataport
    int32_t  data;  //data i/o dataport
    union{
        uint8_t all;
        struct{
            uint8_t  memrsz : 2; //select memory read size   | 00:disable, 01:Byte, 10:Half, 11:Word
            uint8_t  memwsz : 2; //select memory write size  | 00:disable, 01:Byte, 10:Half, 11:Word
            uint8_t:0;//union alignment
        };
    } ctrl;

    //Constructors
    Memory(void);

    //process functions
    void process(void);
};

// End Header Guard
#endif