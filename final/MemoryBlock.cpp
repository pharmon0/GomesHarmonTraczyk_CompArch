/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Memory Block Module */
#include "MemoryBlock.h"

//================================
// Block Constructor
//================================
//Default Constructor
Block::Block(){
    this->block_tag = 0;
    this->least_recent = false;
    this->cold_start = false;
    this->bank.clear();
}

//================================
// Set Tag
//================================
void Block::set_tag(uint32_t tag){
    this->block_tag = tag;
}

//================================
// Set cold_start value
//================================
void Block::start_cold(void){
    this->cold_start = true;
}

//================================
// Get cold_start value
//================================
bool Block::is_cold(void) const{
    return this->cold_start;
}

//================================
// Read from the Block
//================================
uint8_t Block::read_byte(uint32_t offset){
    if(this->bank.find(offset) != this->bank.end()){
        return this->bank[offset];
    }
    return 0;
}

//================================
// Write to the Block
//================================
void Block::write_byte(uint32_t offset, uint8_t data){
    if(data == 0){
        if(this->bank.find(offset) != this->bank.end()){
            this->bank.erase(offset);
        }
        return;
    }
    this->bank[offset] = data;
}