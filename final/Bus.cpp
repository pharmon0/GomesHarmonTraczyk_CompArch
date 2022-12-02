/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Arbitrated Memory Bus Module */
#include "Bus.h"


//============================================
// Bus Constructor
//============================================
Bus::Bus(string name, vector<Cache> caches, Memory* memory, uint32_t access_time){
    this->bus_name = name;
    this->main_memory = memory;
    for(int i = 0; i < caches.size(); i++){
        this->members.push_back(&caches.at(i));
    }
    this->access_counter = access_time;
    this->access_ticks = access_time;
}


//============================================
// Bus Access
//============================================
response_t Bus::bus_request(Cache* requester){
    response_t response;
    if(requester == this->members.at(token)){
        //requester controls the bus. allow action.

    } else {
        //requester does not control the bus. wait.
        response.reason = "Member does not control the bus";
        response.success = false;
    }
    return response;
}