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
        this->blocks_in_set = this->blocks_in_cache / this->sets_in_cache;
    }

    this->offset_width = ceil(log2(block_bytes));
    this->tag_width = SYSTEM_BITWIDTH - this->offset_width - this->index_width;

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