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
#include <vector>
#include "Definitions.h"
#include <iostream>
using std::cout;
using std::endl;
using std::vector;

class Block{
    
    //Physical Components
    vector<uint8_t> bank;

    //Control Flags
    uint32_t block_tag;
    bool least_recent;
    bool cold_start;
    char mesi_flag;

    //metadata
    uint32_t block_size;

  public:

    //Constructor
    Block();
    Block(uint32_t size);

    //control flag accessors
    void set_tag(uint32_t tag);
    uint32_t get_tag(void) const;
    void start_cold(void);
    void set_mesi(char mesi);
    char get_mesi(void) const;
    void set_lru(bool lru);
    bool get_lru(void) const;
    bool is_cold(void) const;

    //Bank Access
    //untimed, uncontrolled accessors
    uint8_t read_byte(uint32_t offset);
    void write_byte(uint32_t offset, uint8_t data);

};

// End Header Guard
#endif