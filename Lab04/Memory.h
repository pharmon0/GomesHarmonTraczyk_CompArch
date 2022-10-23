/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Memory Object Module */

// Start Header Guard
#ifndef GHT_MEMORY
#define GHT_MEMORY

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
    
    read(uint32_t,uint8_t); //helper for memread
    write(uint32_t,uint32_t,uint8_t); //helper for memwrite

 public:
    //I-port
    struct{
        uint32_t address;
        uint32_t data;
        union{
            uint8_t all;
            struct{
                // I-Port Conceptual Operation
                // Memory::request and Core::membusy both are the set output of
                //  an RS Latch. Memory::ack is the reset line and
                //   Core::request is the set line.
                // Memory::ack and Core::request auto reset on clock
                uint8_t request : 1; //input: memory access requested
                uint8_t     ack : 1; //output: data ready
                uint8_t counter : 1; //internal: keeps track of time
                uint8_t:0; //union alignment
            }
        } ctrl;
    } portI;

    //D-port
    struct{
        uint32_t address;
        uint32_t data;
        union{
            uint8_t all;
            struct{
                uint8_t  memrsz : 2; //input: select memory read size   | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t  memwsz : 2; //input: select memory write size  | 00:disable, 01:Byte, 10:Half, 11:Word
                uint8_t     ack : 1; //output: data ready
                uint8_t counter : 1; //internal: keeps track of time
                uint8_t:0; //union alignment
            }
        } ctrl;
    } portD;

    //Constructors
    Memory(void);

    //process functions
    void processPortI(void); //32bit read only
    void processPortD(void); //read/write 8,16,32-bit data
};

// End Header Guard
#endif