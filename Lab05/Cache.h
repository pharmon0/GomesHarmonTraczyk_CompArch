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
#include <iostream>
#include "CacheBlock.h"
#include "Functions.h"
#include "Datatypes.h"
#include <string>
#include <bitset>
using std::string;
using std::bitset;
using std::vector;
using std::ceil;
using std::log2;
using std::map;
using std::cout;
using std::endl;

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
//tickrates
#define MEM_TICKS 100

//============================================
// Class Definition
//============================================
class Cache{

    //cache operating state (see process())
        cache_state_t state;
        uint32_t index;
        uint32_t tag;
        uint32_t offset;
        uint32_t entry;

    //cache statistic data
        uint32_t accesses; //total accesses
        uint32_t misses; //total misses

    //cache metadata
        string name;
        //time for one cache access
        uint16_t lookupTicks;
        uint16_t lookupCounter;

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

    //to access bank correctly:
    //get tag, index, offset from address using splitAddress()
    //loop for entries from 0 to setBlocks
    //  if bank[index][entry].getTag() matches tag:
    //      bank[index][entry][offset] = byte
    vector<vector<CacheBlock> > bank;

    //Can the tag be found in the set?
    //returns -1 for tag not found
    //otherwise returns entry of tag
    int32_t findEntry(uint32_t index, uint32_t tag);

  public:

    //addressing helpers
    uint32_t makeTag(uint32_t address);
    uint32_t makeIndex(uint32_t address);
    uint32_t makeOffset(uint32_t address);

    //cache data ports
        blockport_t membusPort;
        memport_t* cpuPort;

    //Constructor
    Cache(void);
    Cache(uint32_t cacheSize, uint32_t blockSize, uint8_t assMode, memport_t* cpuPort, string ID);

    //Bank Access Functions
    uint32_t byteRead(uint32_t index, uint32_t entry, uint32_t offset, uint8_t byteWidth);
    CacheBlock blockRead(uint32_t index, uint32_t tag);
    void byteWrite(uint32_t index, uint32_t entry, uint32_t offset, uint32_t data, uint8_t byteWidth);
    void blockWrite(uint32_t index, uint32_t tag, CacheBlock block);

    //statistics
    void printStats(void);

    void process();
};

// End Header Guard
#endif