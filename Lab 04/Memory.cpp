/* ECGR 5181 - Computer Architecture
 * Phillip Harmon
 * Memory Function Code */
#include "Memory.h"

//============================================
// Constructor
//============================================
Memory::Memory(void){
    this->ctrl.all = 0;
    this->address = 0;
    this->data = 0;
    this->bank.clear();
}

/*
//============================================
// Memory::processLoad
//  Load data in dataport 'loaded' if memory read enabled
//============================================
void Memory::processLoad(void){
    this->data = 0x0;
    switch(this->ctrl.memrsz){
        case 0b11: //Word-size
            if(this->bank.find(this->address+3) != this->bank.end()){
                this->data  = 0xFF000000 & (this->bank[this->address+3] << 24);
            }
            if(this->bank.find(this->address+2) != this->bank.end()){
                this->data |= 0x00FF0000 & (this->bank[this->address+2] << 16);
            }
        case 0b10: //Half-size
            if(this->bank.find(this->address+1) != this->bank.end()){
                this->data |= 0x0000FF00 & (this->bank[this->address+1] << 8);
            }
        case 0b01: //Byte-size
            if(this->bank.find(this->address) != this->bank.end()){
                this->data |= 0x000000FF & (this->bank[this->address]);
            }
        break;
    }
}

//============================================
// Memory::processStore
//  Store data 'store' in memory if memory write enabled
//============================================
void Memory::processStore(void){
    switch(this->ctrl.memwsz){
        case 0b11: //Word-size
            this->bank[this->address+3] = ((uint8_t)((this->data>>24)&0xFF));
            this->bank[this->address+2] = ((uint8_t)((this->data>>16)&0xFF));
        case 0b10: //Half-size
            this->bank[this->address+1] = ((uint8_t)((this->data>>8)&0xFF));
        case 0b01: //Byte-size
            this->bank[this->address]   = ((uint8_t)(this->data&0xFF));
        break;
    }
}
*/

//============================================
// Memory::process
//  contains a statemachine to handle
//  - Load or Store selection
//  - Item Width
//  - Artificial Delay
//============================================
void Memory::process(void){

}