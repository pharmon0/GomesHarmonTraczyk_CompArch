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
#include <iostream>
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
    uint8_t accessTicks; //number of ticks required for memory access

    map<uint32_t, uint8_t> bank; //Memory Bank Hashtable
    
    uint32_t memRead(uint32_t,uint8_t); //helper for memread
    void memWrite(uint32_t,uint32_t,uint8_t); //helper for memwrite

    int counterIA;
    int counterIB;
    int counterD;

 public:
    //Memory Ports
    memport_t portIA;
    memport_t portIB;
    memport_t portD;

    //Constructors
    Memory(uint8_t);

    //process functions
    void process(uint64_t); //Process Memory Ports

    //misc. Functions
    void populateFloat(string,uint32_t);
    void populate(string,uint32_t);
    //void printToFile(string);
    void printBytes(string);
    void printFloats(string);
};

// End Header Guard
#endif