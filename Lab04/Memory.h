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
#include <vector>
#include <string>
#include <fstream>
#include <bitset>
#include <iostream>
#include "Functions.h"
#include "Datatypes.h"
using std::stoi;
using std::cout;
using std::endl;
using std::fstream;
using std::string;
using std::vector;
using std::map;
using std::bitset;
using std::ifstream;
using std::ofstream;

//============================================
// Constants and Macros
//============================================

//============================================
// Memory Controller Class
//============================================
class Memory{
    map<uint32_t, uint8_t> bank; //Memory Bank Hashtable
    
    uint32_t memRead(uint32_t,uint8_t); //helper for memread
    void memWrite(uint32_t,uint32_t,uint8_t); //helper for memwrite

    uint8_t counterI;
    uint8_t counterD;

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

    //Constructors
    Memory(void);

    //process functions
    void processPortI(void); //32bit read only
    void processPortD(void); //read/write 8,16,32-bit data

    //init
    void populateFloat(string,uint32_t);
    void populateInt(string,uint32_t);
    void printToFile(string);
};

// End Header Guard
#endif