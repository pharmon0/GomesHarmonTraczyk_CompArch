/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Memory Module */
#include "Memory.h"

//================================
// Memory Constructor
//================================
Memory::Memory(string name, uint32_t bytes, uint32_t access_time){
    this->memory_name = name;
    this->bytes_in_memory = bytes;
    this->access_ticks = access_time;
}

//================================
// Memory Constructor
//================================
Block Memory::memory_read(uint32_t address){
    if(this->bank.find(address) != this->bank.end()){
        return this->bank[address];
    }
    return 0;
}

//================================
// Memory Constructor
//================================
void Memory::memory_write(uint32_t address, Block data){
    this->bank[address] = data;
}