/* ECGR 5181 - Computer Architecture
 * Phillip Harmon
 * Main Program Code */

//============================================
// Headers and Namespaces
//============================================
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include "Definitions.h"
#include "Cache.h"
#include "Memory.h"
#include "Bus.h"
#include "Core.h"
using std::string;
using std::vector;
using std::cout;
using std::endl;

//============================================
// Definitions and Constants
//============================================
#define BLOCK_SIZE 64
#define CACHE_SIZE 65536
#define MEMORY_SIZE 262144
#define CACHE_ACCESS_TICKS 10
#define MEMORY_ACCESS_TICKS 30
#define BUS_ACCESS_TICKS 10

//============================================
// Main Program
//============================================
int main(void){
    //load program instructions
    //TODO

    //build caches
    cout << "Constructing Caches" << endl;
    vector<Cache> cache = {Cache("cache_0", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_DIRECT),
                           Cache("cache_1", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_2", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_4_WAY),
                           Cache("cache_3", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_FULL)};
        //override automatic access time calculations
        for(int i = 0; i < cache.size(); i++){
            cache.at(i).set_access_time(CACHE_ACCESS_TICKS);
        }

    //build cores
    //No cores needed. Build fake request feeders
    //FIXME make this actually do stuff
    cout << "Constructing Cores" << endl;
    vector<Core> core = {Core(&cache.at(0)),
                         Core(&cache.at(1)),
                         Core(&cache.at(2)),
                         Core(&cache.at(3))};

    //attach cores to caches
    for(int i = 0; i < cache.size(); i++){
        cache.at(i).attach_cpu(&core.at(i));
    }

    //build memory
    cout << "Constructing Memory" << endl;
    Memory memory = Memory("main_memory", MEMORY_SIZE, BLOCK_SIZE, MEMORY_ACCESS_TICKS);

    //build bus
    cout << "Constructing Bus" << endl;
    Bus bus = Bus("memory_bus", cache, &memory, BUS_ACCESS_TICKS);
    
    //attach bus to memory
    memory.attach_bus(&bus);

    //attach bus to caches
    for(int i = 0; i < cache.size(); i++){
        cache.at(i).attach_bus(&bus);
    }

    //run main program loop
    uint32_t tick = 0;
    bool halt = false;
    do{
        //process clock tick
        //TODO ticks
        cout << "Tick " << tick << endl;
        for(int i = 0; i < core.size(); i++){
            core.at(i).process(tick);
        }
        //Bus arbiter handoff
        bus.update_token(tick);

        //Move to next tick
        tick++;
        if(tick > 100){
            //DEBUG PROGRAM HALT
            halt = true;
        }
    }while(!halt);

    return 0;
}