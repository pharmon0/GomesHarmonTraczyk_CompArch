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
    this->token = 0;
    this->bus_active = false;
}

//============================================
// Bus Access
//============================================
response_t Bus::bus_request(Cache* requester, string bus_message, uint32_t address, Block data){
    response_t response;
    if(requester == this->members.at(token)){
        //requester controls the bus. allow action.
        this->bus_active = true;
        if(this->access_counter > 0){
            //still counting down.
            this->access_counter--;
            response.data = 0;
            response.success = false;
            response.reason = "waiting for cache access";
        }else{
            //access time done

        }
    } else {
        //requester does not control the bus. wait.
        response.reason = "Member does not control the bus";
        response.success = false;
    }
    return response;
}

//============================================
// Token Handoff
//============================================
void Bus::update_token(uint32_t tick){
    if(!this->bus_active){
        this->token++;
        if(this->token >= this->members.size()){
            this->token = 0;
        }
    }
}