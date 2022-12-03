/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Pseudo-Core Module */
#include "Core.h"

//============================================
// Core Constructor
//============================================
Core::Core(string name, Cache* mem){
    this->cache = mem;
    this->core_name = name;
    this->tick_complete = 0;
}

//============================================
// Simulation Tick Process
//============================================
bool Core::process(uint32_t tick){
    response_t response = this->cache->cache_access(0,0,false,0b11);
    if(response.success){
        if(this->tick_complete == 0){
            this->tick_complete = tick;
        }
        cout << this->core_name << " | Completed Request on tick " << tick_complete << endl;
    } else {
        cout << this->core_name << " | Incomplete Request on tick " << tick << ". Reason:\n\t" << response.reason << endl;
    }
    //TODO make this work for more than a single request and only return true when all requests complete
    return response.success;
}