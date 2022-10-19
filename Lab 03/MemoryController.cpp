/* ECGR 5181 - Computer Architecture
 * Lab 03
 * Phillip Harmon
 * Memory Controller Function Code */
#include "MemoryController.h"

//============================================
// Constructor
//============================================
MemoryController::MemoryController(void){
    this->ctrl.all = 0;
    this->address = 0;
    this->loaded = 0;
    this->memory = 12345678;
}

//============================================
// MemoryController::processLoad
//  Load data in dataport 'loaded' if memory read enabled
//============================================
void MemoryController::processLoad(void){
    //!!!PLACEHOLDER CODE!!!
    switch(this->ctrl.memrsz){
        case 0b11: this->loaded = this->memory;            break;
        case 0b10: this->loaded = this->memory&0x0000FFFF; break;
        case 0b01: this->loaded = this->memory&0x000000FF; break;
    }
}

//============================================
// MemoryController::processStore
//  Store data 'store' in memory if memory write enabled
//============================================
void MemoryController::processStore(int32_t store){
    //!!!PLACEHOLDER CODE!!!
    switch(this->ctrl.memwsz){
        case 0b11:
            this->memory = store;
        break;
        case 0b10:
            this->memory &= 0xFFFF0000;
            this->memory |= (store&0x0000FFFF);
        break;
        case 0b01:
            this->memory &= 0xFFFFFF00;
            this->memory |= (store&0x000000FF);
        break;
    }
}