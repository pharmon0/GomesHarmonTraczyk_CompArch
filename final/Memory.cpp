/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Memory Module */
#include "Memory.h"

//================================
// Memory Constructor
//================================
Memory::Memory(string name, uint32_t memory_bytes, uint32_t block_bytes, uint32_t access_time){
    this->memory_name = name;
    
    this->access_ticks = access_time - 1;
    this->access_counter = this->access_ticks;

    this->bytes_in_memory = memory_bytes;
    this->bytes_in_block = block_bytes;
    this->blocks_in_memory = memory_bytes / block_bytes;

    this->address_mask = 0;
    for(int i = 1; i < block_bytes; i = i << 1){
        this->address_mask |= i;
    }
    this->address_mask = ~this->address_mask;

    for(uint32_t addr = 0; addr < memory_bytes; addr += block_bytes){
        this->bank[addr] = Block();
    }
}

//================================
// Memory Read
//================================
response_t Memory::memory_read(uint32_t address){
    response_t response;
    if(this->access_counter > 0){
        this->access_counter--;
        response.success = false;
        response.reason = "Waiting for Memory";
    } else {
        this->access_counter = this->access_ticks;
        response.success = true;
        response.block = this->bank[address&this->address_mask];
    }
    return response;
}

//================================
// Memory Write
//================================
response_t Memory::memory_write(uint32_t address, Block data){
    response_t response;
    if(this->access_counter > 0){
        this->access_counter--;
        response.success = false;
        response.reason = "Waiting for Memory";
    } else {
        this->access_counter = this->access_ticks;
        response.success = true;
        this->bank[address] = data;
    }
    return response;
}

//================================
// Attach memory bus
//================================
void Memory::attach_bus(Bus* memory_bus){
    this->bus = memory_bus;
}