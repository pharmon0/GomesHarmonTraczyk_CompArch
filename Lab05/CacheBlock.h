/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * CPU Cache Block Module */

// Start Header Guard
#ifndef GHT_CACHEBLOCK
#define GHT_CACHEBLOCK

//============================================
// Libraries
//============================================
#include <cstdint>
#include <map>
#include <vector>
#include <iostream>
#include <bitset>
using std::bitset;
using std::cout;
using std::endl;
using std::vector;
using std::map;

//============================================
// Constants and Definitions
//============================================
#define MESI_M 0b11
#define MESI_E 0b10
#define MESI_S 0b01
#define MESI_I 0b00

//============================================
// Class Definition
//============================================
class CacheBlock{    
    //CacheBlock metadata
      //blocksize
      uint32_t blockSize;

    //MESI flags
    uint8_t mesi;
    //address tag
    uint32_t tag;
    //LRU flag
    bool LRU;

  public:
    //bytes[offset] = byte
    vector<uint8_t> bytes;

    //Constructor
    CacheBlock(void);
    CacheBlock(uint32_t size, uint32_t addrtag);
    CacheBlock(vector<uint8_t> data, uint32_t addrtag);
    CacheBlock(vector<uint8_t> data, uint32_t addrtag, uint8_t status);

    uint8_t readOffset(uint32_t offset);
    void writeOffset(uint32_t offset, uint8_t byte);

    uint32_t getTag(void);
    uint8_t getMESI(void);

    bool getLRU(void);
    void setLRU(bool newLRU);
};

// End Header Guard
#endif