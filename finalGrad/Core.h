/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Pseudo-Core Module */

// Start Header Guard
#ifndef GHT_CORE
#define GHT_CORE

//==========================
// Headers and Namespaces
//==========================
#include <cstdint>
#include <string>
#include "Definitions.h"
using std::string;

//==========================
// Custom Definitions
//==========================

class Core{

  public:

    //Constructor
    Core();

    //Tick Process
    bool process(uint32_t tick);

};

// End Header Guard
#endif