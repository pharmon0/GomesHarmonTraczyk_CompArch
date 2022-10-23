/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
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
    this->counter = 0;
}

//============================================
// Memory::processPortI
//  Read-only 32-bit word instructions
//============================================
void Memory::processPortI(void){
    if(!this->portI.memctrl.request) return;
    if(!this->portI.memctrl.counter){
        this->portI.memctrl.counter++; //count one cycle
    }else{
        this->portI.memctrl.counter = 0; //memory done.
        this->portI.memctrl.ack = 1;
        this->portI.data = this->read(this->portI.address, 0b11); //read 32bits
    }
}

//============================================
// Memory::processPortD
//  Read/Write 8,16,32-bit data
//============================================
void Memory::processPortD(void){
    if(this->portD.memctrl.memrsz == this->portD.memctrl.memwsz) return;
    if(!this->portD.memctrl.counter){
        this->portD.memctrl.counter++; //count one cycle
    }else if(this->portD.memctrl.memrsz > this->portD.memctrl.memwsz){ //Load
        this->portD.memctrl.counter = 0; //memory done.
        this->portD.memctrl.ack = 1;
        this->portD.data = this->read(this->portD.address,
                                        this->portD.memctrl.memrsz);
    }else{ //Store
        this->portD.memctrl.counter = 0; //memory done.
        this->portD.memctrl.ack = 1;
        this->write(this->portD.address,
                    this->portD.data,
                    this->portD.memctrl.memrsz);
    }
}

//============================================
// Memory::read (private)
//  returns memory data
//============================================
uint32_t Memory::read(uint32_t addr, uint8_t size){
    uint32_t value = 0;
    //access memory byte-wise and check if nonzero data is present.
    switch(size){
        case 0b11://WORD
            if(this->bank.find(addr+3) != this->bank.end())
                value |= ((uint32_t)(this->bank[addr+3])) << 24;
            if(this->bank.find(addr+2) != this->bank.end())
                value |= ((uint32_t)(this->bank[addr+2])) << 16;
        case 0b10://HALF
            if(this->bank.find(addr+1) != this->bank.end())
                value |= ((uint32_t)(this->bank[addr+1])) << 8;
        case 0b01://BYTE
            if(this->bank.find(addr) != this->bank.end())
                value |= (uint32_t)(this->bank[addr]);
    }
    return value;
}

//============================================
// Memory::write (private)
//  stores memory data
//============================================
void Memory::write(uint32_t addr, uint32_t value, uint8_t size){
    switch(size){
        //access memory bytewise and delete zeroed data
        case 0b11://WORD
            this->bank[addr+3] = 0xFF & (value >> 24);
            if(this->bank[addr+3] == 0) this->bank.erase(addr+3)
            this->bank[addr+2] = 0xFF & (value >> 16);
            if(this->bank[addr+2] == 0) this->bank.erase(addr+2)
        case 0b10://HALF
            this->bank[addr+1] = 0xFF & (value >> 8);
            if(this->bank[addr+1] == 0) this->bank.erase(addr+1)
        case 0b01://BYTE
            this->bank[addr] = 0xFF & value;
            if(this->bank[addr] == 0) this->bank.erase(addr)
    }
}