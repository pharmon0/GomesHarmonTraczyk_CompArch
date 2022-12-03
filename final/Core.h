/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Pseudo-Core Module */

// Start Header Guard
#ifndef GHT_CORE
#define GHT_CORE

//==========================
// Headers and Namespaces
//==========================
#include <iostream>
#include <cstdint>
#include <string>
#include "Definitions.h"
#include "Cache.h"
using std::string;
using std::cout;
using std::endl;

//==========================
// Custom Definitions
//==========================

class Core{

  Cache* cache;

  string core_name;
  uint8_t core_id;

  public:

    //Constructor
    Core(string name, uint8_t id, Cache* mem);

    //Tick Process
    bool process(uint32_t tick);

};

// End Header Guard
#endif