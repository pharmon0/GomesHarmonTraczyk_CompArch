/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Cache Module */
#include "Cache.h"

//================================
// Cache Constructor
//================================
Cache::Cache(string name, uint32_t cache_bytes, uint32_t block_bytes, uint8_t associativity){
    this->cache_name = name;

    this->bytes_in_cache = cache_bytes;
    this->bytes_in_block = block_bytes;
    this->blocks_in_cache = cache_bytes / block_bytes;

    if(associativity == ASSOCIATIVITY_FULL){
        //fully associative memory behaves
        // as if it has a single set
        this->sets_in_cache = 1;
        this->index_width = 0;
        this->blocks_in_set = this->blocks_in_cache;
    } else {
        //other associativities behave thusly
        this->sets_in_cache = this->blocks_in_cache / associativity;
        this->index_width = ceil(log2(this->sets_in_cache));
        this->blocks_in_set = associativity;
    }

    this->offset_width = ceil(log2(block_bytes));
    this->tag_width = SYSTEM_BITWIDTH - this->offset_width - this->index_width;
    
    this->offset_mask = 0;
    for(int i = 0; i < this->offset_width; i++){
        this->offset_mask |= (1<<i);
    }
    //cout << this->cache_name << "| offset_width=" << setw(2) << this->offset_width << " mask=" << bitset<32>(this->offset_mask) << endl;
    
    this->index_mask = 0;
    for(int i = 0; i < this->index_width; i++){
        this->index_mask |= (1<<i);
    }
    for(int i = 0; i < this->offset_width; i++){
        this->index_mask = this->index_mask << 1;
    }
    //cout << this->cache_name << "|  index_width=" << setw(2) << this->index_width << " mask=" << bitset<32>(this->index_mask) << endl;

    this->tag_mask = 0;
    for(int i = 0; i < this->tag_width; i++){
        this->tag_mask |= (1<<i);
    }
    for(int i = 0; i < (this->offset_width + this->index_width); i++){
        this->tag_mask = this->tag_mask << 1;
    }
    //cout << this->cache_name << "|    tag_width=" << setw(2) << this->tag_width << " mask=" << bitset<32>(this->tag_mask) << endl;

    this->access_ticks = ceil(log2(this->bytes_in_cache/this->bytes_in_block)) - 1;
    this->access_counter = this->access_ticks;

    for(int i = 0; i < this->sets_in_cache; i++){
        vector<Block> new_set;
        for(int j = 0; j < this->blocks_in_set; j++){
            Block new_block = Block();
            new_block.start_cold();
            new_set.push_back(new_block);
        }
        this->bank.push_back(new_set);
    }
}

//================================
// Manually change cache access time
//================================
void Cache::set_access_time(uint32_t ticks){
    this->access_ticks = ticks - 1;
    this->access_counter = this->access_ticks;
}

//================================
// Cache Access by CPU
//  - controlled and timed
//================================
response_t Cache::cache_access(uint32_t address, uint32_t data, bool write, uint8_t data_width){
    this->total_tick_counter++;
    response_t response;
    if(this->access_counter > 0){
        //still counting down.
        this->access_counter--;
        response.data = 0;
        response.success = false;
        response.reason = "waiting for cache access";
    }else{
        uint32_t tag = this->make_tag(address);
        uint32_t index = this->make_index(address);
        uint32_t offset = this->make_offset(address);
        int32_t entry = this->find_entry(index, tag);
        if(entry < 0){
        //tag not found in cache (MISS)
            response_t miss_response = this->handle_miss(address, write);
            response.success = false;
            response.reason = "Cache Miss | Miss:" + miss_response.reason;
        } else {
        //tag found in cache
            if(this->bank[index][entry].get_mesi() != MESI_I){
            //Block is valid (HIT)
                if(write){
                //write data
                    if(this->bank[index][entry].get_mesi() == MESI_E || this->bank[index][entry].get_mesi() == MESI_M){
                    //Only I hold this data. I can write freely.
                        this->bank[index][entry].set_mesi(MESI_M);
                        this->write_to_block(index,entry,offset,data,data_width);
                        this->update_lru(index,entry);
                        response.success = true;
                    } else { //this means MESI == S
                    //I am shared, I need to invalidate this block on the bus
                        response_t bus_response = this->bus->bus_request(this, BUS_INVALIDATE, address, Block());
                        if(bus_response.success){
                        //other caches successfully invalidated
                            this->bank[index][entry].set_mesi(MESI_M);
                            this->write_to_block(index,entry,offset,data,data_width);
                            this->update_lru(index,entry);
                            response.success = true;
                        } else {
                        //waiting for bus
                            response.success = false;
                            response.reason = "Waiting for the bus | Bus:" + bus_response.reason;
                        }
                    }
                } else {
                //read data
                    response.data = this->read_from_block(index,entry,offset,data_width);
                    this->update_lru(index,entry);
                    response.success = true;
                }
            } else {
            //Block is invalid (MISS)
                response_t miss_response = this->handle_miss(address, write);
                response.success = false;
                response.reason = "Cache Miss | Miss:" + miss_response.reason;
            }
        }
    }
    return response;
}

//================================
// Read data from a cache block
//================================
uint32_t Cache::read_from_block(uint32_t index, uint32_t entry, uint32_t offset, uint8_t data_width){
    uint32_t data = 0;
    switch(data_width){
        case 0b11:
            data |= this->bank[index][entry].read_byte(offset+3) << 24;
            data |= this->bank[index][entry].read_byte(offset+2) << 16;
        case 0b10:
            data |= this->bank[index][entry].read_byte(offset+1) << 8;
        default:
            data |= this->bank[index][entry].read_byte(offset);
        break;
    }
    return data;
}

//================================
// Write data to a cache block
//================================
void Cache::write_to_block(uint32_t index, uint32_t entry, uint32_t offset, uint32_t data, uint8_t data_width){
    switch(data_width){
        case 0b11:
            this->bank[index][entry].write_byte(offset+3, (data>>24) & 0xFF);
            this->bank[index][entry].write_byte(offset+2, (data>>16) & 0xFF);
        case 0b10:
            this->bank[index][entry].write_byte(offset+1, (data>>8) & 0xFF);
        case 0b01:
            this->bank[index][entry].write_byte(offset, data & 0xFF);
    }
}

//================================
// Convert address to tag
//================================
uint32_t Cache::make_tag(uint32_t address){
    return (address & this->tag_mask) >> (this->index_width + this->offset_width);
}

//================================
// Convert address to index
//================================
uint32_t Cache::make_index(uint32_t address){
    return (address & this->index_mask) >> this->offset_width;
}

//================================
// Convert address to offset
//================================
uint32_t Cache::make_offset(uint32_t address){
    return address & this->offset_mask;
}

//================================
// Search for tag within set
//  returns -1 if missed
//================================
int32_t Cache::find_entry(uint32_t index, uint32_t tag){
    for(int i = 0; i < this->bank[index].size(); i++){
        if(this->bank[index].at(i).get_tag() == tag){
            return i;
        }
    }
    return -1;
}

//================================
// Access Cache Name
//================================
string Cache::get_name(void) const{
    return this->cache_name;
}

//================================
// Compare equality of Caches
//================================
bool Cache::operator == (const Cache& rhs){
    return this->cache_name == rhs.get_name();
}

//================================
// attach bus pointer
//================================
void Cache::attach_bus(Bus* memory_bus){
    this->bus = memory_bus;
}

//================================
// attach core pointer
//================================
void Cache::attach_cpu(Core* processor){
    this->core = processor;
}

//================================
// update LRU for a block/set
//================================
void Cache::update_lru(uint32_t index, uint32_t entry){
    this->bank[index][entry].set_lru(true);
    bool check = true;
    for(int i = 0; i < this->bank[index].size(); i++){
        check = check && this->bank[index][i].get_lru();
    }
    if(check){
        for(int i = 0; i < this->bank[index].size(); i++){
            this->bank[index][i].set_lru(false);
        }
    }
}

//================================
// Find the Least Recently Used entry in a set
//================================
uint32_t Cache::get_lru_entry(uint32_t index){
    for(int i = 0; i < this->bank[index].size(); i++){
        if(!this->bank[index].at(i).get_lru()){
            return i;
        }
    }
    //This should never happen, but it will recover from a fail case
    for(int i = 0; i < this->bank[index].size(); i++){
        this->bank[index].at(i).set_lru(false);
    }
    return 0;
}

//================================
// Handle a cache miss
//================================
response_t Cache::handle_miss(uint32_t address, bool write){
    response_t response;
    response_t bus_response;
    this->miss_tick_counter++;
    uint32_t index = this->make_index(address);
    uint32_t replacement_entry = this->get_lru_entry(index);
    char mesi_status = this->bank[index][replacement_entry].get_mesi();
    if(mesi_status == MESI_M){
    //This block has been modified. It needs to be saved to memory.
        response.success = false;
        bus_response = this->bus->bus_request(this, BUS_SAVE, address, this->bank[index][replacement_entry]);
        if(bus_response.success){
        //block has been saved
            response.reason = "Completed handling the replaced block. Still waiting to read new block";
            this->bank[index][replacement_entry].set_mesi(MESI_I);
        } else {
        //waiting on bus
            response.reason = "Waiting on bus | Bus:" + bus_response.reason;
        }
    } else {
        if(write){
        //write miss. send read with intent to write request
            bus_response = this->bus->bus_request(this, BUS_RWITM, address, Block());
            if(bus_response.success){
            //block has been fetched
                response.success = true;
                Block block = bus_response.block;
                block.set_tag(this->make_tag(address));
                block.set_mesi(MESI_M);
                this->bank[index][replacement_entry] = block;
                response.reason = "Cache Miss resolved. | Bus:" + bus_response.reason;
            } else{
            //waiting on bus
                response.success = false;
                response.reason = "Waiting on the bus | Bus:" + bus_response.reason;
            }
        } else {
        //read miss. send read request
            bus_response = this->bus->bus_request(this, BUS_READ, address, Block());
            if(bus_response.success){
            //block has been fetched
                response.success = true;
                Block block = bus_response.block;
                block.set_tag(this->make_tag(address));
                if(bus_response.data){
                //block was sent from another cache (shared)
                    block.set_mesi(MESI_S);
                } else {
                //block was sent from memory directly (exclusive)
                    block.set_mesi(MESI_E);
                }
                this->bank[index][replacement_entry] = block;
                response.reason = "Cache Miss resolved. | Bus:" + bus_response.reason;
            } else {
            //waiting on bus
                response.success = false;
                response.reason = "Waiting on the bus | Bus:" + bus_response.reason;
            }
        }
    }
    return response;
}

//================================
// set the MESI status of an address (does nothing if block not present)
//================================
void Cache::set_remote_mesi(uint32_t address, char mesi){
    uint32_t index = this->make_index(address);
    uint32_t tag = this->make_tag(address);
    int32_t entry = this->find_entry(index,tag);
    if(entry >= 0){
        this->bank[index][entry].set_mesi(mesi);
    }
}

//================================
// get the MESI status of an address (returns invalid if not found)
//================================
char Cache::get_remote_mesi(uint32_t address){
    uint32_t index = this->make_index(address);
    uint32_t tag = this->make_tag(address);
    int32_t entry = this->find_entry(index,tag);
    if(entry < 0){
        return MESI_I;
    }
    return this->bank[index][entry].get_mesi();
}

//================================
// Bus Arbitrated Cache Snooping
//================================
response_t Cache::snooping(string bus_message, uint32_t address){
        uint32_t tag = this->make_tag(address);
        uint32_t index = this->make_index(address);
        uint32_t offset = this->make_offset(address);
        int32_t entry = this->find_entry(index, tag);
}

