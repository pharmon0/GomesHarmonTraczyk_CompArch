/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Memory Bus Object Module */

// Start Header Guard
#ifndef GHT_MEMBUS
#define GHT_MEMBUS

//============================================
// Libraries
//============================================
#include "Memory.h"
#include "Core.h"
#include <cstdint>
#include "RISCV.h"
#include <vector>
using std::vector;

//============================================
// Constants and Macros
//============================================

//============================================
// Memory Controller Class
//============================================
class Membus{
    vector<memport_t*> ports;
    memport_t* mem;
    bool toMem;

  public:
    //constructor
    Membus(vector<memport_t*>,memport_t*);

    //Clock Cycle Process
    void process(uint64_t);
};

// End Header Guard
#endif