/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Memory Function Code */
#include "Memory.h"

//============================================
// Constructor
//============================================
Memory::Memory(uint8_t accessTickCount){
    this->accessTicks = accessTickCount;
    this->counterD = 0;
    this->counterI = 0;
    this->portI.memctrl.all = 0;
    this->portI.address = 0;
    this->portI.data = 0;
    this->portD.memctrl.all = 0;
    this->portD.address = 0;
    this->portD.data = 0;
    this->bank.clear();
}

//============================================
// Memory::process
//  Read/Write 8,16,32-bit data to/from ports
//============================================
void Memory::process(uint64_t tick){
    //Port I
    if(this->portI.memctrl.memrsz == this->portI.memctrl.memwsz);
    else if(this->counterI != 0){
        //count one tick
        this->counterI--; 
    }else{
        //memory done.
        this->counterI = this->accessTicks;
        this->portI.memctrl.memack = 1;

        //Load
        if(this->portI.memctrl.memrsz > this->portI.memctrl.memwsz){
            this->portI.data = this->memRead(this->portI.address,
                                            this->portI.memctrl.memrsz);
        //Store
        }else{
            this->memWrite(this->portI.address,
                        this->portI.data,
                        this->portI.memctrl.memrsz);
        }
    }
    //Port D
    if(this->portD.memctrl.memrsz == this->portD.memctrl.memwsz);
    else if(this->counterD != 0){
        //count one tick
        this->counterD--; 
    }else{
        //memory done.
        this->counterD = this->accessTicks;
        this->portD.memctrl.memack = 1;

        //Load
        if(this->portD.memctrl.memrsz > this->portD.memctrl.memwsz){
            this->portD.data = this->memRead(this->portD.address,
                                            this->portD.memctrl.memrsz);
        //Store
        }else{
            this->memWrite(this->portD.address,
                        this->portD.data,
                        this->portD.memctrl.memrsz);
        }
    }
}

//============================================
// Memory::read (private)
//  returns memory data
//============================================
uint32_t Memory::memRead(uint32_t addr, uint8_t size){
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
void Memory::memWrite(uint32_t addr, uint32_t value, uint8_t size){
    switch(size){
        //access memory bytewise and delete zeroed data
        case 0b11://WORD
            this->bank[addr+3] = 0xFF & (value >> 24);
            if(this->bank[addr+3] == 0) this->bank.erase(addr+3);
            this->bank[addr+2] = 0xFF & (value >> 16);
            if(this->bank[addr+2] == 0) this->bank.erase(addr+2);
        case 0b10://HALF
            this->bank[addr+1] = 0xFF & (value >> 8);
            if(this->bank[addr+1] == 0) this->bank.erase(addr+1);
        case 0b01://BYTE
            this->bank[addr] = 0xFF & value;
            if(this->bank[addr] == 0) this->bank.erase(addr);
    }
}

//============================================
// Memory::populateFloat
//  prepopulates memory
//============================================
void Memory::populateFloat(string filename, uint32_t start){
    ifstream file;
    file.open(filename);
    if(!file.is_open())
        cout << "FILE '" << filename << "' DID NOT OPEN" << endl;
    data32_t val;
    uint32_t i = 0;
    while(file >> val.single){
        this->memWrite(start+i, val.uinteger, 0b11);
        i += 4;
    }
    file.close();
}

//============================================
// Memory::populateInt
//  prepopulates memory
//============================================
void Memory::populate(string filename, uint32_t start){
    ifstream file;
    file.open(filename);
    if(!file.is_open())
        cout << "FILE '" << filename << "' DID NOT OPEN" << endl;
    uint32_t val;
    uint32_t i = 0;
    while(file >> val){
        this->memWrite(start+i, val, 0b11);
        i += 4;
    }
    file.close();
}

//============================================
// Memory::populate
//  prepopulates memory
//============================================
void Memory::printToFile(string filename){
    ofstream file;
    file.open(filename);
    map<uint32_t,uint8_t>::iterator iter;
    for(iter = this->bank.begin(); iter != this->bank.end(); iter++){
        file << hexString(iter->first) << ":" << bitset<8>(iter->second) << endl;
    }
    file.close();
}