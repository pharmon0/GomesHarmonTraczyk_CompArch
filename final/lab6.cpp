/* ECGR 5181 - Computer Architecture
 * Phillip Harmon
 * Main Program Code */

//============================================
// Headers and Namespaces
//============================================
#include <cstdint>
#include <string>
#include <vector>
#include "Definitions.h"
#include "Cache.h"
#include "Memory.h"
#include "Node.h"
#include "Core.h"
#include <fstream>
using std::string;
using std::vector;

//============================================
// Definitions and Constants
//============================================
#define CACHE_SIZE 65536
#define CACHE_BLOCK_SIZE 64
#define CACHE_ACCESS_TICKS 10
#define MEMORY_BANK_SIZE 262144
#define MEMORY_ACCESS_TICKS 30

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

    //build cores
    //No cores needed. Build fake request feeders
    //FIXME make this actually do stuff
    vector<Core> core = {Core(),
                         Core(),
                         Core(),
                         Core()};

    //build caches
    vector<Cache> cache = {Cache("cache_0", CACHE_SIZE, CACHE_BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_1", CACHE_SIZE, CACHE_BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_2", CACHE_SIZE, CACHE_BLOCK_SIZE, ASSOCIATIVITY_2_WAY),
                           Cache("cache_3", CACHE_SIZE, CACHE_BLOCK_SIZE, ASSOCIATIVITY_2_WAY)};
        //override automatic access time calculations
        for(int i = 0; i < cache.size(); i++){
            cache.at(i).set_access_time(CACHE_ACCESS_TICKS);
        }
        //TODO attach cores to caches
        //No cores needed. Use fake request feeders

    //build memory
    vector<Memory> memory = {Memory("memory_0", MEMORY_BANK_SIZE, MEMORY_ACCESS_TICKS),
                             Memory("memory_1", MEMORY_BANK_SIZE, MEMORY_ACCESS_TICKS),
                             Memory("memory_2", MEMORY_BANK_SIZE, MEMORY_ACCESS_TICKS),
                             Memory("memory_3", MEMORY_BANK_SIZE, MEMORY_ACCESS_TICKS)};

    //build nodes
    vector<Node> node = {Node("node_0", 0, &cache.at(0), &memory.at(0)),
                         Node("node_1", 1, &cache.at(1), &memory.at(1)),
                         Node("node_2", 2, &cache.at(2), &memory.at(2)),
                         Node("node_3", 3, &cache.at(3), &memory.at(3))};
        //interconnect nodes
        for(int i = 0; i < node.size(); i++){
            for(int j = 0; i < node.size(); i++){
                node.at(i).add_connection(&node.at(j));
            }
        }

    //run main program loop
    uint32_t tick = 0;
    bool halt = false;
    do{
        //process clock tick
        //TODO ticks
        //The plan here is that the core-level components will recieve the clock ticks
        // and processes will propogate upwards through the stackup with pointers
        halt = true; //halt will only stay true after this if all cores return true
        for(int i = 0; i < core.size(); i++){
            halt = halt && core.at(i).process(tick);
        }

        //Move to next tick
        tick++;
        if(tick > 10000){
            //DEBUG PROGRAM HALT
            halt = true;
        }
    }while(!halt);

    return 0;
}