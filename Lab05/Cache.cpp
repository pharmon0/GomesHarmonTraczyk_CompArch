/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * CPU Cache Module */
#include "Cache.h"

//============================================
// Constructors for Cache Object
//============================================
Cache::Cache(void){};
Cache::Cache(uint32_t cacheByteSize, uint32_t blockByteSize, uint8_t assMode){
    //set the size of the cache and each block's size
    this->cacheSize = cacheByteSize;
    this->blockSize = blockByteSize;

    //get the number of blocks
    this->blockCount = this->cacheSize / this->blockSize;

    //get the size of the offset and block address
    this->offsetWidth = ceil(log2(this->blockSize));
    this->tagWidth = SYSTEM_BITWIDTH - this->offsetWidth;
    
    //determine set associativity, index and tag widths
    //Full Associativity has only a single set
    if(assMode == CACHE_ASS_FULLY){
        this->setCount = 1;
        this->indexWidth = 0;
        this->setBlocks = blockCount;
    }else{
        this->setCount = blockCount / assMode;
        this->indexWidth = ceil(log2(setCount));
        this->tagWidth -= this->indexWidth;
        this->setBlocks = blockCount / setCount;
    }

    //get the looktup time of the cache
    this->lookupTicks = ceil(log2(float(cacheSize)/float(blockSize))) * assMode;

    //populate initial dirty cache data
    //iterate through sets
    for(int index = 0; index < this->setCount; index++){
        //iterate through blocks
        for(int tag = 0; tag < this->setBlocks; tag++){
            //populate block
            this->bank[index][tag] = CacheBlock();
        }
    }
}

//============================================
// Function to Find Element in Cache
//  returns true for a hit, false for a miss
//============================================
bool Cache::find(uint32_t address){
    cacheaddr_t addr; addr.address = address;
    splitAddress(&addr);
}

//============================================
// Function to mask address to index, tag, offset
//============================================
void Cache::splitAddress(cacheaddr_t* addr){

}