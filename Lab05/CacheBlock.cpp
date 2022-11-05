/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * CPU Cache Block Module */
#include "CacheBlock.h"

//============================================
// Constructors
//============================================
CacheBlock::CacheBlock(void){
    this->mesi = MESI_I;
    this->tag = 0;
    this->bytes.clear();
}
CacheBlock::CacheBlock(vector<uint8_t> data, uint32_t addrtag){
    this->mesi = MESI_E;
    this->tag  = addrtag;
    this->bytes.clear();
    for(int i = 0; i < data.size(); i++){
        this->bytes[i] = data[i];
    }
}
CacheBlock::CacheBlock(vector<uint8_t> data, uint32_t addrtag, uint8_t status){
    this->mesi = status;
    this->tag = addrtag;
    this->bytes.clear();
    for(int i = 0; i < data.size(); i++){
        this->bytes[i] = data[i];
    }
}

//============================================
// Read byte from block
// This simply returns the data present at block[offset]
//============================================
uint8_t CacheBlock::readOffset(uint32_t offset){
    if(this->bytes.find(offset) != this->bytes.end())
        return this->bytes[offset];
    return 0;
}

//============================================
// Write byte to block
// This simply writes to block[offset]
//============================================
uint8_t CacheBlock::writeOffset(uint32_t offset, uint8_t byte){
    this->bytes[offset] = byte;
    if(this->bytes[offset] == 0)
        this->bytes.erase(offset);
}