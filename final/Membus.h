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
#include "CacheBlock.h"
#include "Cache.h"
using std::vector;

//============================================
// Constants and Macros
//============================================
#ifndef TICKS_PER_CLOCK
#define TICKS_PER_CLOCK 10
#endif

//============================================
// Memory Controller Class
//============================================
class Membus{
    vector<blockport_t*> ports;
    bool messageSent;
    bool transferDirection;
    bool dataReady;
    blockport_t* mem;
    uint8_t token;
  public:
    //constructor
    Membus(vector<blockport_t*>,blockport_t*);

    //Clock Cycle Process
    void process(uint64_t);
};

// End Header Guard
#endif