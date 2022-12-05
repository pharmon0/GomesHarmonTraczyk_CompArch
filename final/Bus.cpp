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
        this->members.push_back(caches.at(i));
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
    if(requester->get_name() == this->members.at(token).get_name()){
        //requester controls the bus. allow action.
        this->bus_active = true;
        if(this->access_counter > 0){
        //still counting down.
            this->access_counter--;
            response.data = 0;
            response.success = false;
            response.reason = "waiting for Bus access";
        }else{//access time done
            
            //saving to memory
            if(bus_message == BUS_SAVE){
                memory_response = this->main_memory->memory_write(address, snoop_response.block);
                if(memory_response.success){
                    response.success = true;
                    this->bus_active = false;
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
                        snoop_response = this->members.at(i).snooping(bus_message, address);
                        
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
                                for(i=i; i < this->members.size(); i++){
                                    if(i != this->token){
                                        this->members.at(i).snooping(BUS_INVALIDATE, address);
                                    }
                                }

                                //return block on RWITM
                                response.block = snoop_response.block;
                                response.success = true;
                                response.block.set_mesi(MESI_M);
                                this->bus_active = false;
                                return response;

                            }
                             
                            //return block on READ or RWITM
                            response.block = snoop_response.block;
                            if (bus_message == BUS_RWITM){
                                response.block.set_mesi(MESI_M);
                            } else if(response.block.get_mesi() == MESI_E){
                                response.block.set_mesi(MESI_S);
                            }
                            response.success = true;
                            bus_active = false;
                            return response;
                            
                        }
                    }
                }

                

                //If data needs to be retrieved from memory
                if((bus_message == BUS_READ) || (bus_message == BUS_RWITM)){
                //Read data from memory
                    memory_response = this->main_memory->memory_read(address);
                    if(memory_response.success){
                        response.success = true;
                        this->bus_active = false;
                        response.block = memory_response.block;
                        if(bus_message == BUS_READ){
                            response.block.set_mesi(MESI_E);
                        }else{
                            response.block.set_mesi(MESI_M);
                        }
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
                    response.success - true;
                    this->bus_active = false;
                    response.block = this->writeback_buffer;
                    if(bus_message == BUS_READ){
                        response.block.set_mesi(MESI_S);
                    }else{
                        response.block.set_mesi(MESI_M);
                    }
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
        return response;
    }
    //should only ever get here on BUS_INVALIDATE
    response.success = true;
    this->bus_active = false;
    return response;
}

//============================================
// Token Handoff
//============================================
void Bus::update_token(uint32_t tick){
    cout << "Applying tick " << tick << " to bus\n\t"
         << "current token holder is " << this->members.at(token).get_name();
    if(!this->bus_active){
        this->token++;
        if(this->token >= this->members.size()){
            this->token = 0;
        }
        cout << "\n\tToken handoff to " << this->members.at(token).get_name();
    } else {
        cout << "\n\tBus is Busy. Token handoff blocked";
    }
    cout << endl;
}