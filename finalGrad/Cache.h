/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Cache Module */

// Start Header Guard
#ifndef GHT_CACHE
#define GHT_CACHE

//==========================
// Headers and Namespaces
//==========================
#include <vector>
#include <cstdint>
#include <string>
#include <math.h>
#include "Definitions.h"
#include "MemoryBlock.h"
using std::log2;
using std::ceil;
using std::string;
using std::vector;

//==========================
// Custom Definitions
//==========================
//Associativity Types
#define ASSOCIATIVITY_FULL 0
#define ASSOCIATIVITY_DIRECT 1
#define ASSOCIATIVITY_2_WAY 2
#define ASSOCIATIVITY_4_WAY 4

class Cache{

    //Physical Components
    //Bank is arranged as follows:
    //----------------------------------------------
    // index ||                entry
    //----------------------------------------------
    // set 0 || block(0,0) | block(0,1) | block(0,2)
    // set 1 || block(1,0) | block(1,1) | block(1,2)
    // set 2 || block(2,0) | block(2,1) | block(2,2)
    // set 3 || block(3,0) | block(3,1) | block(3,2)
    //----------------------------------------------
    vector<vector<Block> > bank;

    //Physical Properties
    uint32_t tag_width;
    uint32_t index_width;
    uint32_t offset_width;

    uint32_t bytes_in_block;
    uint32_t blocks_in_set;
    uint32_t bytes_in_cache;
    uint32_t blocks_in_cache;
    uint32_t sets_in_cache;
    
    //Timing
    uint32_t access_counter;
    uint32_t access_ticks;

    //Metadata
    string cache_name;

  public:

    //Constructor
    Cache(string name, uint32_t cache_bytes, uint32_t block_bytes, uint8_t associativity);
    
    //Cache Setup Functions
    void attach_cpu();//TODO attach_cpu()
    void attach_bus();//TODO attach_bus()

    //Mutators for Cache Properties
    void set_access_time(uint32_t ticks);

    //Bank Access
    response_t cache_access(uint32_t address, uint32_t data, bool write, uint8_t data_width);

    //helper functions
    uint32_t make_tag(uint32_t address);
    uint32_t make_index(uint32_t address);
    uint32_t make_offset(uint32_t address);
    int32_t find_entry(uint32_t index, uint32_t tag); 
};

// End Header Guard
#endif