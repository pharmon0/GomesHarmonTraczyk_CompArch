/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * CPU Cache Module */

// Start Header Guard
#ifndef GHT_CACHE
#define GHT_CACHE

//============================================
// Libraries
//============================================
#include <cstdint>
#include <math.h>
#include <map>
#include <vector>
#include "CacheBlock.h"
using std::vector;
using std::ceil;
using std::log2;
using std::map;

//============================================
// helpful tuple datatype
//============================================
typedef struct{
    union{
        uint32_t index;
        uint32_t address;
    };
    uint32_t tag;
    uint32_t offset;
} cacheaddr_t;

//============================================
// Constants and Definitions
//============================================
//SYSTEM PARAMETER BITWIDTH (32bit system)
#define SYSTEM_BITWIDTH 32
//cache associativity types
#define CACHE_ASS_FULLY 0
#define CACHE_ASS_DIRECT 1
#define CACHE_ASS_2WAY 2
#define CACHE_ASS_4WAY 4

//============================================
// Class Definition
//============================================
class Cache{
    //time for one cache access
    uint16_t lookupTicks;

    //cache metadata
        //Cache Sizes
        uint32_t cacheSize;
        uint32_t blockSize;
        uint32_t blockCount;
        uint32_t setCount;
        uint32_t setBlocks;

        //width of cache address fields
        uint8_t indexWidth; //log2(a)
        uint8_t tagWidth;
        uint8_t offsetWidth; //log2(blockSize)

    //bank[index][tag][offset] = byte
    vector<vector<CacheBlock> > bank;

    //Bank Access Functions
    uint32_t cacheRead(uint32_t address, uint8_t byteWidth);
    void cacheWrite(uint32_t address, uint32_t data, uint8_t byteWidth);

    //addressing helpers
    void splitAddress(cacheaddr_t* addr);

  public:

    //Constructor
    Cache(void);
    Cache(uint32_t cacheSize, uint32_t blockSize, uint8_t assMode);
    
    //member functions
    bool find(uint32_t address);

};

// End Header Guard
#endif