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
#include "Datatypes.h"
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
// blockport_t Datatype
// - serves as a memory access point between cache and memory
//============================================
typedef struct{
    uint32_t address;
    CacheBlock data;
    mesi_message_t mesi;
    union{
        uint8_t all;
        struct{
            uint8_t    read : 1; //read a block
            uint8_t   write : 1; //write a block
            uint8_t request : 1; //has a memory request been made?
            uint8_t  memack : 1; //is memory operation complete?
            uint8_t:0; //union alignment
        };
    } memctrl;
} blockport_t;

//============================================
// Class Definition
//============================================
class CacheBlock{    
    //CacheBlock metadata
    //address tag
    uint32_t tag;
    //LRU flag
    bool LRU;
    //tag set flag
    bool tagSet;

  public:

    //MESI flags
    uint8_t mesi;
      //blocksize
      uint32_t blockSize;

    //bytes[offset] = byte
    vector<uint8_t> bytes;

    //Constructor
    CacheBlock(void);
    CacheBlock(uint32_t size, uint32_t addrtag);
    CacheBlock(vector<uint8_t> data, uint32_t addrtag);
    CacheBlock(vector<uint8_t> data);
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