/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Membus Function Code */
#include "Membus.h"

//============================================
// Constructor
//  Links the membus to all cores and the ram
//   through pointers to their memports
//============================================
Membus::Membus(vector<blockport_t*> caches, blockport_t* memPort){
    this->mem = memPort;
    //this->messageSent = false;
    bool transferDirection = false;
    //bool dataReady = false;
    this->ports = caches;
    this->token = 0;
}

//============================================
// Membus::process
//============================================
void Membus::process(uint64_t tick){
    //switch(this->busState){
    //case BUS_MAIN:
        //bus arbitration
        cout << " Current Token Holder : CPU" << (int)this->token;
        if(!(tick % TICKS_PER_CLOCK)){
            //if the current holder of the token is making no request
            if(!this->ports.at(token)->memctrl.request){
                cout << " | Token Handoff to CPU";
                //rotate token
                this->messageSent = false;
                this->token = (this->token < this->ports.size()-1)?(this->token+1):(0);
                cout << (int)this->token;
            }
        }
        cout << endl;
        //process the memory ports
        //transferDirection controls memory directionality
        
        if(this->transferDirection){
            *(this->mem) = *(this->ports.at(token));
            this->transferDirection = false;
        }else{
            *(this->ports.at(token)) = *(this->mem);
            this->transferDirection = true;
        }
        
        //the above has been replaced by cache MESI communications
        /*
        //check MESI flag of token holder
        if(!this->messageSent){
            this->messageSent = true;
            switch(this->caches.at(token)->membusPort.mesi){
                case REQUEST_M:
                //in this case, the token holder is requesting a block of memory to read
                //first other holder should send their copy. a 'holder' has 'E', 'S', or 'M'
                //holders with 'E' status should update to 'S'
                //token holder gets 'S' if another cache was found to hold the block
                    //TODO implement this case
                    bool dataFound = false;
                    for(int i = 0; i < this->caches.size(); i++){
                        if(!dataFound){
                            int32_t entry = this->caches.at(i)->findEntry(this->caches.at(i)->membusPort.address);
                            if(entry != -1){
                                uint32_t index = this->caches.at(i)->makeIndex(this->caches.at(i)->membusPort.address);
                                if(this->caches.at(i)->bank[index][entry].getMESI() != MESI_I){
                                    //this->caches[i]->bank[index][entry] is the block we need
                                    dataFound = true;
                                    //resolve remote cache MESI state
                                    this->caches.at(i)->bank[index][entry].mesi = MESI_S;
                                    //move the data from cache[i] to cache[token]
                                    this->caches.at(token)->membusPort.data = this->caches.at(i)->bank[index][entry];
                                }
                            }
                        }
                    }
                    if(!dataFound){
                        //no caches had the data
                        //move the data from memory to cache[token]

                    }
                break;
                case REQUEST_MOD_M:
                //in this case, the token holder is requesting a block of memory to write
                //first other holder should send their copy. a 'holder' has 'E', 'S', or 'M'
                //holders with 'M', 'E', or 'S' status should update to 'I'
                //token holder gets 'M'.
                    //TODO implement this case
                break;
                case MOD_M:
                //in this case, the token holder is modifying a block of memory that it already holds.
                //other holders with 'M', 'E', or 'S' should update to 'I'

                break;
                case DUMP_M:
                //in this case, the token holder is dumping a valid, unmodified block.
                //other holders: if only 1 'S', change to 'E'. otherwise, no change.
                    //TODO implement this case
                break;
                case SAVE_M:
                //in this case, the token holder is saving a modified block back to memory.
                //for this to happen, the token holder must be 'M', meaning that all others have 'I'
                //therefore, no snooping updates happen.
                    //TODO implement this case
                break;
            }
        }
    break;
    case BUS_WAITING:
    break;
    }
    */
}