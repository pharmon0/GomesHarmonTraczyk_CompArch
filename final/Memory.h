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
#include <cstdint>
#include <string>
#include "Definitions.h"
#include "MemoryBlock.h"
using std::string;
using std::vector;

class Memory{

    //Physical Components
    map<uint32_t, Block> bank;

    //Timing
    uint32_t access_ticks;
    uint32_t access_counter;

    //Metadata
    string memory_name;
    uint32_t bytes_in_memory;

  public:

    //Constructor
    Memory(string name, uint32_t bytes, uint32_t access_time); //TODO Memory::Memory()

    //Bank Access
    Block memory_read(uint32_t address); //TODO memory_read()
    void memory_write(uint32_t address, Block data); //TODO memory_write()
};

// End Header Guard
#endif