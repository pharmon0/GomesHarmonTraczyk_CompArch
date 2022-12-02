/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Arbitrated Memory Bus Module */

// Start Header Guard
#ifndef GHT_MEMBUS
#define GHT_MEMBUS

//==========================
// Headers and Namespaces
//==========================
#include <cstdint>
#include <string>
#include <vector>
#include "Definitions.h"
#include "Datatypes.h"
#include "Cache.h"
#include "Memory.h"
using std::vector;
using std::string;

//==========================
// Custom Definitions
//==========================

class Bus{

    //Physical Components
    vector<Cache*> members;
    Memory* main_memory;

    //Arbitration
    //The holder of the token
    // (cache in the vector for whom the
    //  token is the iterator), is the only
    //   member able to control the bus
    uint8_t token;

    //timing
    uint32_t access_ticks;
    uint32_t access_counter;

    //Metadata
    string bus_name;

  public:

    //Constructor
    Bus(string name, vector<Cache> caches, Memory* memory, uint32_t access_time);

    //Bus Access
    response_t bus_request(Cache* requester);
};

// End Header Guard
#endif