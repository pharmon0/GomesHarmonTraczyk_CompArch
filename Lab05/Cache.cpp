/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * CPU Cache Module */
#include "Cache.h"

//============================================
// Constructors for Cache Object
//============================================
Cache::Cache(void){};
Cache::Cache(uint32_t cacheByteSize, uint32_t blockByteSize, uint8_t assMode, memport_t* cpuPort, string ID){
    this->name = ID;
 
    this->cpuPort = cpuPort;
    
    this->accesses = 0;
    this->misses = 0;

    //set the size of the cache and each block's size
    this->cacheSize = cacheByteSize;
    this->blockSize = blockByteSize;

    //get the number of blocks
    this->blockCount = this->cacheSize / this->blockSize;

    //get the size of the offset and block address
    this->offsetWidth = ceil(log2(this->blockSize));
    this->tagWidth = SYSTEM_BITWIDTH - this->offsetWidth;

    //determine set associativity, index and tag widths
    //Full Associativity has only a single set
    if(assMode == CACHE_ASS_FULLY){

        this->setCount = 1;
        this->indexWidth = 0;
        this->setBlocks = blockCount;
    }else{

        this->setCount = blockCount / assMode;
        this->indexWidth = ceil(log2(setCount));
        this->tagWidth -= this->indexWidth;
        this->setBlocks = blockCount / setCount;
    }

    //get the looktup time of the cache
    this->lookupTicks = ceil(log2(float(cacheSize)/float(blockSize))) * assMode;

    //populate initial dirty cache data
    //iterate through sets
    for(int index = 0; index < this->setCount; index++){
        this->bank.push_back({});
        //iterate through blocks
        for(int entry = 0; entry < this->setBlocks; entry++){
            //populate block
            this->bank[index].push_back(CacheBlock(this->blockSize,0));
        }
    }

    this->state = LOOKUP_STATE;
}

void Cache::process(){
    /* ALGORITHM
    STATE: Lookup
        Check if memory requested
            run counter for lookup time
            if counter done
                state -> Checkin
    STATE: Checkin
        check if block is present (read hit/miss detect)
            if read hit
                state -> read hit
            if write hit
                state -> write hit
            if read miss
                state -> read miss
            if write miss
                state -> write miss
    STATE: read hit
        return the data.
        done.
    STATE: read miss
        broadcast request
        wait for the data.
        if replaced data is valid
            if replaced data is modified
                broadcast save to memory
                wait for broadcast complete
            else
                broadcast dump
                wait for broadcast complete
        return the data.
        done.
    STATE: write hit
        broadcast modify
        wait for broadcast complete
        if modified block is
        modify the data
        done.
    STATE: write miss
        broadcast request with mod
        wait for the data
        modify the data
        done
    */ 
    /*
    if(entry < 0){
        //MISS! Block not in Cache
        this->misses++;

        cout << "Cache::byteRead() | miss! block not in cache" << endl;

        // FIX ME Placeholder memory fetch!
        this->blockWrite(index, tag, CacheBlock(this->blockSize,tag));

        entry = this->findEntry(index, tag);

        cout << "Cache::byteRead() | index=" << index << " entry=" << entry
             << " bank[index][entry].bytes.size()=" << this->bank[index][entry].bytes.size() << endl;

    }else if(this->bank[index][entry].getMESI() == MESI_I){
        //MISS! Block not valid
        this->misses++;

        cout << "Cache::byteRead() | miss! block invalid" << endl;

        // FIX ME Placeholder memory fetch!
        this->blockWrite(index, tag, CacheBlock(this->blockSize,tag));

        entry = this->findEntry(index, tag);
    }

    cout << "Cache::byteRead() | any misses have been resolved" << endl;
    */

   /* starting over
    if(this->state == LOOKUP_STATE){
    //perform lookup operations
        if(this->cpuPort->memctrl.memrsz == this->cpuPort->memctrl.memwsz){
            cout << "Cache::process() | No Memory Access this tick!" << endl;
            this->lookupCounter = this->lookupTicks-1;
        }else if(this->lookupCounter != 0){
            //count one tick
            cout << "Cache::process() | Counter Decremented (" << this->lookupCounter << " --> " << --this->lookupCounter << ")" << endl;
        }else{
            cout << "Cache::process() | Lookup Complete!" << endl;
            //memory done.
            this->lookupCounter = this->lookupTicks-1;
            this->state = CHECK_STATE;

        }
    }
    if(this->state == CHECK_STATE){
    //This part initiates the cache access

        this->tag = this->makeTag(this->cpuPort->address);
        this->index = this->makeIndex(this->cpuPort->address);
        this->offset = this->makeOffset(this->cpuPort->address);
        this->entry = this->findEntry(this->index, this->tag);
    
        cout << "Cache::process() | resolved address :: address=" << hexString(this->cpuPort->address)
            << " tag=" << bitset<32>(this->tag)
            << " index=" << bitset<32>(this->index)
            << " offset=" << bitset<32>(this->offset)
            << " entry=" << this->entry <<  endl;
        
        if(this->entry >= 0){
        //BLOCK IS IN CACHE!
            if(this->bank[this->index][this->entry].getMESI() != MESI_I){
            //BLOCK IS VALID!
                if(this->cpuPort->memctrl.memwsz > this->cpuPort->memctrl.memrsz){
                //WRITE HIT!
                    //WRITING TO MEMORY! INFORM THE BUS!
                    //"I AM WRITING DATA TO [ADDRESS]!"
                    //TO DO inform the bus
                    this->state = BUS_STATE;
                }else{
                //READ HIT!
                    this->state = ACCESS_STATE;
                }
            }else{
            //BLOCK IS INVALID, MISS!
                this->misses++;
            }
        }else{
        //BLOCK IS NOT IN CACHE, MISS!
            this->misses++;
        }
    }
    if(this->state == BUS_STATE){
        //TO DO initialize MESI messaging

    }
    if(this->state == ACCESS_STATE){
    //this part actually gives/takes memory from the CPU

        this->tag = this->makeTag(this->cpuPort->address);
        this->index = this->makeIndex(this->cpuPort->address);
        this->offset = this->makeOffset(this->cpuPort->address);
        this->entry = this->findEntry(this->index, this->tag);
    
        cout << "Cache::process() | resolved address :: address=" << hexString(this->cpuPort->address)
            << " tag=" << bitset<32>(this->tag)
            << " index=" << bitset<32>(this->index)
            << " offset=" << bitset<32>(this->offset)
            << " entry=" << this->entry <<  endl;

        //Load
        if(this->cpuPort->memctrl.memrsz > this->cpuPort->memctrl.memwsz){
            this->cpuPort->data = this->byteRead(this->index,this->entry,this->offset,
                                                 this->cpuPort->memctrl.memrsz);
            cout << "Cache::process() | " << hexString(this->cpuPort->data) << " Read from "
                 << hexString(this->cpuPort->address) << "\n >" << endl;
        //Store
        }else if(this->cpuPort->memctrl.memwsz > this->cpuPort->memctrl.memrsz){
            this->byteWrite(this->index,this->entry,this->offset,
                            this->cpuPort->data,this->cpuPort->memctrl.memwsz);
            cout << "Cache::process() | " << hexString(this->cpuPort->data) << " Written to "
                 << hexString(this->cpuPort->address) << "\n >" << endl;
        }
        //reset flags
        this->state = LOOKUP_STATE;
        this->cpuPort->memctrl.memack = 1;
    }
    */
}

//============================================
// Function to Find Element in Cache
//  returns -1 for tag not found
//  otherwise returns entry of tag
//============================================
int32_t Cache::findEntry(uint32_t index, uint32_t tag){
    cout << "Cache::findEntry() | entered" << endl;
    cout << " index=" << bitset<32>(index) << " this->setBlocks=" << this->setBlocks 
         << " tag=" << bitset<32>(tag) << endl;
    for(uint32_t entry = 0; entry < this->setBlocks; entry++){
        if(this->bank[index][entry].getTag() == tag){
            cout << "Cache::findEntry() | Found tag! entry=" << entry << endl;
            return entry;
        }
    }
    cout << "Cache::findEntry() | Never Found. Exiting with code -1" << endl;
    return -1;
}

//============================================
// Returns the tag of an address
//============================================
uint32_t Cache::makeTag(uint32_t address){
    return address >> (SYSTEM_BITWIDTH - this->tagWidth);
}

//============================================
// Returns the index of an address
//============================================
uint32_t Cache::makeIndex(uint32_t address){
    if(this->setCount == 1) return 0;
    return (address << this->tagWidth) >> (this->tagWidth + this->offsetWidth);
}

//============================================
// Returns the offset of an address
//============================================
uint32_t Cache::makeOffset(uint32_t address){
    return (address << (SYSTEM_BITWIDTH - this->offsetWidth)) >> (SYSTEM_BITWIDTH - this->offsetWidth);
}

//==================================================================
// Function reads from bytes within a cache block
//  increments access count
//  updates LRU
//  WARNING! Non-word-aligned words may break the whole thing
//==================================================================
uint32_t Cache::byteRead(uint32_t index, uint32_t entry, uint32_t offset, uint8_t byteWidth){



    cout << "Cache::byteRead() | entered Cache::byteRead()" << endl;

    this->accesses++;
    
    //read the bytes
    uint32_t value = 0;
    switch(byteWidth){ //access memory byte-wise
        case 0b11://WORD
            value |= (this->bank[index][entry].readOffset(offset+3)) << 24;
            value |= (this->bank[index][entry].readOffset(offset+2)) << 16;
        case 0b10://HALF
            value |= (this->bank[index][entry].readOffset(offset+1)) << 8;
        case 0b01://BYTE
            value |= this->bank[index][entry].readOffset(offset);
    }
    cout << "Cache::byteRead() | data has been read" << endl;
    
    //LRU update
    this->bank[index][entry].setLRU(true);
    bool LRUSearch = true;
    for(int block = 0; block < this->bank[index].size(); block++){
        LRUSearch = LRUSearch && this->bank[index][block].getLRU();
    }
    if(LRUSearch){
        for(int block = 0; block < this->bank[index].size(); block++){
            this->bank[index][block].setLRU(false);
        }
        this->bank[index][entry].setLRU(true);
    }

    cout << "Cache::byteRead() | lru updated" << endl;
    cout << "Cache::byteRead() | Exiting Cache::byteRead()" << endl;
    return value;
    //hit/miss detection, MESI are handled by the calling function, not here.
}

//==================================================================
// Function reads a cache block from a set and a known entry
//==================================================================
CacheBlock Cache::blockRead(uint32_t index, uint32_t entry){
    return this->bank[index][entry];
}

//==================================================================
// Function writes to a byte within a cache block
//  increments accesses
//  updates LRU
//  WARNING! Non-word-aligned words may break the whole thing
//==================================================================
void Cache::byteWrite(uint32_t index, uint32_t entry, uint32_t offset, uint32_t data, uint8_t byteWidth){
    this->accesses++;
    //write the bytes
    switch(byteWidth){
        //access memory bytewise and delete zeroed data
        case 0b11://WORD
            this->bank[index][entry].writeOffset(offset+3,0xFF & (data >> 24));
            this->bank[index][entry].writeOffset(offset+2,0xFF & (data >> 16));
        case 0b10://HALF
            this->bank[index][entry].writeOffset(offset+1,0xFF & (data >> 8));
        case 0b01://BYTE
            this->bank[index][entry].writeOffset(offset,0xFF & data);
    }

    //LRU update
    this->bank[index][entry].setLRU(true);
    bool LRUSearch = true;
    for(int block = 0; block < this->bank[index].size(); block++){
        LRUSearch = LRUSearch && this->bank[index][block].getLRU();
    }
    if(LRUSearch){
        for(int block = 0; block < this->bank[index].size(); block++){
            this->bank[index][block].setLRU(false);
        }
        this->bank[index][entry].setLRU(true);
    }

    //hit/miss detection, MESI are handled by the calling function, not here.
}

//==================================================================
// Function to add data to cache using pseudo-LRU replacement policy
//==================================================================
uint32_t Cache::blockWrite(uint32_t index, CacheBlock block){

    cout << "Cache::blockWrite() | entered function" << endl;

    //find Least Recently Used block to replace
    bool seekingLRU = true;
    int32_t entry = 0;

    cout << "\tCache::blockWrite() | Entering LRU Loop" << endl;
    
    while(seekingLRU){
    
        cout << "\tCache::blockWrite() | LRU LOOPING :: entry=" << entry << endl;

        //if all entries have LRU==1, set all to 0.
        if(entry == this->setBlocks){

            cout << "\tCache::blockWrite() | Exhausted the loop. Setting all to LRU=0" << endl;
    
            for(entry = 0; entry < this->setBlocks; entry++){
                this->bank[index][entry].setLRU(false);
            }
            entry = 0;
        }

        //check if entry is LRU
        if(!this->bank[index][entry].getLRU()){

            cout << "\tCache::blockWrite() | FOUND LRU! entry=" << entry << endl;

            seekingLRU = false;
        }

        //if not done seeking, increment entry
        if(seekingLRU){

            cout << "\tCache::blockWrite() | incrementing entry from " << entry;

            entry++;

            cout << " to " << entry << endl;

        }
    }



    cout << "Cache::blockWrite() | Entry to replace is " << entry
         << " block.bytes.size()=" << block.bytes.size() <<  endl;

    //check entry for modifications (if modified, store back to memory)
    //TODO Store back to memory

    //store new block
    this->bank[index][entry] = block;
}

//==================================================================
// Prints Cache Statistics
//==================================================================
void Cache::printStats(void){
    uint32_t hits = this->accesses - this->misses;
    float hitrate = 100.0 * float(hits) / float(this->accesses);
    float missrate = 100.0 * float(this->misses) / float(this->accesses);
    uint32_t ticks = (this->accesses * this->lookupTicks) + (this->misses * MEM_TICKS);

    cout << "Cache Statistics" << endl;
    cout << " Total Cache Size : " << this->cacheSize << "B" << endl;
    cout << " Block Size : " << this->blockSize << "B" << endl;
    cout << " Block Count : " << this->blockCount << endl;
    cout << " Associativity : ";
    if(this->setCount == 1){
        cout << "Fully Associative";
    }else if(this->setCount == this->blockCount){
        cout << "Directly Mapped";
    }else{
        cout << this->setBlocks << "-way";
    }
    cout << endl;
    cout << " Cache Lookup Time : " << this->lookupTicks << " simulation ticks" << endl;
    cout << endl;
    cout << " Total Cache Accesses : " << this->accesses << endl;
    cout << " Total Cache Hits : " << hits << endl;
    cout << " Total Cache Misses : " << this->misses << endl;
    cout << " Cache Hit Rate : " << hitrate << "%" << endl;
    cout << " Cache Miss Rate : " << missrate << "%" << endl;
    cout << " Total Cache Access Time : " << ticks << " simulation ticks ("
         << (float(ticks)/10.0) << " clock cycles)" << endl;
}