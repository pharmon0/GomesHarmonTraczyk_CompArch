/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Memory Function Code */
#include "Memory.h"

//============================================
// Constructor
//============================================
Memory::Memory(void){
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
// Memory::processPortI
//  Same Operation as PortD fundamentally,
//   but in practice, read-only 32bit
//============================================
void Memory::processPortI(void){
    if(this->portI.memctrl.memrsz == this->portI.memctrl.memwsz) return;
    if(!this->counterI){
        this->counterI++; //count one cycle
    }else if(this->portI.memctrl.memrsz > this->portI.memctrl.memwsz){ //Load
        this->counterI = 0; //memory done.
        this->portI.memctrl.memack = 1;
        this->portI.data = this->memRead(this->portI.address,
                                        this->portI.memctrl.memrsz);
    }else{ //Store
        this->counterI = 0; //memory done.
        this->portI.memctrl.memack = 1;
        this->memWrite(this->portI.address,
                    this->portI.data,
                    this->portI.memctrl.memrsz);
    }
}

//============================================
// Memory::processPortD
//  Read/Write 8,16,32-bit data
//============================================
void Memory::processPortD(void){
    if(this->portD.memctrl.memrsz == this->portD.memctrl.memwsz) return;
    if(!this->counterD){
        this->counterD++; //count one cycle
    }else if(this->portD.memctrl.memrsz > this->portD.memctrl.memwsz){ //Load
        this->counterD = 0; //memory done.
        this->portD.memctrl.memack = 1;
        this->portD.data = this->memRead(this->portD.address,
                                        this->portD.memctrl.memrsz);
    }else{ //Store
        this->counterD = 0; //memory done.
        this->portD.memctrl.memack = 1;
        this->memWrite(this->portD.address,
                    this->portD.data,
                    this->portD.memctrl.memrsz);
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
// Memory::populate
//  prepopulates memory
//============================================
void Memory::populate(string filename, uint32_t start){
    ifstream file;
    file.open(filename);
    vector<uint32_t> values;
    data32_t val;
    while(file >> val.single){
        values.push_back(val.uinteger);
        cout << "Read : val = " << val.single << endl;
    }
    file.close();
    uint32_t length = values.size();
    for(uint32_t i = 0; i < length * 4; i+= 4){
        this->memWrite(start + i, values.at(0), 0b11);
        cout << hexString(start + i) << ":" << hexString(values.at(0)) << endl;
        values.erase(values.begin());
    }
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