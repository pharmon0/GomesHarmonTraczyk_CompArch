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
    this->access_ticks = access_time - 1;
    this->access_counter = this->access_ticks;
    this->token = 0;
    this->bus_active = false;
}

//============================================
// Bus Access
//============================================
response_t Bus::bus_request(Cache* requester, string bus_message, uint32_t address, Block data){
    response_t response;
    if(requester->get_name() == this->members.at(token)->get_name()){
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
            for(int i = 0; i < this->members.size(); i++){
                if(i != this->token){
                    response_t snoop_response = this->members.at(i)->snooping(bus_message, address);
                    if(snoop_response.success == true)
                }
            }
        }
    } else {
        //requester does not control the bus. wait.
        response.reason = "Member does not control the bus";
        response.success = false;
    }




    /*
    response_t response;
    if(requester->get_name() == this->members.at(token)->get_name()){
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
            
            //the below is a temporary implementation
            //TODO remove this temporary stuff
            response.block = Block();
            response.data = 0;
            response.success = true;
            response.reason = "PLACEHOLDER CODE";
            this->bus_active = false;
            this->access_counter = this->access_ticks;
            goto TEMPORARY_GOTO;
            //END temporary implementation

            if(bus_message == BUS_INVALIDATE){
                for(int i = 0; i < this->members.size(); i++){
                    if(this->members.at(i)->get_name() != this->members.at(token)->get_name()){
                        this->members.at(i)->set_remote_mesi(address, MESI_I);
                    }
                }
                response.success = true;
                response.reason = "all other members informed to invalidate";
            } else if(bus_message == BUS_READ){
                bool found_data = false;
                for(int i = 0; i < this->members.size(); i++){
                    if(!found_data){
                        uint32_t index = this->members.at(i)->make_index(address);
                        uint32_t tag = this->members.at(i)->make_tag(address);
                        int32_t entry = this->members.at(i)->find_entry(index,tag);
                        if(entry >= 0){
                            char mesi = this->members.at(i)->get_remote_mesi(address);
                            if(mesi == MESI_I){
                            // doesn't count
                                //TODO
                            } else if(mesi == MESI_M){
                            // send to the requester AND save to memory. set sender and requester to S
                                found_data = true;
                                //TODO
                                //TODO figure out how to save to memory (has to stall everything for a long time)
                            } else if(mesi == MESI_S){
                            // send to the requester. set requester to S
                                found_data = true;
                                //TODO
                            } else if(mesi == MESI_E){
                            // send to the requester. set sender and requester to S
                                found_data = true;
                                //TODO
                            }
                        }
                    }
                }
            } else if(bus_message == BUS_RWITM){
                //TODO
            } else if(bus_message == BUS_DUMP){
                int share_count = 0;
                for(int i = 0; i < this->members.size(); i++){
                    char mesi = this->members.at(i)->get_remote_mesi(address);
                    if(mesi == MESI_S){
                        share_count++;
                    }
                }
                if(share_count < 2){
                    for(int i = 0; i < this->members.size(); i++){
                        char mesi = this->members.at(i)->get_remote_mesi(address);
                        if(mesi == MESI_S){
                            this->members.at(i)->set_remote_mesi(address, MESI_E);
                        }
                    }
                }
                response.success = true;
                response.reason = "Operation Complete";
            } else if(bus_message == BUS_SAVE){
                response_t memory_response = this->main_memory->memory_write(address, data);

            }
        }
    } else {
        //requester does not control the bus. wait.
        response.reason = "Member does not control the bus";
        response.success = false;
    }
    TEMPORARY_GOTO: //TODO remove this when the implementation is finished
    return response;
    */
}

//============================================
// Token Handoff
//============================================
void Bus::update_token(uint32_t tick){
    cout << "Applying tick " << tick << " to bus\n\t"
         << "current token holder is " << this->members.at(token)->get_name();
    if(!this->bus_active){
        this->token++;
        if(this->token >= this->members.size()){
            this->token = 0;
        }
        cout << "\n\tToken handoff to " << this->members.at(token)->get_name();
    } else {
        cout << "\n\tBus is Busy. Token handoff blocked";
    }
    cout << endl;
}