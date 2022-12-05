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
#include <iomanip>
using std::setw;
using std::setfill;
#include <fstream>
using std::string;
using std::vector;
using std::cout;
using std::endl;

//============================================
// Definitions and Constants
//============================================

//============================================
// Main Program
//============================================
int main(void){
    //load program instructions
    //
    vector<int> addr1, addr2, addr3, addr4;
    vector<bool> rw1, rw2, rw3, rw4;
    std::ifstream is1("text/D1addresses.txt");
    if (is1.is_open()) {
        for (int i = 0; i < 100; i++) {
            string temp;
            std::getline(is1, temp);
            bool rwTemp = (temp.substr(0) == "1") ? true : false;
            rw1.push_back(rwTemp);
            temp.erase(temp.begin());
            addr1.push_back(std::stoi(temp, 0, 2));
        }
        is1.close();
    }
    std::ifstream is2("text/D2addresses.txt");
    if (is2.is_open()) {
        for (int i = 0; i < 100; i++) {
            string temp;
            std::getline(is2, temp);
            bool rwTemp = (temp.substr(0) == "1") ? true : false;
            rw2.push_back(rwTemp);
            temp.erase(temp.begin());
            addr2.push_back(std::stoi(temp, 0, 2));
        }
        is2.close();
    }
    std::ifstream is3("text/D3addresses.txt");
    if (is3.is_open()) {
        for (int i = 0; i < 100; i++) {
            string temp;
            std::getline(is3, temp);
            bool rwTemp = (temp.substr(0) == "1") ? true : false;
            rw3.push_back(rwTemp);
            temp.erase(temp.begin());
            addr3.push_back(std::stoi(temp, 0, 2));
        }
        is3.close();
    }
    std::ifstream is4("text/D4addresses.txt");
    if (is4.is_open()) {
        for (int i = 0; i < 100; i++) {
            string temp;
            std::getline(is4, temp);
            bool rwTemp = (temp.substr(0) == "1") ? true : false;
            rw4.push_back(rwTemp);
            temp.erase(temp.begin());
            addr4.push_back(std::stoi(temp, 0, 2));
        }
        is4.close();
    }
    
    //build caches
    cout << "Constructing Caches" << endl;
    vector<Cache> cache = {Cache("cache_0", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_1", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_2", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_3", CACHE_SIZE, BLOCK_SIZE, ASSOCIATIVITY_2_WAY)};
        //override automatic access time calculations
        for(int i = 0; i < cache.size(); i++){
            cache.at(i).set_access_time(CACHE_ACCESS_TICKS);
        }

    //build cores
    //No cores needed. Build fake request feeders
    cout << "Constructing Cores" << endl;
    vector<Core> core = {Core("core_0", &cache.at(0), addr1, rw1),
                         Core("core_1", &cache.at(1), addr2, rw2),
                         Core("core_2", &cache.at(2), addr3, rw3),
                         Core("core_3", &cache.at(3), addr4, rw4)};

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
    uint32_t tick = 1;
    bool halt = false;
    do{
        cout << setfill('=') << setw(80) << "" << endl;
        cout << "Tick " << tick << endl;
        cout << setfill('=') << setw(80) << "" << endl;

        //process clock tick
        halt = true;
        for(int i = 0; i < core.size(); i++){
            bool state = core.at(i).process(tick);
            halt = halt && state;
        }
        //Bus arbiter handoff
        bus.update_token(tick);

        if(halt){
            cout << "\nAll cores complete. Halting..." << endl;
        }

        //Move to next tick
        tick++;
        /*if(tick > 1000){
            //DEBUG PROGRAM HALT
            halt = true;
            cout << "Halting with debug limit" << endl;
        }*/
    }while(!halt);

    return 0;
}