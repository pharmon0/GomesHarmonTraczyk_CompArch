/* ECGR 4181/5181 - Computer Architecture
 * Stanly Gomes, Phillip Harmon, Gibert Traczyk
 * Main Program Code */

//============================================
// Libraries
//============================================
#include "Cache.h"
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
//#include <cstring>
#include "Functions.h"
using std::ifstream;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stoul;
using std::tolower;

//============================================
// Constants and Definitions
//============================================
vector<uint32_t> readFile(string filename);

//============================================
// Main Program
//============================================
int main(void){
    vector<uint32_t> addresses = readFile("text/addresses.txt");

    cout << "Lab05::main() | File Read Successfully" << endl;

    vector<Cache> caches = { Cache(256, 32, CACHE_ASS_DIRECT),
                             Cache(512, 32, CACHE_ASS_DIRECT),
                             Cache(256, 64, CACHE_ASS_DIRECT),
                             Cache(256, 32, CACHE_ASS_4WAY)
                             /*Cache(256, 8, CACHE_ASS_DIRECT),
                             Cache(256, 8, CACHE_ASS_2WAY),
                             Cache(256, 8, CACHE_ASS_4WAY),
                             Cache(256, 8, CACHE_ASS_FULLY),

                             Cache(256, 16, CACHE_ASS_DIRECT),
                             Cache(256, 16, CACHE_ASS_2WAY),
                             Cache(256, 16, CACHE_ASS_4WAY),
                             Cache(256, 16, CACHE_ASS_FULLY),

                             Cache(256, 32, CACHE_ASS_DIRECT),
                             Cache(256, 32, CACHE_ASS_2WAY),
                             Cache(256, 32, CACHE_ASS_4WAY),
                             Cache(256, 32, CACHE_ASS_FULLY),
                             
                             Cache(256, 64, CACHE_ASS_DIRECT),
                             Cache(256, 64, CACHE_ASS_2WAY),
                             Cache(256, 64, CACHE_ASS_FULLY), //4-way

                             Cache(256, 128, CACHE_ASS_FULLY) //2-way*/
                             };
    
    //iterate through all caches
    for(int i = 0; i < caches.size(); i++){
       cout << "Lab05::main() | Running Simulation of Cache " << i+1 << endl;
       //read every address
       for(int j = 0; j < addresses.size(); j++){
           cout << "Lab05::main() | attempting to read byte from " << hexString(addresses[j]) << endl;
           caches[i].byteRead(addresses[j],0b01);
       }
       //print stats
       //caches[i].printStats();
       //waiting to the end to print stats now
    }

    for(int i = 0; i < caches.size(); i++){
        cout << "--------------------------" << endl;
        caches[i].printStats();
    }

    // Cache cacheA = Cache(256, 32, CACHE_ASS_DIRECT);
    // cout << "Lab05::main() | Running Simulation of CacheA" << endl;
    // //read every address
    // for(int j = 0; j < addresses.size(); j++){
    //     cout << "Lab05::main() | attempting to read byte from " << hexString(addresses[j]) << endl;
    //     cacheA.byteRead(addresses[j],0b01);
    // }
    // cacheA.printStats();
    

    return 0;
}

//============================================
// Main Program
//============================================
vector<uint32_t> readFile(string filename){
    ifstream file;
    file.open(filename);
    if(!file.is_open()){
        cout << "Lab05::readFile | FILE '" << filename << "' DID NOT OPEN" << endl;
        return {};
    }
    string value;
    vector<uint32_t> result;
    //cout << " Reading File!" << endl;
    while( file >> value ){
        uint32_t item = uint32_t(stoul(value, nullptr, 16));
        result.push_back(item);
    }
    file.close();
    return result;
}
