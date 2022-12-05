/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Memory Module */

// Start Header Guard
#ifndef GHT_MEMORY
#define GHT_MEMORY

//==========================
// Headers and Namespaces
//==========================
#include <vector>
#include <map>
#include <cstdint>
#include <string>
#include <iostream>
#include <bitset>
#include "Definitions.h"
#include "Datatypes.h"
#include "MemoryBlock.h"
#include "Bus.h"
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::bitset;
using std::map;

class Memory{

    //Physical Components
    map<uint32_t, Block> bank;
    Bus* bus;

    //Physical Properties
    uint32_t address_mask;

    //Timing
    uint32_t access_ticks;
    uint32_t access_counter;

    //Metadata
    string memory_name;
    uint32_t bytes_in_memory;
    uint32_t bytes_in_block;
    uint32_t blocks_in_memory;

  public:

    //Constructor
    Memory(string name, uint32_t memory_bytes, uint32_t block_bytes, uint32_t access_time);

    //Bank Access
    response_t memory_read(uint32_t address);
    response_t memory_write(uint32_t address, Block data);

    //interconnection of components
    void attach_bus(Bus* memory_bus);
};

// End Header Guard
#endif