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
#include <vector>
#include <string>
#include "Definitions.h"
#include "Cache.h"
using std::string;
using std::cout;
using std::endl;
using std::vector;

//==========================
// Custom Definitions
//==========================

class Core{

  Cache* cache;
  vector<int> addresses;
  vector<bool> writes;

  string core_name;

  uint32_t tick_complete;

  uint32_t iterator;

  public:

    //Constructor
    Core(string name, Cache* mem, vector<int> addr, vector<bool> rw);

    //Tick Process
    bool process(uint32_t tick);

};

// End Header Guard
#endif