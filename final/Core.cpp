/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Pseudo-Core Module */
#include "Core.h"

//============================================
// Core Constructor
//============================================
Core::Core(string name, Cache* mem, vector<int> addr, vector<bool> rw){
    this->addresses = addr;
    this->writes = rw;
    this->cache = mem;
    this->core_name = name;
    this->tick_complete = 0;
    this->iterator = 0;
}

//============================================
// Simulation Tick Process
//============================================
bool Core::process(uint32_t tick){
    cout << this->core_name << " | processing operation number " << this->iterator;
    if(this->writes.at(this->iterator)){
        cout << ", write";
    } else cout << ", read";
    cout << " at address " << bitset<32>(this->addresses.at(this->iterator)) << endl;
    response_t response = this->cache->cache_access(this->addresses.at(this->iterator),
                                                    0,
                                                    this->writes.at(this->iterator),
                                                    0b01);
    if(response.success){
        if(this->iterator < (this->addresses.size()-1)){
            this->iterator++;
            cout << this->core_name << " | Completed operation number " << this->iterator-1 << endl;
        } else{
            if(this->tick_complete == 0){
                this->tick_complete = tick;
            }
            cout << this->core_name << " | Completed Final Request on tick " << tick_complete << endl;
        return true;
        }
    } else {
        cout << this->core_name << " | Incomplete Request (Number " << this->iterator << ") on tick " << tick << ". Reason:\n\t" << response.reason << endl;
    }
    
    return false;
}