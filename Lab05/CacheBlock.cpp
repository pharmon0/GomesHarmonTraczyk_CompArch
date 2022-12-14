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
    this->LRU = false;
    this->blockSize = 0;
    this->bytes = {};
    this->tagSet = false;
}
CacheBlock::CacheBlock(uint32_t size, uint32_t addrtag){
    
    cout << "CacheBlock(size,tag) | entered constructor" << endl;
    
    this->mesi = MESI_I;
    this->tag = addrtag;
    this->LRU = false;
    this->blockSize = size;
    for(int i = 0; i < this->blockSize; i++){
        this->bytes.push_back(0);
    }
    this->tagSet = true;
    
    cout << "CacheBlock(size,tag) | Finished For Loop, this->bytes.size()=" << this->bytes.size() << endl;
}
CacheBlock::CacheBlock(vector<uint8_t> data, uint32_t addrtag){
    this->mesi = MESI_I;
    this->tag  = addrtag;
    this->LRU = false;
    this->bytes = data;
    this->blockSize = data.size();
    this->tagSet = true;
}
CacheBlock::CacheBlock(vector<uint8_t> data){
    this->mesi = MESI_I;
    this->tag  = 0;
    this->tagSet = false;
    this->LRU = false;
    this->bytes = data;
    this->blockSize = data.size();
    cout << "CacheBlock::CacheBlock(data) | mesi=" << this->mesi << " tag=" << this->tag << " tagSet="
         << this->tagSet << " LRU=" << this->LRU << " blockSize=" << this->blockSize << " bytes.size()=" << this->bytes.size() << endl;
}
//CacheBlock::CacheBlock(vector<uint8_t> data, uint32_t addrtag, uint8_t status){
//    this->mesi = status;
//    this->tag = addrtag;
//    this->LRU = false;
//    this->bytes = data;
//    this->blockSize = data.size();
//    this->tagSet = true;
//}

//============================================
// Read byte from block
// This simply returns the data present at block[offset]
//============================================
uint8_t CacheBlock::readOffset(uint32_t offset){

    cout << "CacheBlock::readOffset() | offset=" << offset << " bytesSize=" << this->bytes.size() << endl; 

    if(this->bytes.size() > this->blockSize){
        cout << "CacheBlock::readOffset() | WARNING! ARRAY SIZE EXCEEDS BLOCK SIZE. GET READY TO WUMBO" << endl;
    }

    return this->bytes.at(offset);
}

//============================================
// Write byte to block
// This simply writes to block[offset]
//============================================
void CacheBlock::writeOffset(uint32_t offset, uint8_t byte){
    this->bytes.at(offset) = byte;
}

//============================================
// Accessors and Mutators
//============================================
uint32_t CacheBlock::getTag(void){return this->tag;}
uint8_t  CacheBlock::getMESI(void){return this->mesi;}

bool CacheBlock::getLRU(void){return this->LRU;}
void CacheBlock::setLRU(bool newLRU){this->LRU = newLRU;}