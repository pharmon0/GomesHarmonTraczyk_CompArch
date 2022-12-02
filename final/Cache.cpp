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
    cout << this->cache_name << "| offset_width=" << setw(2) << this->offset_width << " mask=" << bitset<32>(this->offset_mask) << endl;
    
    this->index_mask = 0;
    for(int i = 0; i < this->index_width; i++){
        this->index_mask |= (1<<i);
    }
    for(int i = 0; i < this->offset_width; i++){
        this->index_mask = this->index_mask << 1;
    }
    cout << this->cache_name << "|  index_width=" << setw(2) << this->index_width << " mask=" << bitset<32>(this->index_mask) << endl;

    this->tag_mask = 0;
    for(int i = 0; i < this->tag_width; i++){
        this->tag_mask |= (1<<i);
    }
    for(int i = 0; i < (this->offset_width + this->index_width); i++){
        this->tag_mask = this->tag_mask << 1;
    }
    cout << this->cache_name << "|    tag_width=" << setw(2) << this->tag_width << " mask=" << bitset<32>(this->tag_mask) << endl;

    this->access_ticks = ceil(log2(this->bytes_in_cache/this->bytes_in_block));
    this->access_counter = this->access_ticks;

    //TODO populate the bank
}

//================================
// Manually change cache access time
//================================
void Cache::set_access_time(uint32_t ticks){
    this->access_ticks = ticks;
    this->access_counter = ticks;
}

//================================
// Cache Access by CPU
//  - controlled and timed
//================================
response_t Cache::cache_access(uint32_t address, uint32_t data, bool write, uint8_t data_width){
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
            int x = 0; //FIXME continue writing this function
        }
    }
    return response;
}

//================================
// Convert address to tag
//================================
uint32_t Cache::make_tag(uint32_t address){
    return address & this->tag_mask;
}

//================================
// Convert address to index
//================================
uint32_t Cache::make_index(uint32_t address){
    return address & this->index_mask;
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
    return 0; //FIXME make this do shit
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