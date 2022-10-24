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
#include <RISCV.h>

//============================================
// Constants and Macros
//============================================

//============================================
// Memory Controller Class
//============================================
class Membus{
    Core   cpu;
    Memory ram;
  public:
    //constructor
    Membus(void);

    //Clock Cycle Process
    void process(void);
};

// End Header Guard
#endif