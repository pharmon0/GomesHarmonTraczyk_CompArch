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
    response_t snoop_response;
    response_t memory_response;
    if(requester->get_name() == this->members.at(token)->get_name()){
        //requester controls the bus. allow action.
        this->bus_active = true;
        if(this->access_counter > 0){
        //still counting down.
            this->access_counter--;
            response.data = 0;
            response.success = false;
            response.reason = "waiting for cache access";
        }else{//access time done
            
            //saving to memory
            if(bus_message == BUS_SAVE){
                memory_response = this->main_memory->memory_write(address, snoop_response.block);
                if(memory_response.success){
                    response.success = true;
                } else {
                    response.success = false;
                    response.reason = "Saving to main memory | Memory:" + memory_response.reason;
                }
                return response;
            }

            //while we are not writing to memory
            if(!this->writeback_pending){

                //iterate through every cache looking for data
                for(int i = 0; i < this->members.size(); i++){
                    if(i != this->token){//skip current requester
                        snoop_response = this->members.at(i)->snooping(bus_message, address);
                        
                        //If the data is found
                        if(snoop_response.success){
                            
                            //if found data has a modified tag
                            if(snoop_response.block.get_mesi() == MESI_M){
                            //Block in question is modified. save to memory first.
                                this->writeback_buffer = snoop_response.block;
                                this->writeback_pending = true;
                                memory_response = this->main_memory->memory_write(address, snoop_response.block);
                                //This will always be a memory response false
                                response.success = false;
                                response.reason = "Buffering Modified Read to Main Memory";
                                return response;
                                
                            //if found data is shared and will be modified    
                            } else if((snoop_response.block.get_mesi() == MESI_S) && (bus_message == BUS_RWITM)){
                            //invalidate others
                                for(i; i < this->members.size(); i++){
                                    if(i != this->token){
                                        this->members.at(i)->snooping(BUS_INVALIDATE, address);
                                    }
                                }
                                response.block = snoop_response.block;
                                response.success = true;
                                return response;
                            }
                            
                        }
                    }
                }

                //If data needs to be retrieved from memory
                if((bus_message == BUS_READ) || (bus_message == BUS_RWITM)){
                //Read data from memory
                    memory_response = this->main_memory->memory_read(address);
                    if(memory_response.success){
                        response.success = true;
                        response.block = memory_response.block;
                    } else {
                        response.success = false;
                        response.reason = "Reading from main memory | Memory:" + memory_response.reason;
                    }
                    return response;
                }
            } else {
            //Writeback from a read on a MESI_M block to memory pending
                memory_response = this->main_memory->memory_write(address, snoop_response.block);
                if(memory_response.success){
                //writeback complete
                    response.success - false;
                    response.reason = "Completed Buffering Modified block to main memory";
                    writeback_pending = false;
                } else{
                    response.success = false;
                    response.reason = "Buffering Modified Read to Main Memory | Memory:" + memory_response.reason;
                }
                return response;
            }
        }
    } else {
        //requester does not control the bus. wait.
        response.reason = "Member does not control the bus";
        response.success = false;
    }
    //should only ever get here on BUS_INVALIDATE
    response.success = true;
    return response;
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
            //T ODO remove this temporary stuff
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
                                //T ODO
                            } else if(mesi == MESI_M){
                            // send to the requester AND save to memory. set sender and requester to S
                                found_data = true;
                                //T ODO
                                //T ODO figure out how to save to memory (has to stall everything for a long time)
                            } else if(mesi == MESI_S){
                            // send to the requester. set requester to S
                                found_data = true;
                                //T ODO
                            } else if(mesi == MESI_E){
                            // send to the requester. set sender and requester to S
                                found_data = true;
                                //T ODO
                            }
                        }
                    }
                }
            } else if(bus_message == BUS_RWITM){
                //T ODO
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
    TEMPORARY_GOTO: //T ODO remove this when the implementation is finished
    return response;
    */

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