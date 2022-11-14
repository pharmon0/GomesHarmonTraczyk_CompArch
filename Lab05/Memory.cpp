/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Memory Function Code */
#include "Memory.h"

//============================================
// Constructor
//============================================
Memory::Memory(uint8_t accessTickCount){
    this->accessTicks = accessTickCount;
    this->counterD = accessTickCount-1;
    this->counterIA = accessTickCount-1;
    this->counterIB = accessTickCount-1;
    this->portIA.memctrl.all = 0;
    this->portIA.address = 0;
    this->portIA.data = 0;
    this->portIB.memctrl.all = 0;
    this->portIB.address = 0;
    this->portIB.data = 0;
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
    //portIA
    if(this->portIA.memctrl.memrsz == this->portIA.memctrl.memwsz){
        cout << "Memory | PortIA : No Memory Access this tick! |";
        this->counterIA = this->accessTicks-1;
    }else if(this->counterIA != 0){
        //count one tick
        cout << "Memory | PortIA : Counter Decremented (" << this->counterIA << " --> " << --this->counterIA << ") |";
    }else{
        cout << "Memory | PortIA : Memory Ready";
        //memory done.
        this->counterIA = this->accessTicks-1;
        this->portIA.memctrl.memack = 1;

        //Load
        if(this->portIA.memctrl.memrsz > this->portIA.memctrl.memwsz){
            this->portIA.data = this->memRead(this->portIA.address,
                                            this->portIA.memctrl.memrsz);
            cout << "\n\t" << hexString(this->portIA.data) << " Read from " << hexString(this->portIA.address) << "\n >";
        //Store
        }else if(this->portIA.memctrl.memwsz > this->portIA.memctrl.memrsz){
            this->memWrite(this->portIA.address,
                        this->portIA.data,
                        this->portIA.memctrl.memwsz);
            cout << "\n\t" << hexString(this->portIA.data) << " Written to " << hexString(this->portIA.address) << "\n >";
        }
    }
    //portIB
    if(this->portIB.memctrl.memrsz == this->portIB.memctrl.memwsz){
        cout << " portIB : No Memory Access this tick! |";
        this->counterIB = this->accessTicks-1;
    }else if(this->counterIB != 0){
        //count one tick
        cout << " portIB : Counter Decremented (" << this->counterIB << " --> " << --this->counterIB << ") |";
    }else{
        cout << " portIB : Memory Ready";
        //memory done.
        this->counterIB = this->accessTicks-1;
        this->portIB.memctrl.memack = 1;

        //Load
        if(this->portIB.memctrl.memrsz > this->portIB.memctrl.memwsz){
            this->portIB.data = this->memRead(this->portIB.address,
                                            this->portIB.memctrl.memrsz);
            cout << "\n\t" << hexString(this->portIB.data) << " Read from " << hexString(this->portIB.address) << "\n >";
        //Store
        }else if(this->portIB.memctrl.memwsz > this->portIB.memctrl.memrsz){
            this->memWrite(this->portIB.address,
                        this->portIB.data,
                        this->portIB.memctrl.memwsz);
            cout << "\n\t" << hexString(this->portIB.data) << " Written to " << hexString(this->portIB.address) << "\n >";
        }
    }
    //Port D
    if(this->portD.memctrl.memrsz == this->portD.memctrl.memwsz){
        cout << " PortD : No Memory Access this tick!" << endl;
        this->counterD = this->accessTicks-1;
    }else if(this->counterD != 0){
        //count one tick
        cout << " PortD : Counter Decremented (" << this->counterD << " --> " << --this->counterD << ")" << endl;
    }else{
        cout << " PortD : Memory Ready";
        //memory done.
        this->counterD = this->accessTicks-1;
        this->portD.memctrl.memack = 1;

        //Load
        if(this->portD.memctrl.memrsz > this->portD.memctrl.memwsz){
            this->portD.data = this->memRead(this->portD.address,
                                            this->portD.memctrl.memrsz);
            cout << "\n\t" << hexString(this->portD.data) << " Read from " << hexString(this->portD.address) << endl;
        //Store
        }else if(this->portD.memctrl.memwsz > this->portD.memctrl.memrsz){
            this->memWrite(this->portD.address,
                        this->portD.data,
                        this->portD.memctrl.memwsz);
            cout << "\n\t" << hexString(this->portD.data) << " Written to " << hexString(this->portD.address) << endl;
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

    //TODO: Read cache line from bank
    value = 0;
    value |= this->bank[portD.address]; 
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

    //TODO: Write cache line to bank
    value = 0;
    this->bank[portD.address] = 0xFF & value;
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
// Memory::printToFile
//  prints byte-by-byte memory
//============================================
void Memory::printBytes(string filename){
    ofstream file;
    file.open(filename);
    map<uint32_t,uint8_t>::iterator iter;
    for(iter = this->bank.begin(); iter != this->bank.end(); iter++){
        file << hexString(iter->first) << " : "
            << hexString(iter->second,8) << " (" << bitset<8>(iter->second) << ")" << endl;
    }
    file.close();
}

//============================================
// Memory::printToFile
//  prints Lab 4 Arrays
//============================================
void Memory::printFloats(string filename){
    ofstream file;
    file.open(filename);
    file << "A:\n";
    for(uint32_t i = 0x400; i < 0x800; i+=4){
        data32_t val;
        val.uinteger = memRead(i,0b11);
        file << hexString(i) << " : " << hexString(val.uinteger) << " (" << val.single << ")\n";
    }
    file << "\n=====================================\n";
    file << "\nB:\n";
    for(uint32_t i = 0x800; i < 0xC00; i+=4){
        data32_t val;
        val.uinteger = memRead(i,0b11);
        file << hexString(i) << " : " << hexString(val.uinteger) << " (" << val.single << ")\n";
    }
    file << "\n=====================================\n";
    file << "\nC:\n";
    for(uint32_t i = 0xC00; i < 0x1000; i+=4){
        data32_t val;
        val.uinteger = memRead(i,0b11);
        file << hexString(i) << " : " << hexString(val.uinteger) << " (" << val.single << ")\n";
    }
    file << "\n=====================================\n";
    file << "\nD:\n";
    for(uint32_t i = 0x1000; i < 0x1400; i+=4){
        data32_t val;
        val.uinteger = memRead(i,0b11);
        file << hexString(i) << " : " << hexString(val.uinteger) << " (" << val.single << ")\n";
    }
    file.close();
}