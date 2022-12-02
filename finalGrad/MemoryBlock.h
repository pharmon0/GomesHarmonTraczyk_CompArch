/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Memory Block Module */

// Start Header Guard
#ifndef GHT_MEMORYBLOCK
#define GHT_MEMORYBLOCK

//==========================
// Headers and Namespaces
//==========================
#include <cstdint>
#include <map>
#include <vector>
#include "Settings.h"
using std::map;
using std::vector;

class Block{
    
    //Physical Components
    map<uint32_t, uint8_t> bank;

    //Control Flags
    uint32_t block_tag;
    bool least_recent;
    bool cold_start;

  public:

    //Constructor
    Block(uint32_t tag);

    //Bank Access
    uint8_t read_byte(uint32_t offset);
    void write_byte(uint32_t offset, uint8_t data);

};

// End Header Guard
#endif